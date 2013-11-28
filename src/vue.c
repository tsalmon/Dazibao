#include <gtk/gtk.h>  
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "test.h"
#include "vue.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

typedef struct signal_bouton{
  int id_bouton;
}signal_bouton;

int id_bouton[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

gint traitement_bouton(GtkWidget *label, GdkEvent *event, gpointer message){
  int * i =  (int *)message;
  printf("id bouton = %d\n", *i);
  switch(*i){
  case 0:
    makeText();
    break;
  case 1: 
    makeImage();
    break;
  case 2:
    makeDate();
    break;
  case 3:
    printf("rep\n");
    break;
  default:
    printf("non reconnu");
  }
  return FALSE;
}

gint add(GtkWidget *widget, GdkEvent *event, gpointer message){
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(message));
  GtkTextIter i1, it;
  int k, i;
  gchar *text;

  gtk_text_buffer_get_iter_at_mark
    (buffer, &it, gtk_text_buffer_get_insert(buffer));

  k = gtk_text_iter_get_line(&it);
  gtk_text_buffer_get_iter_at_line(buffer, &it, k);
  gtk_text_buffer_get_end_iter(buffer, &i1);
  text = gtk_text_buffer_get_text(buffer, &it, &i1, FALSE);
  for(i=0; text[i];i++);
  text[i]=0;
  printf("%s\n", text);
  return FALSE;
}

void makeDate(){
  struct tlv * curseur;
  GtkWidget* panel_tlv_all;
  GtkWidget* scrollbar;
  int i;
  
  curseur = tlv_actuel->conteneur;
  /* declaration des variables du corps */
  panel_tlv_all	= gtk_vbox_new(FALSE,0);      
  // on pose un scroll sur la liste des tlv
  scrollbar	= gtk_scrolled_window_new(NULL, NULL); 
  
  /* affichage des TLV dans la liste panel_tlv_all*/
  for(i = 0 ; i < tlv_actuel->nb_tlv ; ++i){
    //printf("valeur = %d\n", curseur->type_id);
    /* on prend le 1er tlv */
    
    GtkWidget * panel_tlv;
    GtkWidget* scrollbar_date;
    GtkWidget* dates;
    GtkWidget* button_tlv;
    
    /* definitions des elements de la liste */
    //une serie de dates sous forme de box
    dates = gtk_vbox_new(FALSE,0);
    //le panel du tlv
    panel_tlv = gtk_table_new(1, 5, TRUE);  
    //bouton de tlv
    button_tlv = gtk_button_new_with_label(label_button(curseur));
    //le scroll sur la liste de dates
    scrollbar_date = gtk_scrolled_window_new(NULL, NULL); 
    /* on pose un scroll sur le box des dates */
    gtk_scrolled_window_add_with_viewport
      (GTK_SCROLLED_WINDOW(scrollbar_date),
       dates);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar_date),
				   GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    char texte[10];
    GtkWidget* label_date; 
    sprintf(texte, "-");
    label_date = gtk_label_new(texte);
    gtk_box_pack_start(GTK_BOX(dates), label_date, FALSE, FALSE, 5);   
    
    /* insertion */
    gtk_table_attach_defaults(GTK_TABLE(panel_tlv), scrollbar_date, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(panel_tlv), button_tlv, 1, 5, 0, 1);    
    gtk_box_pack_start(GTK_BOX(panel_tlv_all), panel_tlv, FALSE, FALSE, 5);
    //on passe a la prochaine tlv a afficher
    
    if(curseur->type_id != 6){
      gtk_signal_connect(GTK_OBJECT(button_tlv), "clicked", 
			 (GtkSignalFunc)traitement_tlv, 
			 (gpointer)(curseur));
    } else {
      struct tlv *date = curseur;
      while(date->conteneur != NULL){
	date = date->conteneur;
      }
      gtk_signal_connect(GTK_OBJECT(button_tlv), "clicked", 
			 (GtkSignalFunc)traitement_tlv, 
			 (gpointer)(date));
    }
    curseur = curseur->suivant;
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
		     (gpointer)&(id_bouton[7]));
  gtk_widget_show_all(panel_tlv_all);
  
}

char* makeImage (){
  GtkWidget *dialog;
  GtkWindow *window;
  
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  dialog = gtk_file_chooser_dialog_new 
    ("Save File",
     window,
     GTK_FILE_CHOOSER_ACTION_OPEN,
     GTK_STOCK_CANCEL,
     GTK_RESPONSE_CANCEL,
     GTK_STOCK_OPEN,
     GTK_RESPONSE_ACCEPT,
     NULL);
  gtk_file_chooser_set_do_overwrite_confirmation 
    (GTK_FILE_CHOOSER (dialog), TRUE);
  
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
    char *filename;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    printf("%s\n", filename);
    gtk_widget_destroy (dialog);
    return filename;
  } else {
    return NULL;
  }
}

void afficher_text(int position_tlv){
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
  //gtk_window_set_resizable(GTK_WINDOW(fenetre), FALSE);        
 
  scrollbar = gtk_scrolled_window_new(NULL, NULL);
  panel = gtk_hbox_new(FALSE, 5);  
  
  text = gtk_label_new ("This is an example of a line-wrapped, filled label. "
			"It should be taking "				
			"up the entire              width allocated to it.  " 
			"Here is a sentence to prove "			
			"my point.  Here is another sentence. "	
			"Here comes the sun, do de do de do.\n"	
			"    This is a new paragraph.\n"		
			"    This is another newer, longer, better " 
			"paragraph.  It is coming to an end, " 
			"unfortunately.");
  
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
}

void afficher_image(int position_tlv){
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface *ecran = NULL;
  SDL_Surface *image = NULL;
  SDL_Rect position;

  int fd, size=0, i=0;
  int newWidth = 0, newHeight = 0;
  char *img_buff, buff;
  
  if((fd= open("image.jpg", O_RDONLY)) == -1){
    printf("impossible d'ouvrir le fichier\n");
  }

  //a refaire
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
    //write(fd2, &buff, sizeof(buff));
  }
  close(fd);
  
  position.x = 0;
  position.y = 0;
  SDL_WM_SetCaption("Dazibao", NULL);
  ecran = SDL_SetVideoMode(400, 400, 32, SDL_HWSURFACE| SDL_RESIZABLE);
  SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
  image = IMG_Load_RW(SDL_RWFromMem( img_buff, size), 1);
  if(image  == NULL){ //Si le chargement a raté.
    printf("echec image == NULL\n");
    return;
  }
  SDL_BlitSurface(image, NULL, ecran, &position);
  SDL_Flip(ecran);
  int continuer = 1;
  SDL_Event evenement;
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

void afficher_conteneur(struct tlv * rep){
  printf("repertoire\n");
  tlv_actuel = rep;
  body_init(panel, rep->conteneur, rep->nb_tlv);
  gtk_widget_show_all(window);  
}

gint traitement_tlv(GtkWidget *tlv_btn, GdkEvent *event, gpointer message){ 
  struct tlv *recup = (struct tlv*)message;
  switch(recup->type_id){
  case 2:
    afficher_text(recup->position);
    break;
  case 3: case 4:
    afficher_image(recup->position);
    break;
  case 5:
    afficher_conteneur(recup);
    break;
  default:
    fprintf(stderr, "TLV non reconnue (pour le moment du moins)\n");
  }
  return FALSE;
}

gint traitement_quitter(GtkWidget *label, GdkEvent *event, gpointer message){ 
  gtk_main_quit();
  return FALSE;
}

void makeText(){
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
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar),
				 GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  gtk_box_pack_start(GTK_BOX(text_panel), ok, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(text_panel), scrollbar, TRUE, TRUE, 0);
  /* config text*/
  gtk_text_buffer_create_tag(buffer, "gap","pixels_above_lines", 30, NULL);
  gtk_text_buffer_create_tag(buffer, "lmarg", "left_margin", 5, NULL);
  gtk_text_buffer_create_tag(buffer, "blue_fg", "foreground", "blue", NULL); 
  gtk_text_buffer_create_tag(buffer, "gray_bg", "background", "gray", NULL); 
  gtk_text_buffer_create_tag
    (buffer, "italic", "style", PANGO_STYLE_ITALIC, NULL);
  gtk_text_buffer_create_tag(buffer, "bold", "weight", PANGO_WEIGHT_BOLD, NULL);

  gtk_container_add(GTK_CONTAINER(window), text_panel);
  g_signal_connect_swapped
    (G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), G_OBJECT(window));
  gtk_signal_connect(GTK_OBJECT(ok), "clicked", 
		     (GtkSignalFunc)add, 
		     (gpointer)(view));
  
  gtk_widget_show_all(window);
  gtk_main();

}

void head_init(GtkWidget * panel){
  GtkWidget *home;
  GtkWidget *back;
  GtkWidget *concat;
  GtkWidget *head_panel = gtk_hbox_new(FALSE,0);
  
  home = gtk_button_new_with_label("home");
  back = gtk_button_new_with_label("Back");
  concat = gtk_button_new_with_label("concat");

  gtk_box_pack_start(GTK_BOX(head_panel), home, FALSE, FALSE, 0);  
  gtk_box_pack_start(GTK_BOX(head_panel), back, FALSE, FALSE, 0);  
  gtk_box_pack_start(GTK_BOX(head_panel), concat, FALSE, FALSE, 0);  
  
  gtk_table_attach_defaults(GTK_TABLE(panel), head_panel, 0, 1, 0, 1);
  
  gtk_signal_connect(GTK_OBJECT(home), "clicked", 
		     (GtkSignalFunc)traitement_bouton, 
		     (gpointer)&(id_bouton[4]));
  gtk_signal_connect(GTK_OBJECT(back), "clicked", 
		     (GtkSignalFunc)traitement_bouton, 
		     (gpointer)&(id_bouton[5]));
  gtk_signal_connect(GTK_OBJECT(concat), "clicked", 
		     (GtkSignalFunc)traitement_bouton, 
		     (gpointer)&(id_bouton[6]));
}

const char *label_button(struct tlv* current_tlv){
  switch(current_tlv->type_id){
  case 2:
    return "Text";
  case 3:
    return "PNG";
  case 4:
    return "JPEG";
  case 5:
    return "Repertoir";
  case 6:
    return label_button(current_tlv->conteneur);
  default:
    return "????";
  }
  return "????";
}

void body_init(GtkWidget * panel, struct tlv *tlv_debut, int nb_tlv){ 
  struct tlv * curseur;
  GtkWidget* panel_tlv_all;
  GtkWidget* scrollbar;
  int i;
  
  curseur = tlv_debut;
  /* declaration des variables du corps */
  panel_tlv_all	= gtk_vbox_new(FALSE,0);      
  // on pose un scroll sur la liste des tlv
  scrollbar	= gtk_scrolled_window_new(NULL, NULL); 
  
  /* affichage des TLV dans la liste panel_tlv_all*/
  for(i = 0 ; i < nb_tlv ; ++i){
    //printf("valeur = %d\n", curseur->type_id);
    /* on prend le 1er tlv */
    
    GtkWidget * panel_tlv;
    GtkWidget* scrollbar_date;
    GtkWidget* dates;
    GtkWidget* button_tlv;
    
    /* definitions des elements de la liste */
    //une serie de dates sous forme de box
    dates = gtk_vbox_new(FALSE,0);
    //le panel du tlv
    panel_tlv = gtk_table_new(1, 5, TRUE);  
    //bouton de tlv
    button_tlv = gtk_button_new_with_label(label_button(curseur));
    //le scroll sur la liste de dates
    scrollbar_date = gtk_scrolled_window_new(NULL, NULL); 
    /* on pose un scroll sur le box des dates */
    gtk_scrolled_window_add_with_viewport
      (GTK_SCROLLED_WINDOW(scrollbar_date),
       dates);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar_date),
				   GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    char texte[10];
    GtkWidget* label_date; 
    if(curseur->type_id == 6){
      struct tlv *date = curseur->conteneur;
      int j = 1;
      /* on ajoute les dates au box */
      while(date != NULL){
	sprintf(texte, "date %d", j++);
	label_date = gtk_label_new(texte);
	gtk_box_pack_start(GTK_BOX(dates), label_date, FALSE, FALSE, 5);   
	date = date->conteneur;
      } 
    } else {
      sprintf(texte, "-");
      label_date = gtk_label_new(texte);
      gtk_box_pack_start(GTK_BOX(dates), label_date, FALSE, FALSE, 5);   
    }
    
    /* insertion */
    gtk_table_attach_defaults(GTK_TABLE(panel_tlv), scrollbar_date, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(panel_tlv), button_tlv, 1, 5, 0, 1);    
    gtk_box_pack_start(GTK_BOX(panel_tlv_all), panel_tlv, FALSE, FALSE, 5);
    //on passe a la prochaine tlv a afficher
    
    if(curseur->type_id != 6){
      gtk_signal_connect(GTK_OBJECT(button_tlv), "clicked", 
			 (GtkSignalFunc)traitement_tlv, 
			 (gpointer)(curseur));
    } else {
      struct tlv *date = curseur;
      while(date->conteneur != NULL){
	date = date->conteneur;
      }
      gtk_signal_connect(GTK_OBJECT(button_tlv), "clicked", 
			 (GtkSignalFunc)traitement_tlv, 
			 (gpointer)(date));
    }
    curseur = curseur->suivant;
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
		     (gpointer)&(id_bouton[7]));
}

void foot_init(GtkWidget * panel){
  GtkWidget *texte;
  GtkWidget *image;
  GtkWidget *date;
  GtkWidget *rep;
  GtkWidget *foot_panel;
  
  //defintions
  foot_panel	= gtk_hbox_new(FALSE,0);
  texte		= gtk_button_new_with_label("Text");
  image		= gtk_button_new_with_label("Image");
  date		= gtk_button_new_with_label("Date");
  rep		= gtk_button_new_with_label("Content");

  //ajout des boutons au panel head_panel
  gtk_box_pack_start(GTK_BOX(foot_panel), texte  , FALSE, FALSE, 0);  
  gtk_box_pack_start(GTK_BOX(foot_panel), image  , FALSE, FALSE, 0);  
  gtk_box_pack_start(GTK_BOX(foot_panel), date   , FALSE, FALSE, 0);  
  gtk_box_pack_start(GTK_BOX(foot_panel), rep    , FALSE, FALSE, 0);  

  //ajout de head panel dans le panel principal
  gtk_table_attach_defaults(GTK_TABLE(panel), foot_panel, 0, 1, 9, 10);
  
  //activation des signaux sur boutons
  gtk_signal_connect(GTK_OBJECT(texte), "clicked", 
		     (GtkSignalFunc)traitement_bouton, 
		     (gpointer)&(id_bouton[0]));
  gtk_signal_connect(GTK_OBJECT(image), "clicked", 
		     (GtkSignalFunc)traitement_bouton, 
		     (gpointer)&(id_bouton[1]));
  gtk_signal_connect(GTK_OBJECT(date), "clicked", 
		     (GtkSignalFunc)traitement_bouton, 
		     (gpointer)&(id_bouton[2]));
  gtk_signal_connect(GTK_OBJECT(rep), "clicked", 
		     (GtkSignalFunc)traitement_bouton, 
		     (gpointer)&(id_bouton[3]));
}

int init(int argc, char* argv[]){
  /* config */
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  gtk_window_set_default_size(GTK_WINDOW(window), 520, 700);
  gtk_window_set_title(GTK_WINDOW(window), "Dazibao");
  g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(traitement_quitter),0);
  
  panel = gtk_table_new(10, 1, TRUE);  
  gtk_container_add(GTK_CONTAINER(window),panel);
  
  //signal_btn = malloc(sizeof(signal_bouton) * 6);
  
  head_init(panel);  
  tlv_actuel = daz->tlv_debut;
  tlv_actuel->nb_tlv = daz->nb_tlv;
  body_init(panel, daz->tlv_debut, daz->nb_tlv);
  foot_init(panel);
  
  gtk_widget_show_all(window);
  gtk_main();
  return 0;
} 
