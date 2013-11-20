/*
 *Ce fichier sert a tester l'interface graphique en simulant le resultat d'un dazibao
 */
#include <gtk/gtk.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "test.h"
#include "vue.h"

//struct dazibao *daz;

void test(){
  daz = malloc(sizeof(dazibao));  
  
  //initialisation memoire
  daz->nb_tlv = 3;
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
  daz->tlv_debut->suivant->suivant->conteneur->type_id = 2; 
  daz->tlv_debut->suivant->suivant->conteneur->position = 200;}

void free_test(){
  free(daz->tlv_debut->suivant->suivant);
  free(daz->tlv_debut->suivant);
  free(daz->tlv_debut);
  free(daz);
}

int main(int argc, char *argv[]){
  test();
  init(argc, argv);
  free_test();
  return 0;
}
