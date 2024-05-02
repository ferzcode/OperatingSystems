#define _POSIX_C_SOURCE 200112L

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utility.h"

int is_invalid(const char* name) {
    return (name == NULL || name == "" ||
            strcmp(".", name) == 0 || strcmp("..", name) == 0);
}

char* join(const char* str1, const char* str2, char symb) {
    size_t joined_len = strlen(str1) + strlen(str2) + 2;
    char* joined = malloc(joined_len * sizeof(*joined));
    snprintf(joined, joined_len, "%s%c%s", str1, symb, str2);
    return joined;
}

char* concat(const char* str1, const char* str2) {
    size_t res_len = strlen(str1) + strlen(str2) + 1;
    char* res = malloc(res_len * sizeof(*res));
    snprintf(res, res_len, "%s%s", str1, str2);
    return res;
}

char* get_short_name(char* full_name) {
    char* name = strrchr(full_name, '/');
    if (name == NULL) {
        return full_name;
    }
    return name + 1;
}

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

static char ftypelet(mode_t bits) {
    /* These are the most common, so test for them first.  */
    if (S_ISREG(bits))
        return '-';
    if (S_ISDIR(bits))
        return 'd';

    /* Other letters standardized by POSIX 1003.1-2004.  */
    if (S_ISBLK(bits))
        return 'b';
    if (S_ISCHR(bits))
        return 'c';
    if (S_ISLNK(bits))
        return 'l';
    if (S_ISFIFO(bits))
        return 'p';

    /* Other file types (though not letters) standardized by POSIX.  */
    if (S_ISSOCK(bits))
        return 's';

    return '?';
}

void strmode(mode_t mode, char *str) {
    str[0] = ftypelet(mode);
    str[1] = mode & S_IRUSR ? 'r' : '-';
    str[2] = mode & S_IWUSR ? 'w' : '-';
    str[3] = (mode & S_ISUID
              ? (mode & S_IXUSR ? 's' : 'S')
              : (mode & S_IXUSR ? 'x' : '-'));
    str[4] = mode & S_IRGRP ? 'r' : '-';
    str[5] = mode & S_IWGRP ? 'w' : '-';
    str[6] = (mode & S_ISGID
              ? (mode & S_IXGRP ? 's' : 'S')
              : (mode & S_IXGRP ? 'x' : '-'));
    str[7] = mode & S_IROTH ? 'r' : '-';
    str[8] = mode & S_IWOTH ? 'w' : '-';
    str[9] = (mode & __S_ISVTX
              ? (mode & S_IXOTH ? 't' : 'T')
              : (mode & S_IXOTH ? 'x' : '-'));
    str[10] = ' ';
    str[11] = '\0';
}

static int str_is_num(const char* str, size_t len) {
    for (int i = 0; i < len; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

static int convert_oct_to_dec(int num_oct) {
    int num_dec = 0;
    int mult = 1;
    while (num_oct != 0) {
        int digit = num_oct % 10;
        if (digit < 0 || digit >= 8) {
            return -1;
        }
        num_dec += digit * mult;
        mult *= 8;
        num_oct /= 10;
    }
    return num_dec;
}

mode_t parse_mode(const char* str) {
    if (!str_is_num(str, strlen(str))) {
        return -1;
    }
    mode_t mode_oct = atoi(str);
    return convert_oct_to_dec(mode_oct);
}