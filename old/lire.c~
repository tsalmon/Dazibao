#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


int bandeau();
void Text(char **str);
int lecture();

int fd; // descripteur du dazibao

int bandeau(){
  char c;
  int i = 0, k = 0;
  while(k < 4 && ((i = read(fd, &c, 1)) > 0)){
    if(k == 0 && c != 53){
      return 0;
    }
    else if(k > 0 && c != 0){
      return 0;
    }
    k++;
  }
  return (i > 0 && k == 4);
}

/*
 * on prend une str, et on la remplit
 * l'instruction en commentaire ne marche pas, je ne suis pas satisfait de l'alternative que j'ai trouve
 * Note: on ne gere pas les eventuels erreurs (ex: si le texte est plus petit ou plus grand que ce qui est indique dans length)
 */
void Text(char **str){
  char buff, length[3]; 
  int i = 0, size = 0, k = 0;
  
  if((i = read(fd, &length, sizeof(length))) > 0){
    size = length[0]*256*256 + length[1]*256 + length[2];
  } else{ return ; }
  
  *str = malloc(sizeof(char) * size);
  //read(fd, *str, sizeof*str) * k);// => mot vide
  while(k < size && (i = read(fd, &buff, 1)) > 0){
    (*str)[k++] = buff;
  }
  free(*str);
}

int lecture(){
  int i = 0;
  char buff;
  char *text;
  while((i = read(fd, &buff, 1)) > 0){
    switch(buff){
    case 2: Text(&text); printf("%s\n", text); break;
    default:
      return 0;
    }
  }  
  free(text);
}

int main(int argc, char *argv[]){
  if(argc == 1){
    perror("no args");
    return 1;
  }
  
  fd = open(argv[1], O_RDONLY);

  if(!bandeau()){
    write(STDERR_FILENO, "bandeau invalide\n",17); 
    return 1;
  }
  
  lecture();
  
  return 0;
}
