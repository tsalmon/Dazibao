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

int id_bouton[] = {0, 1, 2, 3, 4, 5, 6};

gint traitement_bouton(GtkWidget *label, GdkEvent *event, gpointer message){
  int * i =  (int *)message;
  printf("id bouton = %d\n", *i);
  return FALSE;
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
  
  text = gtk_label_new ("This is an example of a line-wrapped, filled label.  " 
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


/*
void afficher_image(int position_tlv){
  GtkWidget *fenetre;
  GtkWidget *panel;
  GtkWidget * scrollbar;
  
  int i = 0, size = 0;
  const gchar *img_buff;
  int fd; 
  GtkWidget *image;
  
  if((fd= open("image.jpg", O_RDONLY)) == -1){
    printf("impossible d'ouvrir le fichier\n");
  }
  
  while((i = read(fd, &img_buff, 1)) > 0){
    size++;
  }
  close(fd);
  img_buff = malloc(sizeof(char) * size);
  
  if((fd= open("image.jpg", O_RDONLY)) == -1){
    printf("impossible d'ouvrir le fichier\n");
  }
  size = 0;
  while((i = read(fd, &img_buff, 1)) > 0){
    img_buff[size++];
  }
  close(fd);
  printf("size = %d\n", size);
  
  image = gtk_image_new_from_stock(img_buff, size);
  
  gtk_init(&i, NULL);
  fenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
  gtk_window_set_title (GTK_WINDOW(fenetre), "Hello World !");
  gtk_window_set_default_size(GTK_WINDOW(fenetre), 400, 400);
  g_signal_connect(G_OBJECT(fenetre),"destroy",G_CALLBACK(gtk_main_quit),0); 
  
  scrollbar = gtk_scrolled_window_new(NULL, NULL);
  panel = gtk_hbox_new(FALSE, 5);  
  
  gtk_container_add(GTK_CONTAINER(fenetre), panel);
  
  gtk_scrolled_window_add_with_viewport
    (GTK_SCROLLED_WINDOW(scrollbar), panel);
  gtk_scrolled_window_set_policy
    (GTK_SCROLLED_WINDOW(scrollbar), 
     GTK_POLICY_NEVER,
     GTK_POLICY_ALWAYS);    
  
  gtk_box_pack_start(GTK_BOX(panel), image, FALSE, FALSE, 0);
  gtk_widget_show_all(fenetre);
  gtk_main();
}
*/

void afficher_image(int position_tlv){
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface *ecran = NULL;
  SDL_Surface *image = NULL;
  SDL_Rect position;

  int fd, size=0, i=0;
  char *img_buff, buff;
  
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
    //write(fd2, &buff, sizeof(buff));
  }
  close(fd);
  
  position.x = 0;
  position.y = 0;
  SDL_WM_SetCaption("Charger des images avec la SDL", NULL);
  ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
  SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
  //image = IMG_Load("image.jpg");
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

gint traitement_tlv(GtkWidget *tlv_btn, GdkEvent *event, gpointer message){ 
  struct tlv *recup = (struct tlv*)message;
  switch(recup->type_id){
  case 2:
    afficher_text(recup->position);
    break;
  case 3: case 4:
    afficher_image(recup->position);
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

void body_init(GtkWidget * panel, int n){ 
  struct tlv * curseur;
  GtkWidget* panel_tlv_all;
  GtkWidget* scrollbar;
  int i;
  
  curseur = daz->tlv_debut;
  /* declaration des variables du corps */
  panel_tlv_all	= gtk_vbox_new(FALSE,0);      
  // on pose un scroll sur la liste des tlv
  scrollbar	= gtk_scrolled_window_new(NULL, NULL); 
  
  /* affichage des TLV dans la liste panel_tlv_all*/
  for(i = 0 ; i < daz->nb_tlv ; ++i){
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

int init(int argc, char* argv[]){
  if(argc != 2){
    return 1;
  }

  GtkWidget* window;
  GtkWidget* panel;
  int n = atoi( argv[1]);
  
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
  body_init(panel, n);
  foot_init(panel);
  
  gtk_widget_show_all(window);
  gtk_main();
  return 0;
} 
