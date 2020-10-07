#include "convert.h"

int is_zero(const char* digits, int len) {
    for (int i = 0; i < len; i++) {
        if (digits[i] != '0') {
            return 0;
        }
    }
    return 1;
}

int resize(char** digits, int cap) {
    cap *= 2;
    char* tmp = realloc(*digits, cap * sizeof(char));
    if (tmp != NULL) {
        *digits = tmp;
        return cap;
    }
    else {
        free(*digits);
        return ALLOC_ERR;
    }
}

void reverse(char* digits, int len) {
    for (int i = 0; i < len / 2; i++) {
        int j = len - i - 1;
        char tmp = digits[i];
        digits[i] = digits[j];
        digits[j] = tmp;
    }
}

char int_to_char(int number) {
    char symbol;
    if (number >= 0 && number <= 9) {
        symbol = number + '0';
    }
    else {
        symbol = number + 'A' - 10;
    }
    return symbol;
}

char* convert(int input, int base) {
    int negative = 0;
    if (input < 0) {
        input *= (-1);
        negative = 1;
    }

    int cap = 2;
    int len = 0;
    char* digits = calloc(cap, sizeof(char));
    for (; input != 0; len++) {
        int mod = input % base;
        char digit = int_to_char(mod);

        if (len > cap - 1) {
            cap = resize(&digits, cap);
            if (cap < 0) {
                return NULL;
            }
        }

        digits[len] = digit;
        input /= base;
    }

    if (is_zero(digits, len)) {
        free(digits);
        return ZERO;
    }

    if (negative) {
        if (len > cap - 1) {
            cap = resize(&digits, cap);
            if (cap < 0) {
                return NULL;
            }
        }

        digits[len] = '-';
        len++;
    }

    if (len > cap - 1) {
        cap = resize(&digits, cap);
        if (cap < 0) {
            return NULL;
        }
    }
    digits[len] = '\0';

    reverse(digits, len);

    return digits;
}
