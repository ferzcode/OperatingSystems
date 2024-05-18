#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>

#include "Utility.h"

int global_init = 1;
int global_uninit;
const int global_const = 2;

void foo() {
    int local = 10;
    static int local_static = 15;
    const int local_const = 20;

    printf("local: %lx\nlocal static: %lx\nlocal const: %lx\n",
           (size_t) &local, (size_t) &local_static, (size_t) &local_const);
}

size_t bar() {
    int bar_local = 10;
    return (size_t) &bar_local;
}

void func() {
    enum {BUFF_SIZE = 100};

    char *buffer = malloc(BUFF_SIZE);
    snprintf(buffer, sizeof("hello world"), "hello world");
    print_buffer(buffer, BUFF_SIZE);
    free(buffer);
    printf("AFTER FREE:\n");
    print_buffer(buffer, BUFF_SIZE);

    char *buffer2 = malloc(BUFF_SIZE);
    snprintf(buffer, sizeof("hello world 2"), "hello world 2");
    print_buffer(buffer2, BUFF_SIZE);
    // buffer2 += BUFF_SIZE / 2;
    // free(buffer2);
    // print_buffer(buffer2, BUFF_SIZE);

    free(buffer2);
}

void func_env_var() {
    char *var_value = getenv("MYVAR");
    if (var_value != NULL) {
        printf("%s\n", var_value);
    }

    if (setenv("MYVAR", "hello", 1) == -1) {
        perror("setenv");
        return;
    }

    var_value = getenv("MYVAR");
    if (var_value != NULL) {
        printf("%s\n", var_value);
    }
}

int main() {
    FILE *maps = fopen("/proc/self/maps", "r");
    if (maps == NULL) {
        perror("fopen");
        return -1;
    }
    print_file(maps);
    fclose(maps);

    printf("========================================================================\n");

    foo();
    printf("global: %lx\nglobal uninitialized: %lx\nglobal const: %lx\n",
           (size_t) &global_init, (size_t) &global_uninit, (size_t) &global_const);

    printf("========================================================================\n");
    printf("Bar local: %lx\n", bar());

    printf("========================================================================\n");
    func();

    printf("========================================================================\n");
    func_env_var();
}