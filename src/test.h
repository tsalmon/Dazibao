#ifndef TEST_H
#define TEST_H

typedef struct {
  gint     index;
  gchar *  p_text;
} combo_data_st;

typedef struct value{
  int date;
  char *str;
}value;


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
int id_bouton[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

void test();
void free_test();
#endif
