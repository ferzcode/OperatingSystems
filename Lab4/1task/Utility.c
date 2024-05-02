#include <stdio.h>

#include "Utility.h"

void print_file(FILE* file) {
    enum {BUFF_SIZE = 100};
    char buffer[BUFF_SIZE];

    while (!feof(file)) {
        int bytes_read = fread(buffer, 1, BUFF_SIZE, file);
        int bytes_written = 0;
        while (bytes_written < bytes_read) {
            bytes_written += fwrite(buffer + bytes_written, 1, bytes_read - bytes_written, stdout);
        }
    }
    fflush(stdout);
}

void print_buffer(char *buffer, int size) {
    for (int i = 0; i < size; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");
}