#ifndef DAZIBAO_UTILITIES_H_
#define DAZIBAO_UTILITIES_H_

/* -------------------------------------------------------------------------- */

int get_file_size(char *path);

void dazibao_write(int file_descriptor, void *buffer, size_t count);

/* -------------------------------------------------------------------------- */

char *timestamp_to_date(int timestamp);

char *timestamp_to_date_short(int timestamp);

/* -------------------------------------------------------------------------- */

Dazibao_TLV *create_raw_tlv(Dazibao_TLV_Type type, char *value);

Dazibao_TLV *create_dated_tlv(long timestamp, Dazibao_TLV *element);

Dazibao_TLV *create_compound_tlv(int count, Dazibao_TLV **elements);

/* -------------------------------------------------------------------------- */

Dazibao_TLV *get_tlv_dated_value(Dazibao_TLV *tlv);

long get_tlv_dated_timestamp(Dazibao_TLV *tlv);

int *get_tlv_compound_count(Dazibao_TLV *tlv);

Dazibao_TLV **get_tlv_compound_elements(Dazibao_TLV *tlv);

#endif