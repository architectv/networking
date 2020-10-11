#include "convert.h"

#define PRINT_ERR(err_cnt, index, got, res) \
    printf("[#%d] [func : %s] [case : %d] [Got %s, expected %s]\n", \
    err_cnt, __FUNCTION__, index + 1, got, res);

#define SUCCESS_TEST() \
    printf("---OK------%s---\n", __FUNCTION__);

#define FAIL_TEST() \
    printf("---FAIL------%s---\n", __FUNCTION__);

#define PRINT_ALL_ERR() \
    printf("---Sum Of Errors : %d---\n", all_err_cnt);    

struct test_case {
    int number;
    char* expected;
};

int all_err_cnt = 0;

void test_binary(void) {
    int base = 2;
    struct test_case cases[] = {
        { 0, "0" },
        { 1, "1" },
        { 2, "10" },
        { 9, "1001" },
        { 23, "10111" },
        { 127, "1111111" },
        { -1, "-1" },
        { 1234567, "100101101011010000111" },
    };

    int err_cnt = 0;
    int len = sizeof(cases) / sizeof(cases[0]);
    for (int i = 0; i < len; i++) {
        int num = cases[i].number;
        char* res = cases[i].expected;
        char* got = convert(num, base);
        int check = strcmp(res, got);
        if (check != 0) {
            err_cnt++;
            PRINT_ERR(err_cnt, i, got, res);
        }
    }

    if (0 == err_cnt) {
        SUCCESS_TEST();
    }
    else {
        FAIL_TEST();
    }

    all_err_cnt += err_cnt;
}

void test_octonary(void) {
    int base = 8;
    struct test_case cases[] = {
        { 1, "1" },
        { 2, "2" },
        { 9, "11" },
        { 23, "27" },
        { 127, "177" },
        { -1, "-1" },
        { 1234567, "4553207" },
    };

    int err_cnt = 0;
    int len = sizeof(cases) / sizeof(cases[0]);
    for (int i = 0; i < len; i++) {
        int num = cases[i].number;
        char* res = cases[i].expected;
        char* got = convert(num, base);
        int check = strcmp(res, got);
        if (check != 0) {
            err_cnt++;
            PRINT_ERR(err_cnt, i, got, res);
        }
    }

    if (0 == err_cnt) {
        SUCCESS_TEST();
    }
    else {
        FAIL_TEST();
    }

    all_err_cnt += err_cnt;
}

void test_hexadecimal(void) {
    int base = 16;
    struct test_case cases[] = {
        { 1, "1" },
        { 2, "2" },
        { 9, "9" },
        { 23, "17" },
        { 127, "7F" },
        { -1, "-1" },
        { 1234567, "12D687" },
    };

    int err_cnt = 0;
    int len = sizeof(cases) / sizeof(cases[0]);
    for (int i = 0; i < len; i++) {
        int num = cases[i].number;
        char* res = cases[i].expected;
        char* got = convert(num, base);
        int check = strcmp(res, got);
        if (check != 0) {
            err_cnt++;
            PRINT_ERR(err_cnt, i, got, res);
        }
    }

    if (0 == err_cnt) {
        SUCCESS_TEST();
    }
    else {
        FAIL_TEST();
    }

    all_err_cnt += err_cnt;
}

int main(void) {
    test_binary();
    test_octonary();
    test_hexadecimal();
    PRINT_ALL_ERR();
}
