#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ROWS 6
#define COLS 5
#define P_SIZE 30

// Function prototypes
void prepareKeyMatrix(char key[], char keyMatrix[ROWS][COLS]);
void getPosition(char keyMatrix[ROWS][COLS], char ch, int *row, int *col);
void encryptPlayfair(char keyMatrix[ROWS][COLS], char *plaintext, char *ciphertext);
char affineEncrypt(char ch, int a, int b);
void shiftEncrypt(char *plaintext, char *ciphertext, int k); // Modified prototype

int main() {
    char plaintext[100];
    char keyMatrix[ROWS][COLS];
    char key[30];
    char ciphertext1[100], ciphertext2[100], ciphertext3[100]; // Modify ciphertext variables
    int a = 11, b = 15, k3;

    // Input plaintext
    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; // Remove newline character

    // Remove spaces from the plaintext and handle repeating characters
    int index = 0;
    char adjusted_plaintext[100];
    for (int i = 0; plaintext[i] != '\0'; ++i) {
        if (!isspace(plaintext[i])) {
            if (plaintext[i] == 'J')
                plaintext[i] = 'I';
            adjusted_plaintext[index++] = plaintext[i];
            if (plaintext[i] == plaintext[i + 1]) {
                adjusted_plaintext[index++] = 'X'; // Insert 'X' between repeating characters
            }
        }
    }
    adjusted_plaintext[index] = '\0'; // Null-terminate the string

    printf("Adjusted plaintext: %s\n", adjusted_plaintext);

    // Input first key (K1)
    printf("Enter the first key (K1): ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0'; // Remove newline character

    // Prepare key matrix (K1)
    prepareKeyMatrix(key, keyMatrix);

    // Print key matrix (K1)
    printf("Key Matrix (K1):\n");
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            printf("%c ", keyMatrix[i][j]);
        }
        printf("\n");
    }

    // Encryption using Playfair cipher (C1)
    printf("Encryption using Playfair cipher (C1):\n");
    encryptPlayfair(keyMatrix, adjusted_plaintext, ciphertext1);
    printf("%s\n", ciphertext1);

    // Encryption using Affine cipher (C2)
    printf("Encryption using Affine cipher (C2):\n");
    for (int i = 0; adjusted_plaintext[i] != '\0'; ++i) {
        printf("%c", affineEncrypt(adjusted_plaintext[i], a, b));
    }
    printf("\n");

    // Encryption using Shift cipher (C3)
    do {
        printf("Enter the value of K3 (0 <= K3 <= 29): ");
        scanf("%d", &k3);
        getchar(); // Clear the input buffer

        if (k3 < 0 || k3 > 29) {
            printf("Invalid input! K3 must be between 0 and 29.\n");
        }
    } while (k3 < 0 || k3 > 29);

    shiftEncrypt(adjusted_plaintext, ciphertext3, k3);
    printf("Encryption using Shift cipher (C3):\n%s\n", ciphertext3);

    return 0;
}

// Function to prepare the key matrix
void prepareKeyMatrix(char key[], char keyMatrix[ROWS][COLS]) {
    int k = 0;
    char *alphabet = strdup("ABCDEFGHIJKLMNOPQRSTUVWXYZ,.;?");
    int len = strlen(key);
    int index[P_SIZE] = {0};

    // Initialize index array with 0s
    for (int i = 0; i < P_SIZE; ++i) {
        index[i] = 0;
    }

    // Fill the key into keyMatrix
    for (int i = 0; i < len; ++i) {
        char ch = toupper(key[i]);
        if (ch == 'J')
            ch = 'I'; // Handling I/J
        if (!index[ch - 'A']) {
            keyMatrix[k / COLS][k % COLS] = ch;
            index[ch - 'A'] = 1;
            k++;
        }
    }

    // Fill the remaining matrix with the alphabet
    for (int i = 0; i < strlen(alphabet); ++i) {
        char ch = alphabet[i];
        if (!index[ch - 'A']) {
            keyMatrix[k / COLS][k % COLS] = ch;
            index[ch - 'A'] = 1;
            k++;
        }
    }

    free(alphabet); // Free dynamically allocated memory
}


// Function to get the position of a character in the key matrix
void getPosition(char keyMatrix[ROWS][COLS], char ch, int *row, int *col) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (keyMatrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

// Function to encrypt a digraph using Playfair cipher
void encryptPlayfair(char keyMatrix[ROWS][COLS], char *plaintext, char *ciphertext) {
    int i = 0;
    int plaintext_length = strlen(plaintext);
    while (i < plaintext_length) {
        char a = plaintext[i];
        char b = (i + 1 < plaintext_length) ? plaintext[i + 1] : 'X'; // Handling odd length plaintext
        int row1, col1, row2, col2;
        getPosition(keyMatrix, a, &row1, &col1);
        getPosition(keyMatrix, b, &row2, &col2);

        if (row1 == row2) {
            ciphertext[i] = keyMatrix[row1][(col1 + 1) % COLS];
            ciphertext[i + 1] = keyMatrix[row2][(col2 + 1) % COLS];
        } else if (col1 == col2) {
            ciphertext[i] = keyMatrix[(row1 + 1) % ROWS][col1];
            ciphertext[i + 1] = keyMatrix[(row2 + 1) % ROWS][col2];
        } else {
            ciphertext[i] = keyMatrix[row1][col2];
            ciphertext[i + 1] = keyMatrix[row2][col1];
        }
        i += 2;
    }
    ciphertext[i] = '\0'; // Null-terminate the ciphertext string
}


// Function to encrypt using Affine cipher
char affineEncrypt(char ch, int a, int b) {
    if (isalpha(ch) && isupper(ch)) {
        return ((a * (ch - 'A') + b) % 26) + 'A';
    } else {
        return ch; // Return the character unchanged if it's not an uppercase alphabet
    }
}

// Function to encrypt using Shift cipher
void shiftEncrypt(char *plaintext, char *ciphertext, int k) {
    for (int i = 0; plaintext[i] != '\0'; ++i) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            ciphertext[i] = ((plaintext[i] - base + k) % 26) + base;
        } else {
            ciphertext[i] = plaintext[i]; // Non-alphabetic characters remain unchanged
        }
    }
    ciphertext[strlen(plaintext)] = '\0'; // Null-terminate the ciphertext string
}