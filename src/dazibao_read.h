#ifndef DAZIBAO_READ_H_
#define DAZIBAO_READ_H_

#include "dazibao.h"

bool safe_read(int fd, void *buffer, size_t count);

bool dazibao_open_file(Dazibao *dazibao);

bool dazibao_close_file(Dazibao *dazibao);

void dazibao_get_file_size(Dazibao *dazibao);

bool dazibao_check_header(Dazibao *dazibao);

Dazibao_TLV *find_next_tlv(Dazibao *dazibao, int offset);

Dazibao_TLV **find_next_tlv_array(Dazibao *dazibao, int offset_start, int offset_max, int *tlv_count) ;

void load_tlv_value(Dazibao *dazibao, Dazibao_TLV *tlv);

char *load_tlv_value_raw(Dazibao *dazibao, Dazibao_TLV *tlv);

Dazibao_TLV_Dated_Value *load_tlv_value_dated(Dazibao *dazibao, Dazibao_TLV *tlv);

Dazibao_TLV_Compound_Value *load_tlv_value_compound(Dazibao *dazibao, Dazibao_TLV *tlv);

void print_tlv_header(Dazibao_TLV *tlv);

#endif
