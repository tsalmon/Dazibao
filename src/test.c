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
  daz->tlv_debut->suivant = malloc(sizeof(tlv));
  daz->tlv_debut->suivant->type_id = 2;
  daz->tlv_debut->suivant->

  //on libre la memoire de la struct dazibao
  free(daz->tlv_debut);
  free(daz);
  
}

int main(int argc, char *argv[]){
  test();
  //init(argc, argv);
  return 0;
}
