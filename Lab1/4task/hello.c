#include <stdio.h>
#include <dlfcn.h>

int main() {
    void *handle;
    void (*hello_from_dyn_runtime_lib)();
    char *error;

    handle = dlopen("./libdynruntime.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }

    dlerror();

    *(void **) (&hello_from_dyn_runtime_lib) = dlsym(handle, "hello_from_dynamic_lib");

    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        return 1;
    }

    hello_from_dyn_runtime_lib();

    dlclose(handle);
    return 0;
}
