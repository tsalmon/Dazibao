#ifndef DAZIBAO_UTILITIES_H_
#define DAZIBAO_UTILITIES_H_

/* -------------------------------------------------------------------------- */

int get_file_size(char *path);

void dazibao_write(int file_descriptor, void *buffer, size_t count);

/* -------------------------------------------------------------------------- */

char *timestamp_to_date(int timestamp);

/* -------------------------------------------------------------------------- */

Dazibao_TLV *get_tlv_dated_value(Dazibao_TLV *tlv);

long get_tlv_dated_timestamp(Dazibao_TLV *tlv);

int *get_tlv_compound_count(Dazibao_TLV *tlv);

Dazibao_TLV **get_tlv_compound_elements(Dazibao_TLV *tlv);

#endif