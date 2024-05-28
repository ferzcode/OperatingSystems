#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    printf("Real UID: %i\n", getuid());
    printf("Effective UID: %i\n", geteuid());

    FILE *file = fopen("file", "r");
    if (file == NULL)
        handle_error("fopen");

    char buffer[10];
    fread(buffer, 1, 9, file);
    buffer[9] = 0;
    printf("%s\n", buffer);

    fclose(file);
}
