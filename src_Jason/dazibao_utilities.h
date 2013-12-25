#ifndef DAZIBAO_UTILITIES_H_
#define DAZIBAO_UTILITIES_H_

int get_file_size(char *path);

void dazibao_write(int file_descriptor, void *buffer, size_t count);

char *timestamp_to_date(int timestamp);

#endif