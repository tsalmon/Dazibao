#ifndef VUE_H
#define VUE_H
#include <gtk/gtk.h>
gint traitement_bouton(GtkWidget *label, GdkEvent *event, gpointer message);
gint traitement_quitter(GtkWidget *label, GdkEvent *event, gpointer message);
void head_init(GtkWidget * panel);
void body_init(GtkWidget * panel, int n);
void foot_init(GtkWidget * panel);
int init(int argc, char* argv[]);

#endif

