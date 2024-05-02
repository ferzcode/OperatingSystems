#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileManagerFunc.h"
#include "Utility.h"

static char* get_arg_2(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Missing argument 2.\n");
        exit(-1);
    }
    return argv[2];
}

int main(int argc, char** argv) {
    char* prog_name = argv[0];
    prog_name = get_short_name(prog_name);
    if (argc < 2) {
        fprintf(stderr, "Missing argument 1.\n");
        return -1;
    }
    char* arg1 = argv[1];

    if      (strcmp(prog_name, "fm_mkdir") == 0)    fm_mkdir(arg1, DIR_MODE);
    else if (strcmp(prog_name, "fm_ls") == 0)       fm_ls(arg1);
    else if (strcmp(prog_name, "fm_rmdir_r") == 0)  fm_rmdir_r(arg1);
    else if (strcmp(prog_name, "fm_create") == 0)   fm_create(arg1, FILE_MODE);
    else if (strcmp(prog_name, "fm_cat") == 0)      fm_cat(arg1);
    else if (strcmp(prog_name, "fm_unlink") == 0)   fm_unlink(arg1);
    else if (strcmp(prog_name, "fm_readlink") == 0) fm_readlink(arg1);
    else if (strcmp(prog_name, "fm_stat") == 0)     fm_stat(arg1);

    else if (strcmp(prog_name, "fm_symlink") == 0) {
        char* arg2 = get_arg_2(argc, argv);
        fm_symlink(arg1, arg2);
    }

    else if (strcmp(prog_name, "fm_link") == 0) {
        char* arg2 = get_arg_2(argc, argv);
        fm_link(arg1, arg2);
    }

    else if (strcmp(prog_name, "fm_chmod") == 0) {
        char* arg2 = get_arg_2(argc, argv);
        mode_t mode = parse_mode(arg2);
        if (mode == -1) {
            fprintf(stderr, "Invalid mode.\n");
            return -1;
        }
        fm_chmod(arg1, mode);
    }

    else {
        fprintf(stderr, "Unknown command\n");
        return -1;
    }
}