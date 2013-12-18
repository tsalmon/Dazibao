#include <stdlib.h>
#include <stdio.h>

typedef struct tlv{
  int type_id;
  int nb_tlv;
  long int position;
  struct value *tlv_val;
  struct tlv *conteneur;
  struct tlv *suivant;
}tlv;

typedef struct dazibao{
  struct tlv *tlv_debut;
  int nb_tlv;
}dazibao;

struct dazibao *daz;
struct tlv *tlv_actuel;
int pos_tlv_selected;

struct tlv *init_test(int n){    
  if(n <= 0){
    return NULL;
  }
  int i = 0, j = 0;
  struct tlv * aux = malloc(sizeof(struct tlv));
  aux->type_id = 2 + rand() % 5;
  if(aux->type_id == 6){
    aux->conteneur = init_test(n-1);
  } else if(aux->type_id == 5){
    aux->conteneur = init_test(n);
  }

  aux->suivant = init_test(n-1);
  return aux;
}

void print_test(struct tlv *aux, int p){
  int i = 0;
  for(; aux != NULL;){
    for(i = 0; i < p; i++){
      printf(" ");
    }
    switch(aux->type_id){
    case 2:
      printf("Text\n");
      break;
    case 3:
      printf("JPG\n");
      break;
    case 4:
      printf("PNG\n");
      break;
    case 5:
      printf("Rep\n");
      print_test(aux->conteneur, p+1);
      break;
    case 6:
      printf("Date\n");
      print_test(aux->conteneur, p + 1);
      break;
    default:
      printf("autre\n");
    }
    aux = aux->suivant;
  }
}

void free_test(struct tlv *aux){
  if(aux == NULL){
    return ;
  }
  free_test(aux->suivant);
  if(aux->type_id == 5 || aux->type_id == 6){
    free_test(aux->conteneur);
  }
  free(aux);
}

int main(int argc, char *argv[]){
  srand(time(NULL));
  daz = malloc(sizeof(dazibao));  
  daz->tlv_debut = NULL;
  daz->tlv_debut = init_test(1 + rand() % 5);
  print_test(daz->tlv_debut, 0);
  free_test(daz->tlv_debut);
  free(daz);
  return 0;
}
