#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "dazibao.h"
#include "dazibao_read.h"
#include "dazibao_write.h"

void print_tlv_header(Dazibao_TLV *tlv) {
    printf("[+] TLV | Type %d | at %d | length %d\n", tlv->type, (int)tlv->position, tlv->length);
}

int get_file_size(char *path) {
    struct stat stats;
    if(stat(path, &stats) == -1) {
        perror("get_file_size");
        exit(58);
    }
    return stats.st_size;
}

void dazibao_get_file_size(Dazibao *dazibao) {
    dazibao->file_size = get_file_size(dazibao->file_path);
}

Dazibao_TLV *find_next_tlv(Dazibao *dazibao, int offset) {
    unsigned char header[4];
    
    lseek(dazibao->file_descriptor, offset, SEEK_SET);
    
    if (read(dazibao->file_descriptor, header, DAZIBAO_HEADER_LENGTH) == -1) {
        perror("[!] Error while reading TLV");
        exit(55);
        return NULL;
    } else {
        Dazibao_TLV *new = malloc(sizeof(Dazibao_TLV));
        
        new->type =     header[0];
        new->length =   header[1] * 256 * 256 + header[2] * 256 + header[3];
        new->position = offset;
        
        if(new->type == PADN || new->type == PADONE) {
            printf("PADN | PADONE !!!");
            exit(56);
        }
        return new;
    }
}

Dazibao_TLV **find_next_tlv_array(Dazibao *dazibao, int offset_start, int offset_max, int *tlv_count) {
    Dazibao_TLV *new = NULL;
    Dazibao_TLV **new_array = NULL;
    int next_position = offset_start;

    while (next_position < offset_max) {
        if(*(tlv_count) == 0) {
            new_array = malloc(sizeof(Dazibao_TLV *));
        } else {
            new_array = realloc(new_array, sizeof(Dazibao_TLV *) * ( *(tlv_count) + 1 ));
        }            
        new = find_next_tlv(dazibao, next_position);
        print_tlv_header(new);
        next_position += DAZIBAO_HEADER_LENGTH + new->length;
        new_array[*(tlv_count)] = new;
        (*tlv_count)++;
    }
    return new_array;
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

    printf("[d] Dated : Timestamp %ld\n[d] Element : ", dated->timestamp);
    print_tlv_header(dated->element);

    return dated;
}

Dazibao_TLV_Compound_Value *load_tlv_value_compound(Dazibao *dazibao, Dazibao_TLV *tlv) {
    
    Dazibao_TLV_Compound_Value *compound = malloc(sizeof(Dazibao_TLV_Compound_Value));
    
    compound->elements = find_next_tlv_array(dazibao, tlv->position + 4, tlv->position + tlv->length + 4, &compound->count);
    
    printf("[c] Compound contains %d elements.\n", compound->count);
    
    return compound;
    
}

void load_tlv_value(Dazibao *dazibao, Dazibao_TLV *tlv) {
    printf("[-] Loading tlv type %d at %d\n", tlv->type, (int)tlv->position);
    
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
    printf("[-] ok\n");
}

int main(int argc, char **argv) {

    Dazibao dazibao;
	dazibao.file_path = argv[1];
	dazibao.file_descriptor = 0;
	dazibao.file_size = 0;
	dazibao.tlv_count = 0;
	dazibao.elements = NULL;
	
	if (argc != 2) {
        printf("!!! ARGS !!!\n");
        return 0;
    }
	
	printf("[i] Dazibao file : %s\n", dazibao.file_path);
	
	dazibao_get_file_size(&dazibao);
	dazibao_open_file(&dazibao);
	
	if (!dazibao_check_header(&dazibao)) {
        printf("[!] File header is not valid!\n");
    } else {
        printf("[+] Finding root tlv :\n\n");
        
        dazibao.elements = find_next_tlv_array(&dazibao, DAZIBAO_HEADER_LENGTH, dazibao.file_size, &dazibao.tlv_count);
        
        printf("\n[+] now loading tlvs one by one :\n");
        load_tlv_value(&dazibao, dazibao.elements[0]);
        printf("%s\n", (char *)dazibao.elements[0]->value);

        load_tlv_value(&dazibao, dazibao.elements[1]);
        load_tlv_value(&dazibao, dazibao.elements[2]);
        load_tlv_value(&dazibao, dazibao.elements[3]);
        load_tlv_value(&dazibao, ((Dazibao_TLV_Dated_Value *)dazibao.elements[3]->value)->element);

        /* load_tlv_value(&dazibao, (Dazibao_TLV_Compound_Value *)((Dazibao_TLV_Dated_Value *)dazibao.elements[3]->value)->element->elements[0]); */
             
    } 

	dazibao_close_file(&dazibao);

	printf("[i] :)\n");
	return EXIT_SUCCESS;
}