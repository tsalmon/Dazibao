#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct signal_bouton{
  int id_bouton;
}signal_bouton;

int id_bouton[] = {0, 1, 2, 3, 4, 5, 6};

gint traitement_bouton(GtkWidget *label, GdkEvent *event, gpointer message)
{ 
  //printf("id_bouton = %d\n", ((signal_bouton *)message)->id_bouton);
  int * i =  (int *)message;
  printf("id bouton = %d\n", *i);
  return FALSE;
}

gint traitement_quitter(GtkWidget *label, GdkEvent *event, gpointer message)
{ 
  
  return FALSE;
}

void head_init(GtkWidget * panel){
  GtkWidget *ajouter;
  GtkWidget *contracter;
  GtkWidget * head_panel;

  //defintions
  head_panel	= gtk_hbox_new(FALSE,0);
  ajouter	= gtk_button_new_with_label("Ajouter");
  contracter	= gtk_button_new_with_label("Contracter");

  //ajout des boutons au panel head_panel
  gtk_box_pack_start(GTK_BOX(head_panel), ajouter, FALSE, FALSE, 0);  
  gtk_box_pack_start(GTK_BOX(head_panel), contracter, FALSE, FALSE, 0);  

  //ajout de head panel dans le panel principal
  gtk_table_attach_defaults(GTK_TABLE(panel), head_panel, 0, 1, 0, 1);

  //activation des signaux sur boutons
  gtk_signal_connect(GTK_OBJECT(ajouter), "clicked", 
		     (GtkSignalFunc)traitement_bouton, 
		     (gpointer)&(id_bouton[0]));
  gtk_signal_connect(GTK_OBJECT(contracter), "clicked", 
		     (GtkSignalFunc)traitement_bouton, 
		     (gpointer)&(id_bouton[1]));
}

void body_init(GtkWidget * panel, int n){
  GtkWidget* panel_tlv_all;
  GtkWidget* scrollbar;
  int i;
  int j;

  /* declaration des variables du corps */
  panel_tlv_all	= gtk_vbox_new(FALSE,0);      
  // on pose un scroll sur la liste des tlv
  scrollbar	= gtk_scrolled_window_new(NULL, NULL); 
  
  /* affichage des TLV dans la liste panel_tlv_all*/
  for(i = 0 ; i < n ; ++i){
    GtkWidget * panel_tlv;
    GtkWidget* scrollbar_date;
    GtkWidget* dates;
    GtkWidget* button_tlv;
    char texte[10];
    
    /* definitions des elements de la liste */
    //une serie de dates sous forme de box
    dates = gtk_vbox_new(FALSE,0);
    //le panel du tlv
    panel_tlv = gtk_table_new(1, 5, TRUE);  
    //bouton de tlv
    button_tlv = gtk_button_new_with_label("machin");
    //le scroll sur la liste de dates
    scrollbar_date = gtk_scrolled_window_new(NULL, NULL); 
    /* on pose un scroll sur le box des dates */
    gtk_scrolled_window_add_with_viewport
      (GTK_SCROLLED_WINDOW(scrollbar_date),
       dates);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar_date),
				   GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
 
    /* on ajoute les dates au box */
    for(j = 0; j < 5; j++){
      GtkWidget* label_date; 
      sprintf(texte, "date %d", j);
      label_date = gtk_label_new(texte);
      gtk_box_pack_start(GTK_BOX(dates), label_date, FALSE, FALSE, 5);   
    } 
    
    /* insertion */
    gtk_table_attach_defaults(GTK_TABLE(panel_tlv), scrollbar_date, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(panel_tlv), button_tlv, 1, 5, 0, 1);    
    gtk_box_pack_start(GTK_BOX(panel_tlv_all), panel_tlv, FALSE, FALSE, 5);
  }  
  
  //on configure le scroll
  gtk_scrolled_window_add_with_viewport
    (GTK_SCROLLED_WINDOW(scrollbar), panel_tlv_all);
  gtk_scrolled_window_set_policy
    (GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);  
  //on ajout la liste 'scrolle' dans le milieu de la page (8/10e de la fen)
  gtk_table_attach_defaults(GTK_TABLE(panel), scrollbar, 0, 1, 1, 9);
  
  GtkWidget *more;
  more = gtk_button_new_with_label("MOAR");
  gtk_box_pack_start(GTK_BOX(panel_tlv_all), more, FALSE, FALSE, 5);   
  gtk_signal_connect(GTK_OBJECT(more), "clicked", 
		     (GtkSignalFunc)traitement_bouton, 
		     (gpointer)&(id_bouton[2]));
}

void foot_init(GtkWidget * panel){
  GtkWidget *home;
  GtkWidget *back;
  GtkWidget *head_panel = gtk_hbox_new(FALSE,0);
  
  home = gtk_button_new_with_label("Barrack");
  gtk_box_pack_start(GTK_BOX(head_panel), home, FALSE, FALSE, 0);  
  
  back = gtk_button_new_with_label("Back");
  gtk_box_pack_start(GTK_BOX(head_panel), back, FALSE, FALSE, 0);  
  
  gtk_table_attach_defaults(GTK_TABLE(panel), head_panel, 0, 1, 9, 10);
  
  gtk_signal_connect(GTK_OBJECT(home), "clicked", 
		     (GtkSignalFunc)traitement_bouton, 
		     (gpointer)&(id_bouton[3]));
  gtk_signal_connect(GTK_OBJECT(back), "clicked", 
		     (GtkSignalFunc)traitement_bouton, 
		     (gpointer)&(id_bouton[4]));
}
int main(int argc, char* argv[])
{
  if(argc != 2){
    return 1;
  }

  GtkWidget* window;
  GtkWidget* panel;
  int n = atoi( argv[1]);
  
  /* config */
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), 420, 600);
  gtk_window_set_title(GTK_WINDOW(window), "Dazibao");
  g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(traitement_quitter),0);
  
  panel = gtk_table_new(10, 1, TRUE);  
  gtk_container_add(GTK_CONTAINER(window),panel);
  
  //signal_btn = malloc(sizeof(signal_bouton) * 6);
  
  head_init(panel);  
  body_init(panel, n);
  foot_init(panel);
  
  gtk_widget_show_all(window);
  gtk_main();
  return 0;
} 
