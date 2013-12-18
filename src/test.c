/*
 *Ce fichier sert a tester l'interface graphique en simulant le resultat d'un dazibao
 */
#include <gtk/gtk.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "vue.h"

void test(){  
  struct tlv *aux;
  int i = 0;
  
  daz = malloc(sizeof(dazibao));  
  daz->tlv_debut = malloc(sizeof(tlv));
  aux = daz->tlv_debut;
  for(i = 0; i < 10; i++){
    aux->suivant = malloc(sizeof(tlv));
    aux->type_id = 2;
    aux = aux->suivant;
  }
  aux->type_id = 2;
  
  //free(aux);
  /*
  //initialisation memoire
  daz = malloc(sizeof(dazibao));  
  daz->nb_tlv = 4;
  daz->tlv_debut = malloc(sizeof(tlv));
  //il y a maintenant 3 tlv
  
  //struct tlv *tlv = daz->tlv_debut; // 2 = text
  daz->tlv_debut->type_id = 2;
  daz->tlv_debut->position = 10;
  daz->tlv_debut->suivant = malloc(sizeof(tlv));
  daz->tlv_debut->suivant->type_id = 3;
  daz->tlv_debut->suivant->position = 20;
  daz->tlv_debut->suivant->suivant = malloc(sizeof(tlv));
  daz->tlv_debut->suivant->suivant->type_id = 6;
  daz->tlv_debut->suivant->suivant->position = 56;
  daz->tlv_debut->suivant->suivant->conteneur = malloc(sizeof(tlv)); 
  daz->tlv_debut->suivant->suivant->conteneur->type_id = 6; 
  daz->tlv_debut->suivant->suivant->conteneur->position = 200;
  daz->tlv_debut->suivant->suivant->conteneur->conteneur = malloc(sizeof(tlv));
  daz->tlv_debut->suivant->suivant->conteneur->conteneur->type_id = 2;
  daz->tlv_debut->suivant->suivant->suivant = malloc(sizeof(tlv));
  daz->tlv_debut->suivant->suivant->suivant->type_id = 5;
  daz->tlv_debut->suivant->suivant->suivant->nb_tlv = 2;
  daz->tlv_debut->suivant->suivant->suivant->conteneur = malloc(sizeof(tlv));
  daz->tlv_debut->suivant->suivant->suivant->conteneur->type_id = 2;
  daz->tlv_debut->suivant->suivant->suivant->conteneur->suivant = malloc(sizeof(tlv));
  daz->tlv_debut->suivant->suivant->suivant->conteneur->suivant->type_id = 2;
  */
}

void free_test(){
  //free(daz->tlv_debut->suivant->suivant);
  //free(daz->tlv_debut->suivant);
  //free(daz->tlv_debut);
  //free(daz);
}


int main(int argc, char *argv[]){
  srand(time(NULL));
  test();
  vue_init(argc, argv);
  free_test();
  return 0;
}
