#define _POSIX_C_SOURCE 200112L

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "fm_functions.h"
#include "utility.h"

void fm_mkdir(const char* pathname, mode_t mode) {
    if (mkdir(pathname, mode) == -1) {
        perror("fm_mkdir failed");
        return;
    }
    chmod(pathname, mode);
}

void fm_ls(const char* pathname) {
    DIR* dir_stream = opendir(pathname);
    if (dir_stream == NULL) {
        perror("fm_ls failed");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir_stream)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir_stream);
}

void fm_rmdir_r(const char* pathname) {
    DIR* dir_stream = opendir(pathname);
    if (dir_stream == NULL) {
        perror("fm_rmdir_r failed");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir_stream)) != NULL) {
        if (is_invalid(entry->d_name)) {
            continue;
        }

        char* full_entry_name;
        if (pathname[strlen(pathname) - 1] == '/') {
            full_entry_name = concat(pathname, entry->d_name);
        } else {
            full_entry_name = join(pathname, entry->d_name, '/');
        }

        struct stat entry_info;
        stat(full_entry_name, &entry_info);

        if (S_ISDIR(entry_info.st_mode)) {
            fm_rmdir_r(full_entry_name);
        } else {
            unlink(full_entry_name);
        }

        free(full_entry_name);
    }

    closedir(dir_stream);
    if (rmdir(pathname) == -1) {
        perror("fm_rmdir_r failed");
    }
}

void fm_create(const char* filename, mode_t mode) {
    if (open(filename, O_RDONLY | O_CREAT | O_EXCL, mode) == -1) {
        perror("fm_create failed");
        return;
    }
    chmod(filename, mode);
}

void fm_cat(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("fm_cat failed");
        return;
    }
    print_file(file);
    fclose(file);
}

void fm_unlink(const char* filename) {
    if (unlink(filename) == -1) {
        perror("fm_rm failed");
    }
}

void fm_symlink(const char* target, const char* link) {
    if (symlink(target, link) == -1) {
        perror("fm_symlink failed");
    }
}

void fm_readlink(const char* link) {
    enum {BUFF_SIZE = 200};
    char buffer[BUFF_SIZE];
    int bytes_placed = readlink(link, buffer, sizeof(buffer) - 1);

    if (bytes_placed == -1) {
        perror("fm_readlink failed");
        return;
    }
    buffer[bytes_placed] = 0;
    printf("%s\n", buffer);
}

void fm_link(const char* oldpath, const char* newpath) {
    if (link(oldpath, newpath) == -1) {
        perror("fm_link failed");
    }
}

void fm_stat(const char* filename) {
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        perror("fm_stat failed");
        return;
    }

    char modebuf[12];
    strmode(file_stat.st_mode, modebuf);
    printf("%s%ld\n", modebuf, file_stat.st_nlink);
}

void fm_chmod(const char* pathname, mode_t mode) {
    if (chmod(pathname, mode) == -1) {
        perror("fm_chmod failed");
    }
}