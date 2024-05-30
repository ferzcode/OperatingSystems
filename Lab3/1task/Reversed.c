#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utility.h"

void reverse_dir(const char* path, const char* dst_path, __mode_t mode) {
    DIR* dir_stream = opendir(path);
    if (dir_stream == NULL) {
        opendir_err(errno, path);
        return;
    }

    char* mutable_path = strdup(path);
    if (mutable_path == NULL) {
        perror("Failed to allocate memory for mutable_path");
        closedir(dir_stream);
        return;
    }

    if (mutable_path[strlen(mutable_path) - 1] == '/') {
        mutable_path[strlen(mutable_path) - 1] = '\0';
    }

    char* dir_name = get_short_name(mutable_path);
    free(mutable_path);
    if (is_invalid(dir_name)) {
        printf("Invalid directory name.\n");
        free(dir_name);
        closedir(dir_stream);
        return;
    }

    char* rev_dir_name = reverse_string(dir_name);
    free(dir_name);
    if (!rev_dir_name) {
        perror("Failed to reverse string");
        closedir(dir_stream);
        return;
    }

    char* next_dst_path = join(dst_path, rev_dir_name, '/');
    free(rev_dir_name);
    if (!next_dst_path) {
        perror("Failed to join strings");
        closedir(dir_stream);
        return;
    }

    if (mkdir(next_dst_path, mode) != 0 && errno != EEXIST) {
        perror("Failed to create directory");
        free(next_dst_path);
        closedir(dir_stream);
        return;
    }
    printf("%s -> %s\n", path, next_dst_path);

    struct dirent* content;
    while ((content = readdir(dir_stream)) != NULL) {
        if (is_invalid(content->d_name)) {
            continue;
        }

        char* full_content_name = join(path, content->d_name, '/');
        if (!full_content_name) {
            perror("Failed to allocate memory for full_content_name");
            continue;
        }

        struct stat content_info;
        if (stat(full_content_name, &content_info) != 0) {
            perror("Failed to get file status");
            free(full_content_name);
            continue;
        }

        if (S_ISDIR(content_info.st_mode)) {
            reverse_dir(full_content_name, next_dst_path, content_info.st_mode);
        } else if (S_ISREG(content_info.st_mode)) {
            make_rev_file(full_content_name, next_dst_path, content_info.st_mode);
        }

        free(full_content_name);
    }

    free(next_dst_path);
    closedir(dir_stream);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Directory name expected.\n");
        return EXIT_FAILURE;
    }

    struct stat dir_info;
    if (stat(argv[1], &dir_info) != 0) {
        perror("Failed to get directory status");
        return EXIT_FAILURE;
    }

    if (!S_ISDIR(dir_info.st_mode)) {
        printf("\"%s\" is not a directory.\n", argv[1]);
        return EXIT_FAILURE;
    }

    reverse_dir(argv[1], ".", dir_info.st_mode);

    return EXIT_SUCCESS;
}
