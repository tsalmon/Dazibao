#ifndef VUE_H
#define VUE_H
#include <gtk/gtk.h>

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
char *makeImage();

GtkWidget* panel;
GtkWidget* body_panel;
GtkWidget* window;
#endif

