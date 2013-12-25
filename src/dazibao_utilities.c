#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int get_file_size(char *path) {
    struct stat stats;
    if(stat(path, &stats) == -1) {
        perror("get_file_size");
        exit(58);
    }
    return stats.st_size;
}

void dazibao_write(int file_descriptor, void *buffer, size_t count) {
  if(write(file_descriptor, buffer, count) == -1) {
    perror("error while writing");
    exit(55);
  }
}

char *timestamp_to_date(int timestamp) {
    
    time_t time = timestamp;
    struct tm *tmp;
    
    char *output = malloc(sizeof(char) * 18);
    
    tmp = localtime(&time);
    strftime(output, 18, "%d/%m/%y", tmp);
    
    return output;
}
