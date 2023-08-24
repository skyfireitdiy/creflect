#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>

using orig_execve_type = int (*)(const char* filename, char* const argv[], char* const envp[]);

int execve(const char* filename, char* const argv[], char* const envp[])
{
    orig_execve_type orig_execve;
    orig_execve = reinterpret_cast<orig_execve_type>(dlsym(RTLD_NEXT, "execve"));

    if (orig_execve == nullptr) {

        std::cerr << "execve is nullptr" << std::endl;

        return -1;
    }

    int result = orig_execve(filename, argv, envp);

    return result;
}
