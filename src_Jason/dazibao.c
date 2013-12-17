#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "dazibao.h"
#include "dazibao_read.h"
#include "dazibao_write.h"

int main(int argc, char **argv) {
	
	Dazibao dazibao;
	dazibao.file_path = argv[1];
	dazibao.tlv_count = 0;
	
	Dazibao_TLV *new_tlv;
	dazibao.content[10];
	
	
	
	printf("[i] Dazibao file : %s\n", dazibao.file_path);
	
	dazibao_open_file(&dazibao);
	
	if(!dazibao_check_header(&dazibao)) {
        printf("[!] File header is not valid!\n");
	} else {
	    printf("[+] Reading...\n");
	    
	    while(dazibao.tlv_count < 6) {
	        /* Read next TLV */

	        printf("%d:\n", dazibao.tlv_count);
	        new_tlv = malloc(sizeof(Dazibao_TLV));
	        
	        /* (*dazibao.content + dazibao.tlv_count) = new_tlv; */
	        dazibao.tlv_count++;
	    }
	    
	}
	
	dazibao_close_file(&dazibao);

	printf("[i] :)\n");
	return EXIT_SUCCESS;
}