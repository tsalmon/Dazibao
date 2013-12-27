#ifndef DAZIBAO_H_
#define DAZIBAO_H_

#include <sys/types.h>

#define DAZIBAO_HEADER_LENGTH   4
#define DAZIBAO_MAGIC_NUMBER   53
#define DAZIBAO_VERSION         0

#define DAZIBAO_BUFFER_SIZE  4096

typedef enum { false, true } bool;

typedef enum {
    UNKNOW=    -1,
    PADONE=     0,
    PADN=       1,
    TEXT=       2,
    PNG=        3,
    JPEG=       4,
    COMPOUND=   5,
    DATED=      6
} Dazibao_TLV_Type;

typedef struct {
    Dazibao_TLV_Type type;
    off_t position;
    int length;
    void *value;
} Dazibao_TLV;

typedef struct {
    char *file_path;
    int file_descriptor;
    int file_size;
    int tlv_count;
    Dazibao_TLV **elements;
} Dazibao;

typedef struct {
    int count;
    Dazibao_TLV **elements;
} Dazibao_TLV_Compound_Value;

typedef struct {
    long timestamp;
    Dazibao_TLV *element;
} Dazibao_TLV_Dated_Value;

Dazibao dazibao;
Dazibao_TLV *tlv_dated_selected;
Dazibao_TLV **tlv_compound;
int nb_tlv_compound;
#endif
