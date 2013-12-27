#ifndef DAZIBAO_SAFE_H_
#define DAZIBAO_SAFE_H_

#include "dazibao.h"

void *safe_malloc(int n);

void *safe_realloc(void *p, int n);

void safe_free(void *p);

bool safe_read(int fd, void *buffer, size_t count);

bool safe_write(int fd, void *buffer, size_t count);

bool safe_flock(int fd, int operation);

/*
 * safe open
 */

#endif