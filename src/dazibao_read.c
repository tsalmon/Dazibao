#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "dazibao.h"

#include "dazibao_read.h"
#include "dazibao_utilities.h"


bool safe_read(int fd, void *buffer, size_t count) {
    if(read(fd, buffer, count) == -1) {
        perror("Read Error:");
        return false;
    } else {
        return true;
    }
}

bool dazibao_open_file(Dazibao *dazibao) {
    if ((dazibao->file_descriptor = open(dazibao->file_path, O_RDWR)) < 0) {
        perror("[!] Error when opening file");
        return false;
    } else {
        printf("[+] File opened.\n");
        return true;
    }
}

bool dazibao_close_file(Dazibao *dazibao) {
    if (close(dazibao->file_descriptor) != 0) {
        perror("[!] Error when closing file");
        return false;
    } else {
        printf("[+] File closed.\n");
        return true;
    }
}

void dazibao_get_file_size(Dazibao *dazibao) {
    dazibao->file_size = get_file_size(dazibao->file_path);
}

bool dazibao_check_header(Dazibao *dazibao) {
    char buffer[4];

    lseek(dazibao->file_descriptor, 0, SEEK_SET);

    if (read(dazibao->file_descriptor, buffer, DAZIBAO_HEADER_LENGTH) == -1) {
        perror("[!] Error while reading file");
        return false;
    } else {
        if (buffer[0] == DAZIBAO_MAGIC_NUMBER && buffer[1] == DAZIBAO_VERSION) {
            return true;
        } else {
            return false;
        }
    }
}

/* TODO FONCTIONNNN À REFAIRE !!! TODO */
Dazibao_TLV *find_next_tlv(Dazibao *dazibao, int offset) {
  unsigned char buffer[4096];
  
   int i;
   
   /*printf("find_next_tlv at %d\n", (int)offset);
    */
   
   lseek(dazibao->file_descriptor, offset, SEEK_SET);
   
   if (read(dazibao->file_descriptor, buffer, 4096) == -1) {
     perror("[!] Error while reading TLV");
     exit(55);
     return NULL;
   } else {
     Dazibao_TLV *new = malloc(sizeof(Dazibao_TLV));
     
     new->type =     buffer[0];
     new->length =   buffer[1] * 256 * 256 + buffer[2] * 256 + buffer[3];
     new->position = offset;
     new->value =    NULL;
     
     if(new->type == PADONE) {
       for(i = 0; i < 4096; i++) {
	 if(buffer[i] != PADONE) {
	   return find_next_tlv(dazibao, offset + i);
	 }
       }
     } else {
       return new;
     }
   }
   return NULL;
}

Dazibao_TLV **find_next_tlv_array(Dazibao *dazibao, int offset_start, int offset_max, int *tlv_count) {
   Dazibao_TLV *new = NULL;
   Dazibao_TLV **new_array = NULL;
   int next_position = offset_start;

   while (next_position < offset_max) {
       if(*tlv_count == 0) {
           new_array = malloc(sizeof(Dazibao_TLV *));
       } else {
	 new_array = realloc(new_array, sizeof(Dazibao_TLV *) * (*tlv_count + 1 ));
       }
       new_array[(*tlv_count)++] = new = find_next_tlv(dazibao, next_position);
       next_position = new->position + DAZIBAO_HEADER_LENGTH + new->length;
   }
   return new_array;
}

/*
  call by: main
  on va lire l'arbre des tlv, et associer a chacune d'entre elle une valeur
*/
void load_tlv_init(Dazibao_TLV **element, int nb_elem){
  int i			= 0;
  Dazibao_TLV *aux	= NULL;
  Dazibao_TLV **aux_tab = NULL;
  for(; i < nb_elem; i++){
    /*on initialise la tlv*/
    load_tlv_value(&dazibao, element[i]);
    /*on suis l'arborescence*/
    switch(element[i]->type){
    case DATED:
      do{
	aux = get_tlv_dated_element(element[i]);
	load_tlv_value(&dazibao, aux);
	if(aux->type == COMPOUND){
	  aux_tab = get_tlv_compound_elements(aux);
	  load_tlv_init(aux_tab, ((Dazibao_TLV_Compound_Value *)aux->value)->count);
	}
      }while(aux->type == DATED); /*  cas de dates successives  */
      break;
    case COMPOUND: 
      aux_tab = get_tlv_compound_elements(element[i]);
      load_tlv_init(aux_tab, ((Dazibao_TLV_Compound_Value *)element[i]->value)->count);
      break;
    default:; /*les autres tlv ne nous interesse pas*/
      
    }
    
  }
}

void load_tlv_value(Dazibao *dazibao, Dazibao_TLV *tlv) {
    /*printf("[-] Loading tlv type %d at %d\n", tlv->type, (int)tlv->position);
   */
   switch(tlv->type) {
       case TEXT   :
       case PNG    :
       case JPEG   :
           tlv->value = load_tlv_value_raw(dazibao, tlv);
           break;
       case DATED  :
           tlv->value = load_tlv_value_dated(dazibao, tlv);
           break;
       case COMPOUND:
           tlv->value = load_tlv_value_compound(dazibao, tlv);
           break;
       default     :
           printf("CANT LOAD VALUE !!!\n");
   }
}



char *load_tlv_value_raw(Dazibao *dazibao, Dazibao_TLV *tlv) {
   char *value = malloc(sizeof(char) * (tlv->length));
   lseek(dazibao->file_descriptor, tlv->position + 4, SEEK_SET);
   read(dazibao->file_descriptor, value, tlv->length);
   return value;
}

Dazibao_TLV_Dated_Value *load_tlv_value_dated(Dazibao *dazibao, Dazibao_TLV *tlv) {
   
   char buffer[4];
   Dazibao_TLV_Dated_Value *dated = malloc(sizeof(Dazibao_TLV_Dated_Value));
   
   lseek(dazibao->file_descriptor, tlv->position + 4, SEEK_SET);
   read(dazibao->file_descriptor, buffer, 4);

   dated->timestamp = buffer[0] * 256 * 256 * 256 + buffer[1] * 255 * 256 + buffer[2] * 256 + buffer[3];
   dated->element = find_next_tlv(dazibao, tlv->position + 8);

   return dated;
}

Dazibao_TLV_Compound_Value *load_tlv_value_compound(Dazibao *dazibao, Dazibao_TLV *tlv) {
   
   Dazibao_TLV_Compound_Value *compound = malloc(sizeof(Dazibao_TLV_Compound_Value));
   int count = 0;
   
   compound->elements = find_next_tlv_array(dazibao, tlv->position + 4, tlv->position + tlv->length + 4, &count);

   compound->count = count;
   
   /*
   printf("[c] Compound contains %d elements.\n", compound->count);
   */
   return compound;
   
}



/* TODO */

/* FONCTIONS À GARDER ? */
void print_tlv_header(Dazibao_TLV *tlv) {
    printf("[+] TLV | Type %s | at %d | length %d\n", tlv_type_to_text(tlv->type), (int)tlv->position, tlv->length);
}

/* TODO */
