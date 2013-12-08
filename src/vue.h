#ifndef VUE_H
#define VUE_H
#include <gtk/gtk.h>

typedef struct {
  gint     index;
  gchar *  p_text;
} combo_data_st;

int nbDateBissextile(int d1, int d2);
int isBix(int date);

void addRepOk(GtkWidget *label, GdkEvent *event, gpointer message);
static combo_data_st get_active_data (GtkComboBox * p_combo);
gint newTLVDate(GtkWidget *label, GdkEvent *event, gpointer message);
gint traitement_bouton(GtkWidget *label, GdkEvent *event, gpointer message);
gint add(GtkWidget *label, GdkEvent *event, gpointer message);
void addDateTLV(GtkWidget *widget, GdkEvent *event, gpointer message);
void afficher_text(int position_tlv);
gint traitement_quitter(GtkWidget *label, GdkEvent *event, gpointer message);
gint traitement_tlv(GtkWidget *tlv_btn, GdkEvent *event, gpointer message);
void head_init(GtkWidget * panel);
const char *label_button(struct tlv* current_tlv);
void body_init(GtkWidget * panel, struct tlv *tlv_debut, int nb_tlv);
void foot_init(GtkWidget * panel);
int init(int argc, char* argv[]);
void makeText();
void makeDate();
void makeRep();
GtkWidget *CreateCheckBox (GtkWidget *box, char *szLabel);
char *makeImage();

GtkWidget* panel;
GtkWidget* body_panel;
GtkWidget* foot_panel;
GtkWidget* window;
int id_bouton[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

#endif

