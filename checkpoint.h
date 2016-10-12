#ifndef CHECKPOINT_H
#define CHECKPOINT_H

size_t checkpoint_size;
__attribute__ ((aligned (32))) void *restrict checkpoint;

int prepare_checkpoint(unsigned int accepted,
		       unsigned int total,
		       unsigned long long int toprint);\

int make_checkpoint();

int load_checkpoint(const unsigned char* hash,
		    unsigned int *accepted,
		    unsigned int *total,
		    unsigned long long int *toprint);

int init_checkpoint(const char *fname,
		    unsigned int accepted,
		    unsigned int total,
		    unsigned long long int toprint);


#endif
