#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "dazibao.h"
#include "dazibao_read.h"

bool dazibao_open_file(Dazibao *dazibao) {
    if ((dazibao->file_descriptor = open(dazibao->file_path, O_RDONLY)) < 0) {
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

/* trouve le prochain tlv */
bool dazibao_find_next_tlv(Dazibao *dazibao) {

    return false;
}

/* lit un un tlv et le met en memoire */
bool dazibao_read_tlv(Dazibao *dazibao, Dazibao_TLV *tlv) {

    return false;
}
