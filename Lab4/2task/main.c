#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
    pid_t pid = getpid();
    printf("%i\n", pid);
    sleep(1);
    if (execv(argv[0], argv) == -1) {
        perror("execve");
        return -1;
    }
    printf("hello world\n");
}