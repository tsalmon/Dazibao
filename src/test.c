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
long int c = 0;
/* RANDOM */

struct tlv *init_test_random(int n){    
  if(n <= 0){
    return NULL;
  }
  struct tlv * aux = malloc(sizeof(struct tlv));
  aux->position = c++;
  aux->type_id = 2 + rand() % 5;
  if(aux->type_id == 5){
    aux->conteneur = init_test_random(n+1);
    aux->conteneur->pere = daz->tlv_debut;
    //printf("act: %ld -> %ld\n", tlv_actuel->position, aux->conteneur->position);
    //tlv_actuel = aux->conteneur;
  } else if(aux->type_id == 6){
    aux->conteneur = init_test_random(1);
  }
  aux->suivant = init_test_random(n-1);
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
      printf("PNG\n");
      break;
    case 4:
      printf("JPEG\n");
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

/* LISTE DATE*/
struct tlv *init_test_date(int n){
  struct tlv * aux = malloc(sizeof(struct tlv));
  if(n <= 0){
    aux->type_id = 3;
    aux->conteneur = NULL;
    aux->suivant = NULL;
    return aux;
  }
  aux->type_id = 6;
  aux->conteneur = init_test_date(n-1);
  aux->suivant = NULL;
  return aux;
}

int main(int argc, char *argv[]){
  srand(time(NULL));
  daz = malloc(sizeof(dazibao));  
  daz->tlv_debut = init_test_random(5);
  tlv_actuel = daz->tlv_debut;
  print_test(daz->tlv_debut, 0);
  vue_init();
  free_test(daz->tlv_debut);  free(daz);
  return 0;
}
