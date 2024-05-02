#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h> // Для использования syscall()

void myWrite(int fd, const char *msg, size_t len) {
    syscall(SYS_write, fd, msg, len);
}

int main() {
    myWrite(STDOUT_FILENO, "Hello, World!\n", 14);

    return 0;
}
