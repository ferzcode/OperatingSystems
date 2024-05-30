#include <errno.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utility.h"

int is_invalid(const char* name) {
    return (name == NULL || *name == '\0' ||
            strcmp(".", name) == 0 || strcmp("..", name) == 0);
}

void opendir_err(int errorCode, const char* path) {
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

char* reverse_string(const char* string) {
    size_t len = strlen(string);
    char* reversed = malloc((len + 1) * sizeof(*reversed));
    if (!reversed) {
        perror("Failed to allocate memory for reversed string");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < len; i++) {
        reversed[i] = string[len - i - 1];
    }
    reversed[len] = '\0';
    return reversed;
}

char* join(const char* str1, const char* str2, char symb) {
    size_t joined_len = strlen(str1) + strlen(str2) + 2;
    char* joined = malloc(joined_len * sizeof(*joined));
    if (!joined) {
        perror("Failed to allocate memory for joined string");
        exit(EXIT_FAILURE);
    }
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

    fseek(src, 0, SEEK_END);
    long length = ftell(src);
    int iter = length / BUFF_SIZE;
    int remainder = length % BUFF_SIZE;

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

void make_rev_file(const char* file_name, const char* dst_path, __mode_t mode) {
    char* short_name = get_short_name(file_name);
    if (short_name == NULL) {
        return;
    }

    if (is_invalid(short_name)) {
        free(short_name);
        return;
    }

    char* rev_name = reverse_string(short_name);
    free(short_name);
    if (!rev_name) {
        perror("Failed to reverse string");
        return;
    }

    char* full_rev_name = join(dst_path, rev_name, '/');
    free(rev_name);
    if (!full_rev_name) {
        perror("Failed to join strings");
        return;
    }

    printf("%s -> %s\n", file_name, full_rev_name);

    FILE* file = fopen(file_name, "rb");
    if (!file) {
        perror("Failed to open source file");
        free(full_rev_name);
        return;
    }

    FILE* rev_file = fopen(full_rev_name, "wb");
    if (!rev_file) {
        perror("Failed to open destination file");
        fclose(file);
        free(full_rev_name);
        return;
    }

    rev_file_content(file, rev_file);

    fclose(file);
    fclose(rev_file);

    if (chmod(full_rev_name, mode) != 0) {
        perror("Failed to set file permissions");
    }

    free(full_rev_name);
}

char* get_short_name(const char* full_name) {
    const char* name = strrchr(full_name, '/');
    if (!name) {
        return strdup(full_name);
    }
    return strdup(name + 1);
}
