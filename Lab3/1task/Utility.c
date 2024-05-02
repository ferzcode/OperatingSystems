#include <errno.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utility.h"

int is_invalid(char* name) {
    return (name == NULL || name == "" ||
            strcmp(".", name) == 0 || strcmp("..", name) == 0);
}

void opendir_err(int errorCode, char* path) {
    switch (errorCode) {
        case EACCES:
            printf("Permission denied.\n");
            break;
        case ENOENT:
            printf("Directory does not exist, or \"%s\" is an empty string.\n", path);
            break;
        case ENOTDIR:
            printf("\"%s\" is not a directory.\n", path);
            break;
        default:
            printf("Unknown error.\n");
            break;
    }
}

char* reverse_string(char* string) {
    size_t len = strlen(string);
    char* reversed = malloc((len + 1) * sizeof(*reversed));

    for (size_t i = 0; i < len; i++) {
        reversed[i] = string[len - i - 1];
    }
    reversed[len] = 0;
    return reversed;
}

char* join(char* str1, char* str2, char symb) {
    //One character for the symb delimiter and one more character for the terminating zero are considered (+2)
    size_t joined_len = strlen(str1) + strlen(str2) + 2;
    char* joined = malloc(joined_len * sizeof(*joined));
    snprintf(joined, joined_len, "%s%c%s", str1, symb, str2);
    return joined;
}

static void reverse_array(char* buff, int len) {
    for (int i = 0, j = len - 1; i < j; i++, j--) {
        char tmp = buff[i];
        buff[i] = buff[j];
        buff[j] = tmp;
    }
}

static void rev_file_content(FILE* src, FILE* dst) {
    enum {BUFF_SIZE = 100};
    char buff[BUFF_SIZE];

    //This line sets the file position pointer to the end of file src.
    // This is done to get the length of the file using ftell.
    fseek(src, 0, SEEK_END);
    long length = ftell(src);
    int iter = length / BUFF_SIZE;
    int remainder = length - BUFF_SIZE * iter;

    for (int i = 1; i <= iter; i++) {
        fseek(src, -(BUFF_SIZE * i), SEEK_END);
        int bytes_read = fread(buff, 1, BUFF_SIZE, src);
        reverse_array(buff, BUFF_SIZE);

        int bytes_written = 0;
        while (bytes_written < bytes_read) {
            bytes_written += fwrite(buff + bytes_written, 1, bytes_read - bytes_written, dst);
        }
    }

    fseek(src, 0, SEEK_SET);
    int bytes_read = fread(buff, 1, remainder, src);
    reverse_array(buff, remainder);

    int bytes_written = 0;
    while (bytes_written < bytes_read) {
        bytes_written += fwrite(buff + bytes_written, 1, bytes_read - bytes_written, dst);
    }
}

void make_rev_file(char* file_name, char* dst_path, __mode_t mode) {
    char* short_name = get_short_name(file_name);
    if (is_invalid(short_name)) {
        return;
    }

    char* rev_name = reverse_string(short_name);
    char* full_rev_name = join(dst_path, rev_name, '/');

    printf("%s -> %s\e[0m\n", file_name, full_rev_name);


    FILE* file = fopen(file_name, "rb");
    if (file == NULL) {
        return;
    }
    FILE* rev_file = fopen(full_rev_name, "wb");

    rev_file_content(file, rev_file);

    fclose(file);
    fclose(rev_file);

    chmod(full_rev_name, mode);

    free(rev_name);
    free(full_rev_name);
}

char* get_short_name(char* full_name) {
    char* name = strrchr(full_name, '/');
    if (name == NULL) {
        return full_name;
    }
    return name + 1;
}