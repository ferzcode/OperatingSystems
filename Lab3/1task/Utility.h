#ifndef ERROR_PRINTERS_H_INCLUDED
#define ERROR_PRINTERS_H_INCLUDED

int is_invalid(char* name);

void opendir_err(int errorCode, char* path);

char* reverse_string(char* string);

char* join(char* str1, char* str2, char symb);

void make_rev_file(char* file_name, char* dst_path, __mode_t mode);

char* get_short_name(char* path);

#endif //ERROR_PRINTERS_H_INCLUDED