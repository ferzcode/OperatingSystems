#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int global = 10;

int main(int argc, char ** argv) {
    int local = 5;

    //Создание и инициализация переменных
    printf("Local\t %i\t %p\n", local, &local);
    printf("Global\t %i\t %p\n\n", global, &global);

    // Вывод адресов и содержимого переменных
    pid_t pid = getpid();
    printf("Initial pid\t %i\n\n", pid);

    // Порождение нового процесса
    pid_t fork_pid = fork();
    if (fork_pid == -1) {
        perror("fork");
        return -1;
    }

    if (fork_pid == 0) { // если форк вернул ноль - это дочерний процесс
        //Вывод PID и PPID в дочернем процессе
        printf("%sIn child%s\n", "\e[0;94m", "\e[0m");
        pid = getpid();
        printf("Child pid\t %i\n", pid);
        pid_t parent_pid = getppid();
        printf("Parent pid\t %i\n\n", parent_pid);

        //Вывод адресов и содержимого переменных в дочернем процессе
        printf("Local\t %i\t %p\n", local, &local);
        printf("Global\t %i\t %p\n\n", global, &global);

        //Изменение содержимого переменных и вывод их значений
        local *= 10;
        global *= 10;
        printf("After change:\n");
        printf("Local\t %i\t %p\n", local, &local);
        printf("Global\t %i\t %p\n\n", global, &global);

        //Завершение дочернего процесса с кодом "5"
        sleep(10);
        printf("%sChild exiting...%s\n\n", "\e[0;91m", "\e[0m");
        _exit(5);
    } else {
        //Вывод содержимого переменных в родительском процессе
        sleep(30);
        printf("%sIn parent%s\n", "\e[0;94m", "\e[0m");
        printf("Local\t %i\t %p\n", local, &local);
        printf("Global\t %i\t %p\n\n", global, &global);

        //Задержка в родительском процессе
        sleep(30);
        int status;

        //Дожидается завершения дочернего процесса, считывает код завершения и выводит причину завершения и код
        wait(&status);
        if (WIFEXITED(status)) {
            printf("Terminated normally; exit status: %i\n", WEXITSTATUS(status));
        }
        if (WIFSIGNALED(status)) {
            printf("Terminated by signal; signal number: %i\n", WTERMSIG(status));
        }
    }
}
