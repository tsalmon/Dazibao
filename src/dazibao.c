#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


#include "dazibao.h"
#include "dazibao_read.h"
#include "dazibao_write.h"
#include "dazibao_vue.h"

int main(int argc, char **argv) {
  
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
    load_tlv_init(dazibao.elements, dazibao.tlv_count);
    
    /*
    newtlv.type = TEXT;
    newtlv.position = -1;
    newtlv.value = "Bonjour Monsieur................";
    newtlv.length = strlen(newtlv.value);        
    
    printf("\n[+] now loading tlvs one by one :\n");
    load_tlv_value(&dazibao, dazibao.elements[0]);
    load_tlv_value(&dazibao, dazibao.elements[1]);
    load_tlv_value(&dazibao, dazibao.elements[2]);
    load_tlv_value(&dazibao, dazibao.elements[3]);
    load_tlv_value(&dazibao, ((Dazibao_TLV_Dated_Value *)dazibao.elements[3]->value)->element);
    load_tlv_value(&dazibao, ((Dazibao_TLV_Dated_Value *)dazibao.elements[2]->value)->element);
    
      load_tlv_value(&dazibao, (Dazibao_TLV_Compound_Value *)((Dazibao_TLV_Dated_Value *)dazibao.elements[3]->value)->element->elements[0]);
    */
  } 
  
  
  vue_init();
  
  printf("[i] :)\n");
  dazibao_close_file(&dazibao);
  return EXIT_SUCCESS;
}
