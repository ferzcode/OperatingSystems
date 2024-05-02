gcc -shared -fPIC helloDynamic.c -o libhelloDynamic.so

gcc hello.c -o hello -L. -lhelloDynamic

