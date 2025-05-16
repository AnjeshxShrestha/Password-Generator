#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define UPPERCASE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWERCASE "abcdefghijklmnopqrstuvwxyz"
#define DIGITS "0123456789"
#define SYMBOLS "!@#$%^&*()-_=+[]{}|;:,.<>?/`~"

void shuffle(char *array, size_t n) {
    if (n > 1) {
        for (size_t i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            char t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

char *generate_password(int length, int use_upper, int use_lower, int use_digits, int use_symbols) {
    char pool[256] = "";
    if (use_upper) strcat(pool, UPPERCASE);
    if (use_lower) strcat(pool, LOWERCASE);
    if (use_digits) strcat(pool, DIGITS);
    if (use_symbols) strcat(pool, SYMBOLS);

    if (strlen(pool) == 0 || length <= 0) return NULL;

    char *password = malloc(length + 1);
    for (int i = 0; i < length; i++) {
        password[i] = pool[rand() % strlen(pool)];
    }
    password[length] = '\0';
    shuffle(password, length);
    return password;
}

int main() {
    srand(time(NULL));
    int length;
    printf("Enter desired password length: ");
    scanf("%d", &length);

    char *password = generate_password(length, 1, 1, 1, 1);
    if (password) {
        printf("Generated Password: %s\n", password);
        free(password);
    } else {
        printf("Invalid input or failed to generate password.\n");
    }
    return 0;
}
