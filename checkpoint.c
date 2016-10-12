
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <openssl/sha.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "hex.h"
#include "checkpoint.h"
#include "simulazione.h"
#include "infofile/infofile.h"

static char *filename;

//
// Add info (time and filename and crc sum) in .info file
// make filename a binary file with random number state
// xyz polymer state and laplacian
// close with exit 99
//
// When jobs start, check if info file is there
// if it's there and contains valid checkpoint information
// start with it, if it's there but it is corrupted fail
// corrupted includes 1) inconsistent with command line
// 2) without checkpoint information
// 3) with corrupted checkpoint information
// if it is not there, start a new job
//

// First time you should init_checkpoint
int prepare_checkpoint(unsigned int accepted,
		       unsigned int total,
		       unsigned long long int toprint) {
  // block SIGUSR1 signal
#if NUM_THREADS > 1
  // Something smart should be there
#else
  sigset_t signal_set;
  sigemptyset(&signal_set); sigaddset(&signal_set, SIGUSR1);
  sigprocmask(SIG_BLOCK, &signal_set, NULL);
#endif

  void *dest = checkpoint;
  // Buffer
  memcpy(dest, buffer, buffer_size); dest += buffer_size;
  // Global state
  memcpy(dest, &mc_time, sizeof(mc_time)); dest += sizeof(mc_time);
  // Random state
  memcpy(dest, &dsfmt, sizeof(dsfmt_t)); dest += sizeof(dsfmt_t);
  // Local variables
  *(unsigned int *)dest = accepted; dest += sizeof(accepted);
  *(unsigned int *)dest = total; dest += sizeof(total);
  *(unsigned long long int *)dest = toprint; dest += sizeof(toprint);

#if NUM_THREADS > 1
  // Here too
#else
  sigprocmask(SIG_UNBLOCK, &signal_set, NULL);
#endif
  return 0;
}

// Before you should init_checkpoint
int load_checkpoint(const unsigned char* hash,
		    unsigned int *accepted,
		    unsigned int *total,
		    unsigned long long int *toprint) {
  // block SIGUSR1 signal
#if NUM_THREADS > 1
  // Something smart should be there
#else
  sigset_t signal_set;
  sigemptyset(&signal_set); sigaddset(&signal_set, SIGUSR1);
  sigprocmask(SIG_BLOCK, &signal_set, NULL);
#endif

  int fd = open(filename, O_RDONLY);
  struct stat buf;
  fstat(fd, &buf);
  if (checkpoint_size != buf.st_size) {
    fprintf(stderr, "Checkpoint size and file doesn't match\n");
    exit(-9);
  }

  read(fd, checkpoint, checkpoint_size);
  close(fd);

  // Hash it
  unsigned char hash_cmp[SHA_DIGEST_LENGTH];     // == 20
  char checksum[sizeof(char) * (SHA_DIGEST_LENGTH * 2 + 1)];  // Hexadecimal
  SHA_CTX context;
  SHA1_Init(&context);
  SHA1_Update(&context, checkpoint, checkpoint_size);
  SHA1_Final(hash_cmp, &context);

  bin2hex(checksum, hash_cmp, sizeof(hash_cmp));

  if (strcmp(checksum, hash) != 0) {
    fprintf(stderr, "Checkpoint hash doesn't match: %s %s\n", checksum, hash);
    exit(-10);
  };

  // Parse it
  void *source = checkpoint;
  // Buffer
  memcpy(buffer, source, buffer_size); source += buffer_size;
  // Global state
  memcpy(&mc_time, source, sizeof(mc_time)); source += sizeof(mc_time);
  // Random state
  memcpy(&dsfmt, source, sizeof(dsfmt_t)); source += sizeof(dsfmt_t);
  // Local variables
  *accepted = *(unsigned int *)source; source += sizeof(*accepted);
  *total = *(unsigned int *)source; source += sizeof(*total);
  *toprint = *(unsigned long long int *)source; source += sizeof(*toprint);

  struct data_infofile d; d.time = time(NULL); d.type = is_time_infofile; append_infofile(infos, "RESUME", d);

#if NUM_THREADS > 1
  // Here too
#else
  sigprocmask(SIG_UNBLOCK, &signal_set, NULL);
#endif
  return 0;
}

static
void print_checkpointinfo(char *word, char *filename, char *checksum) {
  time_t tstamp = time(NULL);

  struct data_infofile info;
  info.type = is_list_infofile;
  info.list                 = malloc(sizeof(struct list_infofile));
  info.list -> next         = malloc(sizeof(struct list_infofile));
  info.list -> next -> next = malloc(sizeof(struct list_infofile));

  info.list -> data.s                    = filename;
  info.list -> next -> data.s            = checksum;
  info.list -> next -> next -> data.time = tstamp;

  info.list -> data.type = info.list -> next -> data.type = is_s_infofile;
  info.list -> next -> next -> data.type = is_time_infofile;

  append_infofile(infos, word, info);
}

int make_checkpoint() {

  // Write to file
  int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  write(fd, checkpoint, checkpoint_size);
  close(fd);

  // Hash it
  unsigned char hash[SHA_DIGEST_LENGTH];     // == 20
  char checksum[sizeof(char) * (SHA_DIGEST_LENGTH * 2 + 1)];  // Hexadecimal
  SHA_CTX context;
  SHA1_Init(&context);
  SHA1_Update(&context, checkpoint, checkpoint_size);
  SHA1_Final(hash, &context);

  bin2hex(checksum, hash, sizeof(hash));

  print_checkpointinfo("CHECKPOINT", filename, checksum);

  return 0;
}

int init_checkpoint(const char *fname,
		    unsigned int accepted,
		    unsigned int total,
		    unsigned long long int toprint) {

  filename = malloc(sizeof(char) * (strlen(fname) + 1));
  strcpy(filename, fname);

  checkpoint_size =
    // Buffer
    buffer_size + 
    // Global state
    sizeof(struct mc_time_t) +
    // Random state
    sizeof(dsfmt_t) +
    // Local variables
    sizeof(accepted) + sizeof(total) + sizeof(toprint)
    // 
    ;
  if(posix_memalign((void **)&checkpoint, 32,
		    checkpoint_size)) {
    fprintf(stderr, "Error allocating memory for checkpointing\n");
    exit(-8);
  }
  prepare_checkpoint(accepted, total, toprint);
  // Activate checkpoint signal
#if NUM_THREADS > 1
  // Something smart should be there
#else
  {
    sigset_t signal_set;
    sigemptyset(&signal_set); sigaddset(&signal_set, SIGUSR1);
    sigprocmask(SIG_UNBLOCK, &signal_set, NULL);
  }
#endif
}
