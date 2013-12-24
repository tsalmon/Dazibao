#ifndef VUE_H
#define VUE_H
#include <gtk/gtk.h>
#include "dazibao.h"

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
  struct tlv *pere;
  struct tlv *conteneur;
  struct tlv *suivant;
}tlv;

typedef struct dazi{
  struct tlv *tlv_debut;
  int nb_tlv;
}dazi;



struct tlv *init_test_random(int n);
void print_test(struct tlv *aux, int p);
void free_test(struct tlv *aux);

char *timestamp_to_date(int timestamp);
int nbDateBissextile(int d1, int d2);
int isBix(int date);

combo_data_st get_active_data (GtkComboBox * p_combo);
gint vue_gere_menu(GtkWidget *label, GdkEvent *event, gpointer message);
gint vue_gere_tlv(GtkWidget *tlv_btn, GdkEvent *event, gpointer message);
void addDateTLV(GtkWidget *widget, GdkEvent *event, gpointer message);
void vue_fen_view_Text(char * str);
void vue_fen_view_Image(void *raw_image, int type);
gint traitement_quitter(GtkWidget *label, GdkEvent *event, gpointer message);
const char *label_button(Dazibao_TLV* current_tlv);

/* add TLV */
void vue_add_rep();
gint vue_add_Date(GtkWidget *label, GdkEvent *event, gpointer message);
gint vue_add_Text(GtkWidget *label, gpointer *message);

/* fenetre de saisie */
void vue_fen_make_Text();
void vue_view_rep(Dazibao_TLV *rep);
void vue_body_Date();
void vue_foot_Date();
void vue_fen_make_Date();
void vue_body_rep();
char *vue_fen_make_Image();
GtkWidget *CreateCheckBox (GtkWidget *box, char *szLabel);

/* creation de l'ui*/
void vue_init_foot(GtkWidget * panel);
void vue_init_body(GtkWidget * panel, Dazibao_TLV **tlv, int nb_tlv);
void vue_init_head(GtkWidget * panel);
int  vue_init();

/*
  Variables globales
*/

struct dazi *daz;
struct tlv *tlv_actuel;
int pos_tlv_selected;


GtkWidget* panel;
GtkWidget* body_panel;
GtkWidget* foot_panel;
GtkWidget* window;
GtkWidget* add_dates_saves;

#endif

