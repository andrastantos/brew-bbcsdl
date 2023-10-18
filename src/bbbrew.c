// This file contains wrappers and stubs for functions that are not (yet) supported on Brew and by NewLib
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>

int usleep(unsigned long usec) { errno = EINVAL; return -1; }

char *realpath(const char *restrict path, char *restrict resolved_path)
{
    if (resolved_path == NULL)
        resolved_path = malloc(strlen(path)+1);
    strcpy(path, resolved_path);
    return resolved_path;
}

unsigned int sleep(unsigned int seconds) { return seconds; }

int chdir(const char *path) { errno = EINVAL; return -1; }

char *getcwd(char *buf, size_t size) { buf[0] = 0; return buf; }
