#ifndef ERROR_PRINTERS_H_INCLUDED
#define ERROR_PRINTERS_H_INCLUDED

int is_invalid(const char* name);

char* join(const char* str1, const char* str2, char symb);

char* concat(const char* str1, const char* str2);

char* get_short_name(char* path);

void print_file(FILE* file);

void strmode(mode_t mode, char *str);

mode_t parse_mode(const char* str);

#endif