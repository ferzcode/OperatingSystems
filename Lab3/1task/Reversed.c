#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utility.h"

void reverse_dir(char* path, char* dst_path, __mode_t mode) {
    DIR* dir_stream = opendir(path);
    if (dir_stream == NULL) {
        opendir_err(errno, path);
        return;
    }

    if (path[strlen(path) - 1] == '/') { // Checks if the directory path ends with a slash
        // If yes, removes this character to avoid unnecessary slashes in the path.
        path[strlen(path) - 1] = 0;
    }

    //Get only the short name of the directory, without the path to it
    char* dir_name = get_short_name(path);

    if (is_invalid(dir_name)) {
        printf("Invalid directory name.\n");
        return;
    }

    //Directory name reversal
    char* rev_dir_name = reverse_string(dir_name);
    //Creates a new path for a new directory by concatenating dst_path, reversed name and slash
    char* next_dst_path = join(dst_path, rev_dir_name, '/');

    //Creates a new directory with reversed name and permissions
    mkdir(next_dst_path, mode);
    printf("%s -> %s\n", path, next_dst_path);

    //Reading directory contents
    struct dirent* content;
    while ((content = readdir(dir_stream)) != NULL) {
        if (is_invalid(content->d_name))
            continue;


        char* full_content_name = join(path, content->d_name, '/');

        //Determining the element type (File or Directory)
        struct stat content_info;
        stat(full_content_name, &content_info);

        if (S_ISDIR(content_info.st_mode)) {
            reverse_dir(full_content_name, next_dst_path, content_info.st_mode);
        }

        else if (S_ISREG(content_info.st_mode)) {
            make_rev_file(full_content_name, next_dst_path, content_info.st_mode);
        }

        free(full_content_name);
    }

    free(rev_dir_name);
    free(next_dst_path);
    closedir(dir_stream);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Directory name expected.\n");
        return -1;
    }

    struct stat dir_info;
    stat(argv[1], &dir_info);
    reverse_dir(argv[1], ".", dir_info.st_mode);
}