#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

#include "dazibao.h"
#include "dazibao_read.h"
#include "dazibao_write.h"
#include "dazibao_utilities.h"

void dazibao_write_tlv_header(Dazibao *dazibao, Dazibao_TLV *tlv) {
    char header[4];
    int size = tlv->length;
        
    header[0] = tlv->type;
    header[1] = (size >> 16) & 0xFF;
    header[2] = (size >> 8)  & 0xFF;
    header[3] =  size        & 0xFF;
    
    dazibao_write(dazibao->file_descriptor, header, DAZIBAO_HEADER_LENGTH);
}

/*
void dazibao_write_compound_tlv(Dazibao *dazibao, Dazibao_TLV *tlv) {
    int i = 0;
    
    dazibao_write_tlv_header(dazibao, tlv);
    
    for(i = 0; i < (Dazbiao_TLV_Compound_Value *)(tlv->value)->count, i++) {
        dazibao_write_tlv
    }
    
}

void dazibao_write_dated_tlv(Dazibao *dazibao, Dazibao_TLV *tlv) {
    dazibao_write_tlv_header(dazibao, tlv);
    
    
}

*/

void dazibao_write_raw_tlv(Dazibao *dazibao, Dazibao_TLV *tlv) {
    
    dazibao_write_tlv_header(dazibao, tlv);
    
    dazibao_write(dazibao->file_descriptor, tlv->value, tlv->length);
    
}

void dazibao_write_tlv(Dazibao *dazibao, Dazibao_TLV *tlv) {
    lseek(dazibao->file_descriptor, tlv->position, SEEK_SET);
    
    switch (tlv->type) {
    case TEXT:
    case PNG:
    case JPEG:
        dazibao_write_raw_tlv(dazibao, tlv);
        break;
    case DATED:
        /* dazibao_write_dated_tlv(dazibao, tlv); */
    case COMPOUND:
        /* dazibao_write_compound_tlv(dazibao, tlv); */
    default:
        printf("CANT for now !!!\n");
    }
}


bool dazibao_append_tlv(Dazibao *dazibao, Dazibao_TLV *tlv) {
    printf("appending tlv type %d at %d\n", tlv->type, dazibao->file_size);
    
    tlv->position = dazibao->file_size;
    
    dazibao_write_tlv(dazibao, tlv);

    printf("[-] appened\n");
    
    /* update file size */
    dazibao_get_file_size(dazibao);
    
    return true;
}