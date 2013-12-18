#ifndef DAZIBAO_READ_H_
#define DAZIBAO_READ_H_

#include "dazibao.h"

bool dazibao_open_file(Dazibao *dazibao);

bool dazibao_close_file(Dazibao *dazibao);

bool dazibao_check_header(Dazibao *dazibao);

bool dazibao_find_next_tlv(Dazibao *dazibao);

bool dazibao_read_tlv(Dazibao *dazibao, Dazibao_TLV *tlv);


#endif