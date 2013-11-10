#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
int main(){
  FILE *f = fopen("test", "rb");
  char c;
  while((c = fgetc(f))!= 0){
    printf("%d", c);
  }
}
