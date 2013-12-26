#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "dazibao.h"

#define DATE_FORMAT "%d/%m/%y %Hh%Mm%Ss"
#define DATE_LENGTH strlen(DATE_FORMAT) /* 18 */

/* -------------------------------------------------------------------------- */

/* Retourne la taille d'un fichier */
int get_file_size(char *path) {
    struct stat stats;
    if(stat(path, &stats) == -1) {
        perror("get_file_size");
        exit(58);
    }
    return stats.st_size;
}


/* read(2) wrapper */
/* TODO */

/* write(2) wrapper */
void dazibao_write(int file_descriptor, void *buffer, size_t count) {
    if(write(file_descriptor, buffer, count) == -1) {
        perror("error while writing");
        exit(55);
    }
}

/* -------------------------------------------------------------------------- */

/* Converti un timestamp en date lisible */
char *timestamp_to_date(int timestamp) {
    time_t time = timestamp;
    struct tm *tmp;

    char *output = malloc(sizeof(char) * DATE_LENGTH);

    tmp = localtime(&time);
    strftime(output, DATE_LENGTH, DATE_FORMAT, tmp);

    return output;
}

/* -------------------------------------------------------------------------- */

/* Crée un TLV de type "commun" */
Dazibao_TLV *create_raw_tlv(Dazibao_TLV_Type type, char *value) {
    Dazibao_TLV *output = malloc(sizeof(Dazibao_TLV));

    output->length = strlen(value);
    output->position = -1;
    output->type = type;
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

/* -------------------------------------------------------------------------- */

long get_tlv_dated_timestamp(Dazibao_TLV *tlv) {
    if(tlv->type == DATED) {
        return ((Dazibao_TLV_Dated_Value *)tlv->value)->timestamp;
    } else {
        return -1;
    }
}

Dazibao_TLV *get_tlv_dated_value(Dazibao_TLV *tlv) {
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

/* -------------------------------------------------------------------------- */

















