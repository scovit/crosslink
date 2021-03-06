#ifndef CHECKPOINT_H
#define CHECKPOINT_H

size_t checkpoint_size;
__attribute__ ((aligned (32))) void *restrict checkpoint;

int prepare_checkpoint(unsigned long long int toprint);\

int make_checkpoint();

int load_checkpoint(const char* hash,
		    unsigned long long int *toprint);

int init_checkpoint(const char *fname,
		    unsigned long long int toprint);


#endif
