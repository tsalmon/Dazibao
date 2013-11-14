#include <stdlib.h>
#include <stdio.h>
int main(){
  FILE *f = fopen("test", "wb");
  char x[2] = "a\0";
  fwrite(f, sizeo(x[0]), sizeof(x)/sizeof(x[0]), f);
}
