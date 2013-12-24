#include <gtk/gtk.h>  
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "vue.h"
#include "dazibao.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

/*
  Conventions de nommage des fonctions de ce fichier:
  add: on envoit des donnees saisie par l'utilisateur a l'ecrivain du dazibao
  fen: on produit une sous fenetre
  make: saisir des données
  view: lire une/des données
  body: on modifie corps de la fenetre
  init: initialisation de la page
  gere: gestion des boutons
  Toutes les fonctions commencent par "vue_" afin de préciser de quel fichier elles proviennent  
  vue_fen_make_Text() : produire une sous fenetre qui permet de saisir du texte 
*/

int id_bouton[] = {0, 1, 2, 3, 4, 5, 6, 7, 8 };


char *timestamp_to_date(int timestamp) {
  time_t time = timestamp; 
  struct tm *tmp; 
  char *output = malloc(sizeof(char) * 18);
  tmp = localtime(&time);
  strftime(output, 18, "%d/%m/%y %Hh%Mm%Ss", tmp); 
  return output; 
}

/*
  call by: vue_init_head, vue_init_body, vue_init_foot
  Cette fonction gere les boutons d'ajout de TLV (quand on clique sur les boutons du bas de l'ui),
  ainsi que les boutons du haut de l'ui (Home, back & compacter)
*/
gint vue_gere_menu(GtkWidget *label, GdkEvent *event, gpointer message){
  int * i =  (int *)message;
  switch(*i){
  case 0: /*Add: texte*/
    vue_fen_make_Text();
    break;
  case 1: /*Add: image*/
    vue_fen_make_Image();
    break;
  case 2: /*Add: date*/
    addDateTLV(label, event, message);
    break;
  case 3: /*Add: Repertoire */
    vue_add_rep();
    gtk_widget_show_all(window);
    break;
  case 4: 
    tlv_actuel = daz->tlv_debut;
    gtk_widget_destroy(body_panel);
    /*vue_init_body(panel, tlv_actuel);*/
    gtk_widget_show_all(window);
    break;
  case 5: 
    /*
    printf("back: ");
    if(tlv_actuel == daz->tlv_debut){
      printf("rien a back\n");
    } else {
      printf("%ld -> %ld\n", tlv_actuel->position, tlv_actuel->pere->position);
      gtk_widget_destroy(body_panel);
      tlv_actuel = tlv_actuel->pere;
      vue_init_body(panel, tlv_actuel);
      gtk_widget_show_all(window);
    }
    */
    break;
  case 6:
    break;
  default:
    printf("non reconnu");
  }
  return FALSE;
}

/*
  call_by: traitment_tlv
  remplacer le contenu du corps de l'ui par les TLV du rep sur lequel
  l'utilisateur a cliqué
*/
void vue_view_rep(Dazibao_TLV* tlv){
  /*printf("vue_view_rep\n");*/
  /*printf("passage repertoire: tlv_act = %ld -> %ld\n", tlv_actuel->position, rep->conteneur->position);*/
  Dazibao_TLV_Compound_Value *rep = tlv->value;
  vue_init_body(panel, rep->elements, rep->count);
  gtk_widget_show_all(window);  
}


/*
  (fonction obsolete, non utilise)
  Utiliser pour l'affichage qui permet d'ajouter une tlv de type conteneur
*/
GtkWidget *CreateCheckBox (GtkWidget *box, char *szLabel){
  GtkWidget *check;
  check = gtk_check_button_new_with_label (szLabel);
  gtk_box_pack_start (GTK_BOX (box), check, FALSE, FALSE, 10);
  gtk_widget_show (check);
  return (check);
}

/*
  call by: vue_body_rep
  Apres avoir cliquer sur le bouton Content, puis sur le bouton ok, 
  cette fonction permet de savoir quelles sont les tlv qui ont été selectionnées
  par l'utilisateur (ie. celles qu'il a coché)
*/
void vue_add_rep(){
  printf("vue_add_rep\n");
}


/*
  OBSOLETE
  Call by: vue_gere_menu
  Apres avoir clique sur le bouton Content: 
  On va remplacer toutes les cases de dates des TLV par une case a cocher afin 
  de permettre a l'utilisateur de selectionner une tlv pour l'ajouter a un 
  conteneur
*/
void vue_body_rep(){
  struct tlv * curseur;
  GList *children = gtk_container_get_children(GTK_CONTAINER(body_panel));
  int size = g_list_length(children);
  GtkWidget *aux;
  GtkWidget *aux_table;
  guint i = 0;
  GtkWidget *btn_ok = gtk_button_new_with_label("Ok");
  GtkWidget *btn_cancel = gtk_button_new_with_label("Cancel");
  
  if(tlv_actuel->type_id > 4){
    curseur = tlv_actuel->conteneur;
  } else {
    curseur = tlv_actuel;  
  }
  
  for(i = 0; i < size; i++){ /* on parcours le body_panel*/
    aux = (GtkWidget *) g_list_nth_data (children, i);
    if(GTK_IS_TABLE(aux)){ /* cette condition est la pour eviter de capturer le bouton More*/
      GList *children_table = gtk_container_get_children(GTK_CONTAINER(aux));
      aux_table = (GtkWidget *) g_list_nth_data (children_table, 1);    
      if(GTK_IS_SCROLLED_WINDOW(aux_table)){/* on ne capture que le scroll du date*/
	GtkWidget *check;
	gtk_widget_destroy(aux_table);
	check = gtk_check_button_new();
	gtk_table_attach_defaults(GTK_TABLE(aux), check, 0, 1, 0, 1);	
      }
      curseur = curseur->suivant;    
    }
  }
  /* FOOT SESSION*/
  gtk_widget_destroy(foot_panel); /* on vire tous les boutons  */
  foot_panel	= gtk_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(foot_panel), btn_ok , FALSE, FALSE, 0);  
  gtk_box_pack_start(GTK_BOX(foot_panel), btn_cancel , FALSE, FALSE, 0);  
  gtk_signal_connect(GTK_OBJECT(btn_cancel), "clicked", 
		     (GtkSignalFunc)vue_gere_menu, 
		     (gpointer)&id_bouton[5]);
  gtk_signal_connect(GTK_OBJECT(btn_ok), "clicked", 
		     (GtkSignalFunc)vue_add_rep, 
		     (gpointer)body_panel);

  gtk_table_attach_defaults(GTK_TABLE(panel), foot_panel, 0, 1, 9, 10);
  gtk_widget_show_all(foot_panel);  
}

/*
  Callby: newTLVDate
  permet de compter le nombres de dates bissextiles entre 2 années
  (je n'ai pas trouvé d'autre moyen)
*/
int nbDateBissextile(int annee_1, int annee_2){
  int i = 0, j = 0;
  for(j = annee_1; j < annee_2; j++){
    if (isBix(j))
      i++;
  }
  return i;
}

/*
  call by: nbDateBissextile
*/
int isBix(int annee){
  if(annee % 400 == 0)
    return 1;
  if(annee % 100 == 0)
    return 0;
  return annee % 4 == 0;
} 

/*
  call by: addDateTLV
  recupère les valeurs et calcul le timestamp associé
*/
gint vue_add_Date(GtkWidget *label, GdkEvent *event, gpointer message){
  GtkWidget **data = (GtkWidget **)message;
  GtkComboBox * p_combo  = (GtkComboBox * )data[1];
  combo_data_st p_st;
  int aux = 0, mois[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
  long unsigned int timestamp = 0;
  
  aux		= atoi(gtk_entry_get_text(GTK_ENTRY(data[0])));
  timestamp	= (aux - 1970) * 31536000 + nbDateBissextile(1970, aux) * 86400 - 3600;
  
  p_st = get_active_data (p_combo);  
  timestamp += mois[p_st.index - 1] * 86400;
 
  if(((aux % 4 == 0) && (aux % 100 != 0)) || (aux % 400 == 0))
    timestamp += 86400 ;

  p_st = get_active_data ((GtkComboBox * )data[2]);
  timestamp += (p_st.index - 1) * 86400;

  aux = atoi(gtk_entry_get_text(GTK_ENTRY(data[5])));
  timestamp += 3600 * aux;

  aux = atoi(gtk_entry_get_text(GTK_ENTRY(data[4])));
  timestamp += 60 * aux;

  aux = atoi(gtk_entry_get_text(GTK_ENTRY(data[3])));
  timestamp += aux;
  
  printf("timestamp a produire = %lu\n", timestamp);
  return FALSE;
}

/* 
   call_by: vue_add_date
   Fonction qui recupere les donnees de l'element courant affiche  
*/
combo_data_st get_active_data (GtkComboBox * p_combo){
  GtkTreeModel   *  p_model = NULL;
  GtkTreeIter       iter;
  combo_data_st     p_st;
  
  /* On recupere le modele qu'on a cree. */
  p_model = gtk_combo_box_get_model (p_combo);
  /* On recupere le GtkTreeIter de l'element courant. */
  if (gtk_combo_box_get_active_iter (p_combo, & iter)){
    /*
     * On recupere les donnees de l'element courant a savoir
     * un entier et une chaine de caracteres.
     */
    gtk_tree_model_get (p_model, & iter, 0, & p_st.index, 1, & p_st.p_text, -1);
  } 
  return p_st;
}

/*
  call by: vue_fen_make_Text
  permet de recuperer le texte saisie par l'utilisateur afin de l'ajouter
  dans une TLV de type text
*/
gint vue_add_Text(GtkWidget *widget, gpointer *message){
  GtkWidget	*pTextView;
  GtkTextBuffer *pTextBuffer;
  GtkTextIter	iStart;
  GtkTextIter	iEnd;
  gchar		*sBuffer;

  pTextView = GTK_WIDGET(message);

  /* recupere le buffer */
  pTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(pTextView));
  /* recupere l'origine du buffer */
  gtk_text_buffer_get_start_iter(pTextBuffer, &iStart);
  /* recupere la fin du buffer */
  gtk_text_buffer_get_end_iter(pTextBuffer, &iEnd);

  /* copie le contenu du buffer dans une variable */
  sBuffer = gtk_text_buffer_get_text(pTextBuffer, &iStart, &iEnd, TRUE);
  printf("%s\n", sBuffer);
  /*
  GtkTextBuffer *buffer = message;
  GtkTextIter i1, it;
  int k, i;
  gchar *text;
  
  gtk_text_buffer_get_iter_at_mark
    (buffer, &it, gtk_text_buffer_get_insert(buffer));

  k = gtk_text_iter_get_line(&it);
  gtk_text_buffer_get_iter_at_line(buffer, &it, k);
  gtk_text_buffer_get_end_iter(buffer, &i1);
  text = gtk_text_buffer_get_text(buffer, &it, &i1, FALSE);
  for(i=0; text[i];i++){
  }
  text[i]=0;
  printf("%s\n", text);  
  */
  return FALSE;
}

/*
 */
gint traitement_addDateTLV(GtkWidget *btn_date, GdkEvent *event, gpointer message){
  return FALSE;
}

/*
  call by: vue_body_Date
  Permet de creer une sous-fenetre ou l'on permet de remplir 6 champs de dates:
  Année, mois, jour, seconde, minute et heure
  Par défaut, les valeurs sont la date au moment ou l'utilisateur a cliqué sur
  le bouton d'ajout
  Apres avoir sur le bouton de confirmation, on appel la fonction vue_add_date
*/
void addDateTLV(GtkWidget *panel_tlv, GdkEvent *event, gpointer message){

  struct tlv *getTLV = (struct tlv *) message;
  /*dans le cas ou la tlv est une date, on recupere le dernier*/
  GtkWidget* box;
  GtkWidget *scrollbar;
  GtkWidget *label[6];
  GtkWidget *btn_date;
  int i = 0;

  /*
  GList *children = gtk_container_get_children(GTK_CONTAINER(panel_tlv));
  int size = g_list_length(children);
  GtkWidget *aux;
  GtkWidget *aux_table;
  guint i = 0;

  printf("size = %d\n", size);
  for(i = 0; i < size; i++){ // on parcours le panel_tlv
    printf("i = %d\n", i);
    aux = (GtkWidget *) g_list_nth_data (children, i);
    if(GTK_IS_TABLE(aux)){ // cette condition est la pour eviter de capturer le bouton More
      printf("date\n");
      GList *children_table = gtk_container_get_children(GTK_CONTAINER(aux));
      aux_table = (GtkWidget *) g_list_nth_data (children_table, 1);    
      if(GTK_IS_SCROLLED_WINDOW(aux_table)){// on ne capture que le scroll du date
	
      }
    }
  
  }
*/
  
  if(getTLV->type_id == 6){
    GtkWidget* window_addDate;
    window_addDate = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position (GTK_WINDOW (window_addDate), GTK_WIN_POS_CENTER);
    
    gtk_window_set_default_size(GTK_WINDOW(window_addDate), 320, 200);
    gtk_window_set_title(GTK_WINDOW(window_addDate), "GtkScrolledWindow_AddDate");
    g_signal_connect(G_OBJECT(window_addDate),"destroy",G_CALLBACK(gtk_main_quit),0);
    
    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window_addDate),scrollbar);
    
    box=gtk_vbox_new(FALSE,5);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar), box);
    
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    
    btn_date = gtk_button_new_with_label("ajouter une nouvelle date");
    gtk_box_pack_start(GTK_BOX(box), btn_date, FALSE, FALSE, 5); /* 5 = espacement*/
    while(getTLV->type_id == 6){
      GtkWidget *label;
      char texte[10];
      sprintf(texte, "%d", i++);
      label = gtk_label_new(texte);
      gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 5); 
      btn_date = gtk_button_new_with_label("ajouter une nouvelle date");
      gtk_box_pack_start(GTK_BOX(box), btn_date, FALSE, FALSE, 5);
      getTLV = getTLV->conteneur;
    }
    gtk_widget_show_all (window_addDate);
    gtk_main ();
  } else {
    GtkWidget * p_win  = NULL;
    GtkWidget * p_combo[6];
    time_t secondes;
    GtkWidget *table = gtk_table_new(7, 2, TRUE);
    char setText[4]; 
    GtkWidget *button;
    int i = 0, j = 0;
    struct tm instant;
    
    gtk_init (NULL, NULL); 
    p_win = gtk_window_new           (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title             (GTK_WINDOW (p_win), "GtkComboBox...");
    gtk_window_set_default_size      (GTK_WINDOW (p_win), 250, 150);
    gtk_container_set_border_width   (GTK_CONTAINER (p_win), 5);
    gtk_window_set_position          (GTK_WINDOW (p_win), GTK_WIN_POS_CENTER);
    
    label[0] = gtk_label_new("Annee");
    label[1] = gtk_label_new("Mois");
    label[2] = gtk_label_new("Jour");
    label[3] = gtk_label_new("Seconde");
    label[4] = gtk_label_new("Minute");
    label[5] = gtk_label_new("Heure");
    
    button = gtk_button_new_with_label("OK");
    
    time(&secondes);
    instant=*localtime(&secondes);      
     
    p_combo[0] = gtk_entry_new();
    /* mois et annee*/
    for(j = 1; j < 3; j++){
      GtkListStore      *  p_model  = NULL;
      GtkCellRenderer   *  p_cell   = NULL;
      GtkTreeIter iter;
      p_model = gtk_list_store_new (2, G_TYPE_INT, G_TYPE_STRING);
      p_combo[j] = gtk_combo_box_new_with_model (GTK_TREE_MODEL (p_model));
      p_cell = gtk_cell_renderer_text_new ();
      gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (p_combo[j]), p_cell, FALSE);
      gtk_cell_layout_set_attributes
	(GTK_CELL_LAYOUT (p_combo[j]), p_cell, "text", 0, NULL );
      p_cell = gtk_cell_renderer_text_new ();
      gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (p_combo[j]), p_cell, FALSE);
      gtk_cell_layout_set_attributes 
	( GTK_CELL_LAYOUT (p_combo[j]), p_cell, "text", 1, NULL); 
      gtk_combo_box_set_active (GTK_COMBO_BOX (p_combo[j]), 2);
      if(j == 1){
	gtk_list_store_append (p_model, & iter);
	gtk_list_store_set (p_model, & iter, 0, 1, 1, " Janvier", -1);
	gtk_list_store_append (p_model, & iter);
	gtk_list_store_set (p_model, & iter, 0, 2, 1, " Fevrier", -1);
	gtk_list_store_append (p_model, & iter);
	gtk_list_store_set (p_model, & iter, 0, 3, 1, " Mars", -1);
	gtk_list_store_append (p_model, & iter);
	gtk_list_store_set (p_model, & iter, 0, 4, 1, " Avril", -1);
	gtk_list_store_append (p_model, & iter);
	gtk_list_store_set (p_model, & iter, 0, 5, 1, " Mai", -1);
	gtk_list_store_append (p_model, & iter);
	gtk_list_store_set (p_model, & iter, 0, 6, 1, " Juin", -1);
	gtk_list_store_append (p_model, & iter);
	gtk_list_store_set (p_model, & iter, 0, 7, 1, " Juillet", -1);
	gtk_list_store_append (p_model, & iter);
	gtk_list_store_set (p_model, & iter, 0, 8, 1, " Aout", -1);
	gtk_list_store_append (p_model, & iter);
	gtk_list_store_set (p_model, & iter, 0, 9, 1, " Septembre", -1);
	gtk_list_store_append (p_model, & iter);
	gtk_list_store_set (p_model, & iter, 0, 10, 1, " Octobre", -1);
	gtk_list_store_append (p_model, & iter);
	gtk_list_store_set (p_model, & iter, 0, 11, 1, " Novembre", -1); 
	gtk_list_store_append (p_model, & iter);
	gtk_list_store_set (p_model, & iter, 0, 12, 1, " Decembre", -1);
      } else {
 	for(i = 1; i < 32; i++){
	  gtk_list_store_append (p_model, & iter);	  
	  gtk_list_store_set (p_model, & iter, 0, i, 1, NULL, -1);
	}
      }
    }
    for( i = 3; i < 6; i++){
      p_combo[i] = gtk_entry_new_with_max_length(2);
    }
    
    sprintf(setText, "%d", instant.tm_year + 1900);
    gtk_entry_set_text(GTK_ENTRY(p_combo[0]), (gchar *)setText);

    gtk_combo_box_set_active (GTK_COMBO_BOX (p_combo[1]), instant.tm_mon);
    gtk_combo_box_set_active (GTK_COMBO_BOX (p_combo[2]), instant.tm_mday - 1);     

    sprintf(setText, "%d", instant.tm_sec);
    gtk_entry_set_text(GTK_ENTRY(p_combo[3]), (gchar *)setText);
    sprintf(setText, "%d", instant.tm_min);
    gtk_entry_set_text(GTK_ENTRY(p_combo[4]), (gchar *)setText);
    sprintf(setText, "%d", instant.tm_hour);
    gtk_entry_set_text(GTK_ENTRY(p_combo[5]), (gchar *)setText);
    gtk_container_add(GTK_CONTAINER(p_win), table);
    
    for( i=0; i < 6; i++) {
      gtk_table_attach_defaults(GTK_TABLE(table), label[i], 0, 1, i, i+1 );  
      gtk_table_attach_defaults(GTK_TABLE(table), p_combo[i], 1, 2, i, i+1 );  
    }
    gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 1, 6, 7 );  
    g_signal_connect (G_OBJECT (p_win), "destroy", G_CALLBACK (gtk_main_quit), NULL);

    gtk_signal_connect(GTK_OBJECT(button), "clicked", 
		       G_CALLBACK(vue_add_Date), 
		       (gpointer)p_combo);
  gtk_widget_show_all (p_win);
  gtk_main ();
  }
}

/*
  OBSOLETE
  call by: vue_gere_menus
  on remplace les dates par un bouton d'ajout de dates 
  Si on clique dessus on appel la fonction addDateTLV qui permet d'entrer une
  nouvelle date
*/
void vue_body_Date(){
  struct tlv * curseur;
  GList *children = gtk_container_get_children(GTK_CONTAINER(body_panel));
  int size = g_list_length(children);
  GtkWidget *aux;
  GtkWidget *aux_table;
  guint i = 0;
  
  if(tlv_actuel->type_id > 4){
    curseur = tlv_actuel->conteneur;
  } else {
    curseur = tlv_actuel;  
  }
  for(i = 0; i < size; i++){ 
    aux = (GtkWidget *) g_list_nth_data (children, i);
    if(GTK_IS_TABLE(aux)){ 
      GList *children_table = gtk_container_get_children(GTK_CONTAINER(aux));
      aux_table = (GtkWidget *) g_list_nth_data (children_table, 1);    
      if(GTK_IS_SCROLLED_WINDOW(aux_table)){/* on ne capture que le scroll du date*/
	/*
	  if(GTK_IS_VBOX(children_scrolled_window)) {
	  printf("scroll\n");
	  } else if(GTK_IS_VBOX(aux_scrolled_window)){
	  printf("vbox\n");
	  } else if(GTK_IS_LABEL(aux_scrolled_window)) {
	  printf("label\n");
	  } else if(GTK_IS_BUTTON(aux_scrolled_window)) {
	  printf ("%s\n",);("button\n");
	  } else else if(GTK_IS_TABLE(aux_scrolled_window)) {
	  printf("table\n");
	  }
	*/
	
	gtk_widget_destroy(aux_table);
	aux_table = gtk_button_new_with_label("+");
	gtk_table_attach_defaults(GTK_TABLE(aux), aux_table, 0, 1, 0, 1);
	gtk_signal_connect(GTK_OBJECT(aux_table), "clicked", 
			   (GtkSignalFunc)addDateTLV, 
			   (gpointer)(curseur));
      }
      curseur = curseur->suivant;    
    }
  }
}

/*
  call by: vue_gere_menus
  on remplace le foot par 2 boutons qui permettent de confirmer ou d'annuler l'ajout d'une date
*/
void vue_foot_Date(){
  GtkWidget *btn_cancel = gtk_button_new_with_label("Cancel");
  gtk_widget_destroy(foot_panel);
  foot_panel	= gtk_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(foot_panel), btn_cancel , FALSE, FALSE, 0);  
  gtk_signal_connect(GTK_OBJECT(btn_cancel), "clicked", 
		     (GtkSignalFunc)vue_gere_menu, 
		     (gpointer)&id_bouton[5]);
  gtk_table_attach_defaults(GTK_TABLE(panel), foot_panel, 0, 1, 9, 10);
  gtk_widget_show_all(foot_panel);
}

/*
  call_by: vue_gere_menu
  Permet de selectionner une image a partir d'une fenetre de parcours de 
  fichiers
*/
char* vue_fen_make_Image (){
  GtkWidget *dialog;
  GtkWidget *windowImage;  
  gint dialog_respond = 0;
  windowImage = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  dialog = gtk_file_chooser_dialog_new 
    ("Save File",
     GTK_WINDOW(windowImage),
     GTK_FILE_CHOOSER_ACTION_OPEN,
     GTK_STOCK_CANCEL,
     GTK_RESPONSE_CANCEL,
     GTK_STOCK_OPEN,
     GTK_RESPONSE_ACCEPT,
     NULL);
  gtk_file_chooser_set_do_overwrite_confirmation 
    (GTK_FILE_CHOOSER (dialog), TRUE);
  
  dialog_respond = gtk_dialog_run (GTK_DIALOG (dialog));
  if (dialog_respond == GTK_RESPONSE_ACCEPT) {
    char *filename;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    printf("%s\n", filename);
    gtk_widget_destroy (dialog);
    return filename;
  } else if(dialog_respond == -6){
    gtk_widget_destroy (dialog);
    return NULL;
  } else {
    return NULL;
  }
}

/*
  call by: vue_gere_menu
  Crée une fenetre qui contient un espace pour écrire du texte et un bouton
  pour l'envoyer, si l'utilisateur clique sur le bouton, alors on appel la
  fonction vue_add_Text
*/
void vue_fen_make_Text(){
  GtkWidget *window, *view, *vbox;
  GtkWidget *ok, *text_panel;
  GtkTextBuffer *buffer;
  GtkWidget* scrollbar;
  ok = gtk_button_new_with_label("ok");
  scrollbar	= gtk_scrolled_window_new(NULL, NULL); 
  /* begin config */
  gtk_init(NULL, NULL);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 250, 200);
  gtk_window_set_title(GTK_WINDOW(window), "TextView");
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);
  GTK_WINDOW(window)->allow_shrink = TRUE;
  /* define var */
  vbox = gtk_vbox_new(FALSE, 0);
  text_panel = gtk_vbox_new(FALSE, 0);
  view = gtk_text_view_new();
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
  gtk_box_pack_start(GTK_BOX(vbox), view, TRUE, TRUE, 0);  
  gtk_scrolled_window_add_with_viewport
    (GTK_SCROLLED_WINDOW(scrollbar), vbox);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  gtk_box_pack_start(GTK_BOX(text_panel), ok, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(text_panel), scrollbar, TRUE, TRUE, 0);
  /* config text*/
  gtk_text_buffer_create_tag(buffer, "gap","pixels_above_lines", 30, NULL);
  gtk_text_buffer_create_tag(buffer, "lmarg", "left_margin", 5, NULL);
  gtk_text_buffer_create_tag(buffer, "blue_fg", "foreground", "blue", NULL); 
  gtk_text_buffer_create_tag(buffer, "gray_bg", "background", "gray", NULL); 
  gtk_text_buffer_create_tag(buffer, "italic", "style", PANGO_STYLE_ITALIC, NULL);
  gtk_text_buffer_create_tag(buffer, "bold", "weight", PANGO_WEIGHT_BOLD, NULL);
  gtk_container_add(GTK_CONTAINER(window), text_panel);
  g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), G_OBJECT(window));
  g_signal_connect(GTK_OBJECT(ok), "clicked", (GtkSignalFunc)vue_add_Text, (gpointer)(view));  
  gtk_widget_show_all(window);
  gtk_main();

}

/*
  Cette fonction ne devrait pas etre ici car ne concerne pas directement l'ui,
  je m'en sert seulement pour tester l'affichage de textes
 */
int insertSpace(char str[]){
  int cur = 0;
  int i = 0;
  int nb_lignes = 1;
  while(str[cur] != '\0'){
    if(str[cur] == '\n'){
      nb_lignes++;
      i = 0;
    }
    if(i == 80){
      while(str[cur--] != ' '){
      }
      str[cur] = '\n'; 
      nb_lignes++;
      cur++;
      i = 0;
    } else {
      i++;
      cur++;
    }
  } 
  return nb_lignes;
}

/*
  call_by: vue_gere_tlv
  permet de lire une tlv de type texte
  Le texte s'affiche sur des lignes de ~80 chars
*/
void vue_fen_view_Text(char *str){
  /*char str[1059] = "Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World!\n Hello World!\n Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! <> Hello World!< Hello World! >LOL Hello World! super Hello World!O\n fin\n\nHello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World!\n Hello World!\n Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! Hello World! <> Hello World!< Hello World! >LOL Hello World! super Hello World!O\n fin"; test basic*/
  int nb = 0;
  int i = 0, j = 0, k = 0;
  char *str_lines[80];

  GtkWidget* window = NULL;
  GtkWidget* box;
  GtkWidget** pLabel = NULL;  
  GtkWidget* scrollbar = NULL;
  
  
  nb = insertSpace(str);
  for(i = 0; i < nb; i++){
    str_lines[i]= malloc(sizeof(char) * 80);
    for(j = 0; j < 80; j++){
      str_lines[i][j] = 0;
    }
  }


  for(i = 0; i < nb; i++){
    for(j = 0; j < 80 && str[k] != '\n' && str[k] != '\0'; j++){
      str_lines[i][j] = str[k++];
    }    
    if(str[k] == '\n'){      
      k++;
    }
  }    
  
  pLabel = malloc(sizeof(GtkWidget *) * nb);

  gtk_init(NULL, NULL);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), 420, 200);
  gtk_window_set_title(GTK_WINDOW(window), "GtkScrolledWindow");
  g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),0);
 
  scrollbar = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(window),scrollbar);
  
  box=gtk_vbox_new(FALSE,5);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar), box);
  
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  
  for(i = 0; i < nb; i++){
    pLabel[i] = gtk_label_new(str_lines[i]);
    gtk_box_pack_start(GTK_BOX(box), pLabel[i], FALSE, FALSE, 5); 
  }
  
  gtk_widget_show_all(window); 
  gtk_main();

  /*
  for(i = 0; i < nb; i++){
    free(str_lines[i]);
  }
  free(str_lines);
  */

/*
  GtkWidget *fenetre;
  GtkWidget *text;
  GtkWidget *panel;
  GtkWidget * scrollbar;
  int i = 0;
  
  gtk_init(&i, NULL);
  fenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
  gtk_window_set_title (GTK_WINDOW(fenetre), "Hello World !");
  gtk_window_set_default_size(GTK_WINDOW(fenetre), 400, 400);
  g_signal_connect(G_OBJECT(fenetre),"destroy",G_CALLBACK(gtk_main_quit),0); 
  gtk_window_set_resizable(GTK_WINDOW(fenetre), FALSE);        
  
  scrollbar = gtk_scrolled_window_new(NULL, NULL);
  panel = gtk_hbox_new(FALSE, 5);  
  
  text = gtk_label_new ("This is an example of a line-wrapped, filled label. It should be taking up the entire width allocated to it. Here is a sentence to prove my point.  Here is another sentence. Here comes the sun, do de do de do.\n This is a new paragraph.\nThis is another newer, longer, better paragraph.  It is coming to an end, ufortunately.");
  
  gtk_container_add(GTK_CONTAINER(fenetre), scrollbar);
  gtk_scrolled_window_add_with_viewport
    (GTK_SCROLLED_WINDOW(scrollbar), panel);
  gtk_scrolled_window_set_policy
    (GTK_SCROLLED_WINDOW(scrollbar), 
     GTK_POLICY_NEVER,
     GTK_POLICY_ALWAYS);    

  gtk_box_pack_start(GTK_BOX(panel), text, FALSE, FALSE, 0);
  gtk_widget_show_all(fenetre);
  gtk_main();
*/
}

/*
  call_by: vue_gere_tlv
  permet d'ouvrir dans une fenetre - SDL - une image a partir de son pixbuff
  TODO: remplacer SDL par GTK2+
*/
void vue_fen_view_Image(void *raw_image, int type){
  SDL_Surface *ecran = NULL;
  SDL_Surface *image = NULL;
  SDL_Rect position;
  int continuer = 1;
  SDL_Event evenement;

  int fd, size=0, i=0;
  int newWidth = 0, newHeight = 0;
  char *img_buff, buff;
  
  SDL_Init(SDL_INIT_VIDEO);
  if((fd= open("image.jpg", O_RDONLY)) == -1){
    printf("impossible d'ouvrir le fichier\n");
  }
  
  while((i = read(fd, &buff, 1)) > 0){
    size++;
  }
  close(fd);
  img_buff = malloc(sizeof(char) * size);
  
  if((fd= open("image.jpg", O_RDONLY)) == -1){
    printf("impossible d'ouvrir le fichier\n");
  }
  
  size = 0;
  while((i = read(fd, &buff, 1)) > 0){
    img_buff[size++] = buff;
  }
  close(fd);
  
  position.x = 0;
  position.y = 0;
  SDL_WM_SetCaption("Dazibao", NULL);
  ecran = SDL_SetVideoMode(400, 400, 32, SDL_HWSURFACE| SDL_RESIZABLE);
  SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
  image = IMG_Load_RW(SDL_RWFromMem( img_buff, size), 1);
  if(image  == NULL){ 
    printf("echec image == NULL\n");
    return;
  }
  ecran = SDL_SetVideoMode(image->w, image->h, 32, SDL_HWSURFACE| SDL_RESIZABLE);
  SDL_BlitSurface(image, NULL, ecran, &position);
  SDL_Flip(ecran);
  while(continuer)
    {
      SDL_PollEvent(&evenement);
      switch(evenement.type)
        {
	case SDL_VIDEORESIZE:
	  newWidth = evenement.resize.w;
	  newHeight = evenement.resize.h;
	  SDL_FreeSurface(ecran);
	  ecran = SDL_SetVideoMode
	    (newWidth, newHeight, 32, SDL_HWSURFACE | SDL_RESIZABLE);
	  SDL_BlitSurface(image, NULL, ecran, &position);
	  SDL_Flip(ecran);
	  break;
	case SDL_QUIT:
	  continuer = 0;
	  break;
	case SDL_KEYDOWN:
	  switch(evenement.key.keysym.sym)
            {
	    case SDLK_ESCAPE:
	      continuer = 0;
	      break;
	    default:
	      break;
            }
	  break;
        }
    }
  SDL_Quit();
}


/*
  call by: vue_init_body:
  Se declenche quand l'utilisateur a cliqué sur une TLV.
  En fonction de son id, cette fonction utilise la fonction d'affichage
  appropriée
*/
gint vue_gere_tlv(GtkWidget *tlv_btn, GdkEvent *event, gpointer message){   
  Dazibao_TLV *recup = (Dazibao_TLV *)message;
  switch(recup->type){
  case 2:
    vue_fen_view_Text(recup->value);
    break;
  case 3: case 4:
    vue_fen_view_Image(recup->value, recup->type);
    break;
  case 5:
    vue_view_rep(recup);
    break;
  default:
    fprintf(stderr, "TLV non reconnue (%d)\n", recup->type);
  }
  
  return FALSE;
}

/*
  call by: init
  Signal pour fermer l'ui (quand on clique sur la croix)
*/
gint traitement_quitter(GtkWidget *label, GdkEvent *event, gpointer message){ 
  gtk_main_quit();
  return FALSE;
}

/*
  Call by: init
  initialise le menu du haut de l'ui
*/
void vue_init_head(GtkWidget * panel){
  GtkWidget *home;
  GtkWidget *concat;
  GtkWidget *head_panel = gtk_hbox_new(FALSE,0);
  
  home = gtk_button_new_with_label("home");
  concat = gtk_button_new_with_label("concat");

  gtk_box_pack_start(GTK_BOX(head_panel), home, FALSE, FALSE, 0);  
  gtk_box_pack_start(GTK_BOX(head_panel), concat, FALSE, FALSE, 0);  
  
  gtk_table_attach_defaults(GTK_TABLE(panel), head_panel, 0, 1, 0, 1);
  
  gtk_signal_connect(GTK_OBJECT(home), "clicked", 
		     (GtkSignalFunc)vue_gere_menu, 
		     (gpointer)&(id_bouton[4]));
  /*gtk_signal_connect(GTK_OBJECT(back), "clicked", 
		     (GtkSignalFunc)vue_gere_menu, 
		     (gpointer)&(id_bouton[5]));
  */
  gtk_signal_connect(GTK_OBJECT(concat), "clicked", 
		     (GtkSignalFunc)vue_gere_menu, 
		     (gpointer)&(id_bouton[6]));
}

/*
  call by: vue_init_body
  sous-fonction utilisée par vue_init_body afin de donner un "titre"
  en fonction de l'id de la TLV
  Si une TLV est une date, on prend la tlv contenu, 
  tant que c'est le cas, on continue de prendre ce qu'elle contient 
  jusqu'a ce qu'on arrive sur autre chose qu'une date
*/
const char *label_button(Dazibao_TLV* current_tlv){
  Dazibao_TLV_Dated_Value *d;
  if(current_tlv == NULL){
    return "TLV NULLE";
  }
  switch(current_tlv->type){
  case 2:
    return "Text";
  case 3:
    return "PNG";
  case 4:
    return "JPEG";
  case 5:
    return "Repertoire";
  case 6:
    d = current_tlv->value;
    return label_button(d->element);
  default:
    return "????";
  }
  return "????";
}

/*
  call by: init
  Initialise le corps de l'ui, on affiche les TLV qui sont a la bases du dazibao,
  Les séries de TLV dates sont inscrites dans une cases avec un scroll:
  Si on a quelque chose comme date_1(date_2(...(date_n(TLV_autre_qu_une_date))))
  on aura comme case : (une case qui contient toutes les dates, last_TLV)  
*/
void vue_init_body(GtkWidget * panel, Dazibao_TLV **tlv, int nb_tlv){
  GtkWidget* scrollbar;
  char texte[10];
  GtkWidget* label_date;
  int i;
  /* declaration des variables du corps */
  body_panel	= gtk_vbox_new(FALSE,0);      
  /* on pose un scroll sur la liste des tlv*/
  scrollbar	= gtk_scrolled_window_new(NULL, NULL); 
  /* affichage des TLV dans la liste body_panel*/
  for(i = 0; i < nb_tlv; i++){
    /* on prend le 1er tlv */    
    GtkWidget* panel_tlv;
    GtkWidget* scrollbar_date;
    GtkWidget* dates;
    GtkWidget* button_tlv;
    /* definitions des elements de la liste */
    /*une serie de dates sous forme de box*/
    dates = gtk_vbox_new(FALSE,0);
    /* le panel du tlv */
    panel_tlv = gtk_table_new(1, 5, TRUE);  
    /*bouton de tlv*/
    button_tlv = gtk_button_new_with_label(label_button(tlv[i]));
    /*le scroll sur la liste de dates*/
    scrollbar_date = gtk_scrolled_window_new(NULL, NULL); 
    /* on pose un scroll sur le box des dates */
    gtk_scrolled_window_add_with_viewport
      (GTK_SCROLLED_WINDOW(scrollbar_date),
       dates);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar_date),
				   GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    if(tlv[i]->type == DATED){
      Dazibao_TLV_Dated_Value *date = tlv[i]->value;
      Dazibao_TLV *curseur_date;
      do{
	sprintf(texte, "%ld", date->timestamp);
	label_date = gtk_label_new(texte);
	gtk_box_pack_start(GTK_BOX(dates), label_date, FALSE, FALSE, 5);
	curseur_date = date->element;
	date = curseur_date->value;
      }while(curseur_date->type == DATED);
    } else {
      sprintf(texte, "-");
      label_date = gtk_label_new(texte);
      gtk_box_pack_start(GTK_BOX(dates), label_date, FALSE, FALSE, 5);   
    }
    /* insertion */
    gtk_table_attach_defaults(GTK_TABLE(panel_tlv), scrollbar_date, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(panel_tlv), button_tlv, 1, 5, 0, 1);    
    gtk_box_pack_start(GTK_BOX(body_panel), panel_tlv, FALSE, FALSE, 5);
    /* on passe a la prochaine tlv a afficher */    
    if(tlv[i]->type != DATED){      
      gtk_signal_connect(GTK_OBJECT(button_tlv), "clicked", 
			 (GtkSignalFunc)vue_gere_tlv, 
			 (gpointer)(tlv[i]));
    } else {
      
      Dazibao_TLV_Dated_Value *date = tlv[i]->value;
      Dazibao_TLV *curseur_date;
      do{
	curseur_date = date->element;
	date = curseur_date->value;
      }while(curseur_date->type == DATED);
      gtk_signal_connect(GTK_OBJECT(button_tlv), "clicked", 
			 (GtkSignalFunc)vue_gere_tlv, 
			 (gpointer)(curseur_date));
      
      	
    }
  } 
  
  /*on configure le scroll*/
  gtk_scrolled_window_add_with_viewport
    (GTK_SCROLLED_WINDOW(scrollbar), body_panel);
  gtk_scrolled_window_set_policy
    (GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);  
  /*on ajout la liste 'scrolle' dans le milieu de la page (8/10e de la fen)*/
  gtk_table_attach_defaults(GTK_TABLE(panel), scrollbar, 0, 1, 1, 9);  
}

/*
  call_by: init
  Initialise le menu du bas de l'ui
*/
void vue_init_foot(GtkWidget * panel){
  GtkWidget *texte;
  GtkWidget *image;
  GtkWidget *date;
  GtkWidget *rep;
  
  /*defintions*/
  foot_panel	= gtk_hbox_new(FALSE,0);
  texte		= gtk_button_new_with_label("Text");
  image		= gtk_button_new_with_label("Image");
  date		= gtk_button_new_with_label("Date");
  rep		= gtk_button_new_with_label("Content");

  /*ajout des boutons au panel head_panel*/
  gtk_box_pack_start(GTK_BOX(foot_panel), texte  , FALSE, FALSE, 0);  
  gtk_box_pack_start(GTK_BOX(foot_panel), image  , FALSE, FALSE, 0);  
  gtk_box_pack_start(GTK_BOX(foot_panel), date   , FALSE, FALSE, 0);  
  gtk_box_pack_start(GTK_BOX(foot_panel), rep    , FALSE, FALSE, 0);  

  /*ajout de head panel dans le panel principal*/
  gtk_table_attach_defaults(GTK_TABLE(panel), foot_panel, 0, 1, 9, 10);
  
  /*activation des signaux sur boutons*/
  gtk_signal_connect(GTK_OBJECT(texte), "clicked", 
		     (GtkSignalFunc)vue_gere_menu, 
		     (gpointer)&(id_bouton[0]));
  gtk_signal_connect(GTK_OBJECT(image), "clicked", 
		     (GtkSignalFunc)vue_gere_menu, 
		     (gpointer)&(id_bouton[1]));
  gtk_signal_connect(GTK_OBJECT(date), "clicked", 
		     (GtkSignalFunc)vue_gere_menu, 
		     (gpointer)&(id_bouton[2]));
  gtk_signal_connect(GTK_OBJECT(rep),
		     "clicked", 
		     (GtkSignalFunc)vue_gere_menu, 
		     (gpointer)&(id_bouton[3]));
}

/*
  initialise l'affichage du dazibao
*/
int vue_init(){
  int i;
  for(i = 0; i < dazibao.tlv_count; i++){
    printf("type = %d\n", dazibao.elements[i]->type);
  }
  
  gtk_init(NULL, NULL);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 520, 700);
  gtk_window_set_title(GTK_WINDOW(window), "Dazibao");
  g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(traitement_quitter),0);
  
  panel = gtk_table_new(10, 1, TRUE);  
  add_dates_saves = gtk_vbox_new(FALSE,0);
  gtk_container_add(GTK_CONTAINER(window),panel);
  
  vue_init_head(panel);
  vue_init_body(panel, dazibao.elements, dazibao.tlv_count);
  vue_init_foot(panel);
  gtk_widget_show_all(window);
  gtk_main();
  
  return 0;
} 
