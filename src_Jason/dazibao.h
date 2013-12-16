#ifndef DAZIBAO_H_
#define DAZIBAO_H_

#include <sys/types.h>

/*
	Énumération Dazibao_TLV_Type : Type d'un TLV
*/
typedef enum {
	UNKNOW=	   -1,
	PADONE=		0,
	PADN=		1,
	TEXT=		2,
	PNG=		3,
	JPEG=		4,
	COMPOUND=	5,
	DATED=		6
} Dazibao_TLV_Type;



/*
	Structure Dazibao_TLV : Un TLV.
	.type 	: (T)ype
	.length : (L)ongueur
	.value 	: (V)aleur
*/
typedef struct {
	Dazibao_TLV_Type type;
	off_t position;
	int length;
	void *value;
} Dazibao_TLV;



/*
	Structure Dazibao_TLV_Compound_Value
	.tlv_count : Le nombre de TLV que contient ce composé.
	.content : l'array des TLV composants.
*/
typedef struct {
	int tlv_count;
	Dazibao_TLV *content;
} Dazibao_TLV_Compound_Value;



/*
	Structure Dazibao_TLV_Compound_Value
	.timestamp : Date au format timestamp.
	.content : TLV composant.
*/
typedef struct {
	long timestamp;
	Dazibao_TLV *content;
} Dazibao_TLV_Dated_Value;



typedef struct {
	int file_desciptor;
	int tlv_count;
	Dazibao_TLV *content;
} Dazibao;



typedef struct {
	char version;
	char magic_number;
	char extensions[2];
} Dazibao_Header;



#endif
