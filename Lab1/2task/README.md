gcc -c helloStatic.c -o helloStatic.o

ar rcs helloStatic.a helloStatic.o // Создание статической библиотеки из объектного файла

gcc hello.c -L -l helloStatic.o -o hello.out
