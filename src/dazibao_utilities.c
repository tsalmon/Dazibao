#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "dazibao.h"

/* À supprimer ! */
#include "dazibao_read.h"

#define DATE_FORMAT "%d/%m/%y %Hh%Mm%Ss"
#define DATE_LENGTH strlen(DATE_FORMAT) /* 18 */

#define SHORT_DATE_FORMAT "%d/%m/%y"
#define SHORT_DATE_LENGTH strlen(SHORT_DATE_FORMAT) /* 8 */

#define TAB "    "

/* Retourne la taille d'un fichier */
int get_file_size(char *path) {
    struct stat stats;
    if(stat(path, &stats) == -1) {
        perror("get_file_size");
        exit(58);
    }
    return stats.st_size;
}

/* Converti un timestamp en date lisible format long */
char *timestamp_to_date(int timestamp) {
    time_t time = timestamp;
    struct tm *tmp;

    char *output = malloc(sizeof(char) * DATE_LENGTH);

    tmp = localtime(&time);
    strftime(output, DATE_LENGTH, DATE_FORMAT, tmp);

    return output;
}

/* Converti un timestamp en date lisible format court */
char *timestamp_to_date_short(int timestamp) {
    time_t time = timestamp;
    struct tm *tmp;

    char *output = malloc(sizeof(char) * DATE_LENGTH);

    tmp = localtime(&time);
    strftime(output, SHORT_DATE_LENGTH, SHORT_DATE_FORMAT, tmp);

    return output;
}

/* Crée un TLV de type "commun" */
Dazibao_TLV *create_raw_tlv(Dazibao_TLV_Type type, void *value) {
    Dazibao_TLV *output = malloc(sizeof(Dazibao_TLV));

    output->length = strlen(value);
    output->position = -1;
    output->type = type;
    output->value = value;

    return output;
}

/* Crée un TLV de type TEXT */
Dazibao_TLV *create_text_tlv(char *value) {
    Dazibao_TLV *output = malloc(sizeof(Dazibao_TLV));

    output->length = strlen(value);
    output->position = -1;
    output->type = TEXT;
    output->value = value;

    return output;
}

/* Crée un TLV de type DATED */
Dazibao_TLV *create_dated_tlv(long timestamp, Dazibao_TLV *element) {

    Dazibao_TLV_Dated_Value *value = malloc(sizeof(Dazibao_TLV_Dated_Value));
    Dazibao_TLV *output = malloc(sizeof(Dazibao_TLV));
    
    value->element = element;
    value->timestamp = timestamp;

    output->length = element->length + 8;
    output->position = -1;
    output->type = DATED;
    output->value = value;

    return output;
}

/* Crée un TLV de type COMPOUND */
Dazibao_TLV *create_compound_tlv(int count, Dazibao_TLV **elements) {
    
    Dazibao_TLV_Compound_Value *value = malloc(sizeof(Dazibao_TLV_Compound_Value));
    Dazibao_TLV *output = malloc(sizeof(Dazibao_TLV));
    int i = 0;
    int length = 0;
    
    for(i = 0; i < count; i++) {
        if(elements[i]->type == DATED) {
            length += elements[i]->length + 8;
        } else {
            length += elements[i]->length + 4;
        }
    }
    
    value->count = count;
    value->elements = elements;
    
    output->length = length;
    output->position = -1;
    output->type = COMPOUND;
    output->value = value;
    
    return output;
}

long get_tlv_dated_timestamp(Dazibao_TLV *tlv) {
    if(tlv->type == DATED && tlv->value != NULL) {
        return ((Dazibao_TLV_Dated_Value *)tlv->value)->timestamp;
    } else {
        return 0;
    }
}

Dazibao_TLV *get_tlv_dated_element(Dazibao_TLV *tlv) {
    if(tlv->type == DATED) {
        return ((Dazibao_TLV_Dated_Value *)tlv->value)->element;
    } else {
        return NULL;
    }
}

int get_tlv_compound_count(Dazibao_TLV *tlv) {
    if(tlv->type == COMPOUND) {
        return ((Dazibao_TLV_Compound_Value *)tlv->value)->count;
    } else {
        return -1;
    }
}

Dazibao_TLV **get_tlv_compound_elements(Dazibao_TLV *tlv) {
    if(tlv->type == COMPOUND) {
        return ((Dazibao_TLV_Compound_Value *)tlv->value)->elements;
    } else {
        return NULL;
    }
}

char *tlv_type_to_text(Dazibao_TLV_Type type) {
    char *strings[] = {"PADONE", "PADN", "TEXT", "PNG", "JPEG", "COMPOUND", "DATED"};
    return (type < 0 || type > 7) ? "UNKNOW" : strings[type];
}

char *get_tlv_header(Dazibao_TLV *tlv) {
    char *output = malloc(sizeof(char) * DAZIBAO_BUFFER_SIZE);
    sprintf(output, ". %s | at %d | length %d", tlv_type_to_text(tlv->type), (int)tlv->position, tlv->length);
    return output;
}

char *string_repeat(int n, const char *s) {
    size_t slen = strlen(s);
    char *dest = malloc(n * slen + 1);

    int i;
    char *p;
    for (i = 0, p = dest; i < n; ++i, p += slen) {
        memcpy(p, s, slen);
    }
    *p = '\0';
    return dest;
}

/*
  insert des sauts de lignes pour eviter qu'une lignes 
  depasse les 80 caracteres de long
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


void dazibao_print_tree(Dazibao *dazibao, Dazibao_TLV **elements, int count, int deepness) {
    int i;
    
    /* TODO Comment faire sans allouer une tmp ici ??? */
    Dazibao_TLV *tmp;

    for (i = 0; i < count; i++) {
        printf("%s%d)%s\n", string_repeat(deepness, TAB), i ,get_tlv_header(elements[i]));

        switch(elements[i]->type) {
        case DATED:
            load_tlv_value(dazibao, elements[i]);
            tmp = get_tlv_dated_element(elements[i]);
            dazibao_print_tree(dazibao, &tmp, 1, deepness + 1);
            break;
        case COMPOUND:
            load_tlv_value(dazibao, elements[i]);
            dazibao_print_tree(dazibao, get_tlv_compound_elements(elements[i]), get_tlv_compound_count(elements[i]), deepness + 1);
            break;
        default:;
        }
    }
}

unsigned char* convert_int_to_char(int size) {
    
    unsigned char *length = (unsigned char *) safe_malloc (3);

    length[0] = (size >> 16) & 0xFF;
    length[1] = (size >> 8) & 0xFF;
    length[2] = size & 0xFF;
    
    return (length);

}
