gcc -shared helloRunDyn.c -o helloRunDyn.o -fPIC

gcc hello.c -o hello.out -ldl