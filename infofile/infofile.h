#ifndef INFOFILE_H
#define INFOFILE_H

#include <time.h>
#include <stdio.h>

enum type_infofile {
  is_notfound_infofile,
  is_s_infofile,
  is_g_infofile,
  is_d_infofile, is_u_infofile, is_llu_infofile,
  is_time_infofile,
  is_list_infofile
 };

struct data_infofile {
  union {
    char *s;
    double g;
    int d;
    unsigned int u;
    unsigned long long int llu;
    time_t time;
    struct list_infofile *list;
  };
  int type;
};

struct list_infofile {
  struct data_infofile data;
  struct list_infofile *next;
};

struct records_infofile {
  char *what;
  struct data_infofile data;
  struct records_infofile *next;
};

struct infofile {
  char *filename;
  FILE *file;
  struct records_infofile *content;
};

struct infofile *create_infofile(const char *filename);
struct infofile *open_infofile(const char *filename);

void close_infofile(struct infofile *info);
int flush_infofile(struct infofile *info);

struct data_infofile get_infofile(struct infofile *info, const char *what,
				 const int n);

void append_infofile(struct infofile *info, const char *what,
		      const struct data_infofile data);

#endif

