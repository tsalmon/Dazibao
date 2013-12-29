#ifndef DAZIBAO_WRITE_H_
#define DAZIBAO_WRITE_H_

#include "dazibao.h"

void dazibao_write_tlv_header(Dazibao *dazibao, Dazibao_TLV *tlv);

void dazibao_write_tlv(Dazibao *dazibao, Dazibao_TLV *tlv);

void dazibao_write_raw_tlv(Dazibao *dazibao, Dazibao_TLV *tlv);

void dazibao_write_dated_tlv(Dazibao *dazibao, Dazibao_TLV *tlv);

void dazibao_append_tlv(Dazibao *dazibao, Dazibao_TLV *tlv);

void dazibao_remove_tlv(Dazibao *dazibao, Dazibao_TLV *tlv);

#endif
