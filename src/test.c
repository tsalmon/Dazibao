/*
 *Ce fichier sert a tester l'interface graphique en simulant le resultat d'un dazibao
 */
#include <gtk/gtk.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "vue.h"

int pos_tlv_selected;

/* RANDOM */

struct tlv *init_test_random(int n){    
  if(n <= 0){
    return NULL;
  }
  struct tlv * aux = malloc(sizeof(struct tlv));
  aux->type_id = 2 + rand() % 5;
  if(aux->type_id == 5){
    aux->conteneur = init_test_random(n);
  } else if(aux->type_id == 6){
    aux->conteneur = init_test_random(1);
  }
  
  aux->suivant = init_test_random(n-1);
  return aux;
}

void print_test_random(struct tlv *aux, int p){
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
      printf("PNG\n");
      break;
    case 4:
      printf("JPEG\n");
      break;
    case 5:
      printf("Rep\n");
      print_test_random(aux->conteneur, p+1);
      break;
    case 6:
      printf("Date\n");
      print_test_random(aux->conteneur, p + 1);
      break;
    default:
      printf("autre\n");
    }
    aux = aux->suivant;
  }
}

void free_test_random(struct tlv *aux){
  if(aux == NULL){
    return ;
  }
  free_test_random(aux->suivant);
  if(aux->type_id == 5 || aux->type_id == 6){
    free_test_random(aux->conteneur);
  }
  free(aux);
}

/* LISTE DATE*/
struct tlv *init_test_date(struct tlv *aux, int n){
  
}

int main(int argc, char *argv[]){
  srand(time(NULL));
  daz = malloc(sizeof(dazibao));  
  daz->tlv_debut = NULL;
  daz->tlv_debut = init_test_random(1 + rand() % 5);
  print_test_random(daz->tlv_debut, 0);
  vue_init();
  free_test_random(daz->tlv_debut);
  free(daz);
  return 0;
}
