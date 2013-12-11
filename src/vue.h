#ifndef VUE_H
#define VUE_H
#include <gtk/gtk.h>


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
  //int nb_tlv;
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

void test();
void free_test();


//struct dazibao *daz;
int nbDateBissextile(int d1, int d2);
int isBix(int date);

void addRepOk(GtkWidget *label, GdkEvent *event, gpointer message);
 combo_data_st get_active_data (GtkComboBox * p_combo);
gint newTLVDate(GtkWidget *label, GdkEvent *event, gpointer message);
gint traitement_bouton(GtkWidget *label, GdkEvent *event, gpointer message);
gint add(GtkWidget *label, GdkEvent *event, gpointer message);
void addDateTLV(GtkWidget *widget, GdkEvent *event, gpointer message);
void afficher_text(int position_tlv);
gint traitement_quitter(GtkWidget *label, GdkEvent *event, gpointer message);
gint traitement_tlv(GtkWidget *tlv_btn, GdkEvent *event, gpointer message);
void head_init(GtkWidget * panel);
const char *label_button(struct tlv* current_tlv);
void body_init(GtkWidget * panel, struct tlv *tlv_debut);
void foot_init(GtkWidget * panel);
int init();
// add TLV
void makeText();
void makeDate();
void makeRep();
char *makeImage();
GtkWidget *CreateCheckBox (GtkWidget *box, char *szLabel);

GtkWidget* panel;
GtkWidget* body_panel;
GtkWidget* foot_panel;
GtkWidget* window;

#endif

