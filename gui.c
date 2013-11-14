#include <gtk/gtk.h>

void on_button(GtkWidget *pButton, gpointer data){
  const gchar * sText = "click";
  GtkWidget * label = gtk_label_new(sText);
  
  GtkWidget *pTempEntry;
  GtkWidget *pTempLabel;
  GList *pList;
  
  /* Recuperation de la liste des elements que contient la GtkVBox */
  pList = gtk_container_get_children(GTK_CONTAINER((GtkWidget*)data));
  
  /* Le premier element est le Table */
  pTempEntry = GTK_WIDGET(pList->data);
  
  
  
  /* Cet element est le GtkLabel */
  pTempLabel = GTK_WIDGET(pList->data);
  
  /* Recuperation du texte contenu dans le GtkEntry */
  sText = gtk_entry_get_text(GTK_ENTRY(pTempEntry));
  printf("sText : %s\n",(char *)sText);
  /* Modification du texte contenu dans le GtkLabel */
  gtk_label_set_text(GTK_LABEL(pTempLabel), sText);
  
  /* Liberation de la memoire utilisee par la liste */
  g_list_free(pList);
  
  gtk_label_set_text(GTK_LABEL(label), sText);
}

void panneau(GtkWidget *frame, int row){
  GtkWidget * button = gtk_button_new_with_label("panneau");
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_button), frame);
  
  gtk_table_attach_defaults(GTK_TABLE(frame), button, 0, 1, 0, 1 );    
}

void ecran(GtkWidget * frame){
  GtkWidget * label = gtk_label_new("ecran");
  
  gtk_table_attach_defaults(GTK_TABLE(frame), label, 1, 5, 0, 1 );
}

void config(GtkWidget *window, GtkWidget * frame){
  gtk_table_set_row_spacings(GTK_TABLE(frame), 2);
  gtk_table_set_col_spacings(GTK_TABLE(frame), 2);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
  gtk_window_set_title(GTK_WINDOW(window), "GtkTable");
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);
}

int main( int argc, char *argv[])
{
  gtk_init(&argc, &argv);
  
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);;
  GtkWidget *frame = gtk_table_new(1, 5, TRUE);

  config(window, frame);
  panneau(frame, 0);
  ecran(frame);
  
  gtk_container_add(GTK_CONTAINER(window), frame);
  g_signal_connect_swapped(G_OBJECT(window), "destroy",
			   G_CALLBACK(gtk_main_quit), G_OBJECT(window));
  gtk_widget_show_all(window);
  gtk_main();
  return 0;
}
