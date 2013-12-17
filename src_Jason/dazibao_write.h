#ifndef DAZIBAO_WRITE_H_
#define DAZIBAO_WRITE_H_

#include "dazibao.h"

/* Crée un nouveau fichier Dazibao */
Dazibao *dazibao_create_file(Dazibao *dazibao);

/* Ajoute un TLV à la fin du Dazibao */
bool dazibao_append_tlv(Dazibao *dazibao, Dazibao_TLV *tlv);

/* Met à jour un TLV dans un Dazibao */
bool dazibao_update_tlv(Dazibao *dazibao, Dazibao_TLV *tlv);

#endif
