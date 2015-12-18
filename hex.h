#ifndef HEX_H
#define HEX_H

#include "dSFMT-src-2.2.1/dSFMT.h"

int get_saved_dsfmt(const char*filename, dsfmt_t *dsf);
void write_dsfmt_file(dsfmt_t *dsf, FILE *towhere);

#endif
