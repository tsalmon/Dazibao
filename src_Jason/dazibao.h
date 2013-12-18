#ifndef DAZIBAO_H_
#define DAZIBAO_H_

#include <sys/types.h>

#define DAZIBAO_HEADER_LENGTH   4
#define DAZIBAO_MAGIC_NUMBER   53
#define DAZIBAO_VERSION         0

typedef enum {false, true} bool;

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
    int tlv_count;
    Dazibao_TLV *content;
} Dazibao;

typedef struct {
    int tlv_count;
    Dazibao_TLV *content;
} Dazibao_TLV_Compound_Value;

typedef struct {
    long timestamp;
    Dazibao_TLV *content;
} Dazibao_TLV_Dated_Value;

#endif
