#ifndef DAZIBAO_COMPACTION_H_
#define DAZIBAO_COMPACTION_H_

#include "dazibao_utilities.h"
#include "dazibao_safe.h"

void callDeletePad(int f, char *file);

int deletePad(int f, int sizeLoop, int val);

#endif
