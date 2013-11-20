#ifndef TEST_H
#define TEST_H

typedef struct tlv{
  int type_id;
  long int position;
  struct tlv *conteneur;
  struct tlv *suivant;
}tlv;

typedef struct dazibao{
  struct tlv *tlv_debut;
  int nb_tlv;
}dazibao;

struct dazibao *daz;

#endif
