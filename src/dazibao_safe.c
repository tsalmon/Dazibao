#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/file.h>

#include "dazibao.h"

void *safe_malloc(int n) {
    void *p = malloc(n);
    if ( p == 0 ) {
        perror("Malloc Error:");
        exit(EXIT_FAILURE);
    }
    return p;
}

void *safe_realloc(void *p, int n) {
    void *p_new;
    if ( p == 0 ) {
        p_new = safe_malloc(n);
    } else {
        p_new = realloc(p, n);
        if ( p_new == 0 ) {
            perror("Realloc Error:");
            exit(EXIT_FAILURE);
        }
    }
    return p_new;
}

void safe_free(void *p) {
    if (p) {
        free(p);
    }
}

bool safe_lseek(int fd, off_t offset, int whence) {
    if(lseek(fd, offset, whence) == -1) {
        perror("Lseek Error:");
        return false;
    }
    return true;
}

bool safe_read(int fd, void *buffer, size_t count) {
    if(read(fd, buffer, count) == -1) {
        perror("Read Error:");
        return false;
    }
    return true;
}

bool safe_write(int fd, void *buffer, size_t count) {
    if(write(fd, buffer, count) != (ssize_t)count) {
        perror("Write Error:");
        return false;
    }
    return true;
}

bool safe_flock(int fd, int operation) {
    if(flock(fd, operation) == -1) {
        perror("Flock Error:");
        return false;
    }
    return true;
}

