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
  daz->tlv_tab = malloc(sizeof(tlv) * 3);
  //il y a maintenant 3 tlv
  
  struct tlv *tlv_t = daz->tlv_tab; // 2 = text
  tlv_t[0].type_id = 2;
  tlv_t[1].type_id = 2;
  tlv_t[2].type_id = 2;
  //on libre la memoire de la struct dazibao
  free(daz->tlv_tab);
  free(daz);
  
}

int main(int argc, char *argv[]){
  test();
  //init(argc, argv);
  return 0;
}
