#ifndef UTILITY_H
#define UTILITY_H

#include <sys/stat.h>

int is_invalid(const char* name);

void opendir_err(int errorCode, const char* path);

char* reverse_string(const char* string);

char* join(const char* str1, const char* str2, char symb);

void make_rev_file(const char* file_name, const char* dst_path, __mode_t mode);

char* get_short_name(const char* path);

#endif // UTILITY_H
