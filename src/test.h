#ifndef TEST_H
#define TEST_H

typedef struct tlv{
  int type_id;
  char *str;
  float date;
  struct tlv *conteneur;
}tlv;

typedef struct dazibao{
  struct tlv *tlv_tab;
  int nb_tlv;
}dazibao;

struct dazibao *daz;

#endif
