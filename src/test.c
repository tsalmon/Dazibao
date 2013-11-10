#include <stdio.h>
#include <stdlib.h>

int main(int argc , char * argv[]) {
  
  /* Write Binary */

  FILE *f = fopen("./toto.txt","wb");
  char buffer[8] = "bonjour\0";

  if(f) {
    if(fwrite(buffer,sizeof(buffer),1,f)) {
      printf("Succès de l'écriture\n");
    } else {
      printf("Echec de l'écriture\n");
    }
  } else {
    printf("Erreur du fopen\n");
  }
  fclose(f);

  /* Read Binary */

  return 0;

  FILE *f2 = fopen("./toto.txt","rb");
  char *ptr = malloc(sizeof(char) * 100);
  
  if(fread(ptr,sizeof(char),100,f2)) {
    printf("Succès lors de la lecture\n");
  } else {
    printf("Echec lors de la lecture\n");
  }
  while(*ptr != '\0') {
    printf("%c",*ptr);
    ptr++;
  }
  fclose(f2);
  
  return(0);
}
