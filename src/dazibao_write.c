#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

#include "dazibao.h"
#include "dazibao_read.h"
#include "dazibao_write.h"
#include "dazibao_utilities.h"

bool safe_write(int fd, void *buffer, size_t count) {
    if(write(fd, buffer, count) != (ssize_t)count) {
        perror("Write Error:");
        return false;
    } else {
        return true;
    }
}

/* Écrit le header d'un TLV */
void dazibao_write_tlv_header(Dazibao *dazibao, Dazibao_TLV *tlv) {
    char header[4];

    header[0] = tlv->type;

    header[1] = (tlv->length >> 16) & 0xFF;
    header[2] = (tlv->length >> 8) & 0xFF;
    header[3] = tlv->length & 0xFF;

    safe_write(dazibao->file_descriptor, header, DAZIBAO_HEADER_LENGTH);
}

/* Écrit un TLV de type COMPOUND */
void dazibao_write_compound_tlv(Dazibao *dazibao, Dazibao_TLV *tlv) {
    int i = 0;

    dazibao_write_tlv_header(dazibao, tlv);

    for (i = 0; i < ((Dazibao_TLV_Compound_Value *)(tlv->value))->count; i++) {
        dazibao_write_tlv(dazibao, ((Dazibao_TLV_Compound_Value *)(tlv->value))->elements[i]);
    }
}

/* Écrit un TLV de type DATED */
void dazibao_write_dated_tlv(Dazibao *dazibao, Dazibao_TLV *tlv) {
    char date[4];
    Dazibao_TLV *element = get_tlv_dated_element(tlv);
    
    long timestamp = get_tlv_dated_timestamp(tlv);
    
    date[0] = (timestamp >> 24) & 0xFF;
    date[1] = (timestamp >> 16) & 0xFF;
    date[2] = (timestamp >> 8)  & 0xFF;
    date[3] =  timestamp        & 0xFF;
    
    dazibao_write_tlv_header(dazibao, tlv);
    safe_write(dazibao->file_descriptor, date, 4);
    dazibao_write_tlv(dazibao, element);
}

/* Écrit un TLV de type "commun" */
void dazibao_write_raw_tlv(Dazibao *dazibao, Dazibao_TLV *tlv) {
    printf("write tlv header\n");
    dazibao_write_tlv_header(dazibao, tlv);
    printf("write tlv\n");
    safe_write(dazibao->file_descriptor, tlv->value, tlv->length);
}

/* Écrit un TLV */
void dazibao_write_tlv(Dazibao *dazibao, Dazibao_TLV *tlv) {
    switch (tlv->type) {
    case PADN:
    case TEXT:
    case PNG:
    case JPEG:
        dazibao_write_raw_tlv(dazibao, tlv);
        break;
    case DATED:
        dazibao_write_dated_tlv(dazibao, tlv);
        break;
    case COMPOUND:
        dazibao_write_compound_tlv(dazibao, tlv);
        break;
    default:
        printf("write_tlv : UNKNOW TLV TYPE!\n");
    }
}

/* Ajoute un TLV à la fin du fichier */
void dazibao_append_tlv(Dazibao *dazibao, Dazibao_TLV *tlv) {
    
    printf("Appending TLV Type %d at %d\n", tlv->type, dazibao->file_size);

    tlv->position = dazibao->file_size;

    lseek(dazibao->file_descriptor, 0, SEEK_END);

    dazibao_write_tlv(dazibao, tlv);

    printf("[+] Appened\n");

    /* Update file size */
    dazibao_get_file_size(dazibao);
}

void dazibao_remove_tlv(Dazibao *dazibao, Dazibao_TLV *tlv) {
    char buffer = PADN;

    printf("Removing p%d:l%d from file.\n", (int)tlv->position, tlv->length);
    
    lseek(dazibao->file_descriptor, tlv->position, SEEK_SET);
    write(dazibao->file_descriptor, &buffer, 1);
}
