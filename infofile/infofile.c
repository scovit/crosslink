#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "infofile.h"
#include "parse.h"

static
struct infofile *create_infofile_r(const char *filename, FILE *file) {
  size_t length = strlen(filename);

  struct infofile *retval = 
    malloc(sizeof(struct infofile) + sizeof(char) * (length + 1));

  if (retval) {
    retval -> filename = (char *)(retval + 1);
    strcpy(retval -> filename, filename);
    retval -> file = file;
    retval -> content = NULL;
  }

  return retval;
}

struct infofile *open_infofile(const char *filename) {
  struct infofile *retval = NULL;
  FILE *file = fopen(filename, "r+");

  if (file && (retval = create_infofile_r(filename, file))) {
    int err = 0;

    retval -> content = parse_infofile(file, &err);

    if (err) {
      close_infofile(retval);
      retval = NULL;
    }
  }
  return retval;
}

struct infofile *create_infofile(const char *filename) {
  FILE *file = fopen(filename, "w");

  if (file)
    return create_infofile_r(filename, file);
  else
    return NULL;
}

struct data_infofile get_infofile(struct infofile *info, const char *what,
				  int n) {
  struct records_infofile *curr = info -> content;
  int i = 0;

  if (n < 0) {
    while (curr) {
      if (!strcmp(curr -> what, what))
	i++;
      curr = curr -> next;
    }
    n = i + n;
    i = 0;
    curr = info -> content;
  }

  while (curr) {
    if (!strcmp(curr -> what, what)) {
      if (i == n)
	return curr -> data;
      i++;
    }
    curr = curr -> next;
  }

  // Not found
  struct data_infofile retval;
  memset(&retval, 0, sizeof(retval));
  retval.type = is_notfound_infofile;
  return retval;
}

static
void free_list_infofile(struct list_infofile *lista) {
  while (lista) {
    if (lista -> data.type == is_list_infofile)
      free_list_infofile(lista -> data.list);
    struct list_infofile *next = lista -> next;
    free(lista);
    lista = next;
  }
}

void close_infofile(struct infofile *info) {
  fclose(info -> file);

  struct records_infofile *curr = info -> content;
  while (curr) {
    free(curr -> what);
    if (curr -> data.type == is_list_infofile)
      free_list_infofile(curr -> data.list);

    struct records_infofile *next = curr -> next;
    free(curr);
    curr = next;
  }

  free(info); // Frees also filename
}

int flush_infofile(struct infofile *info) {
  return fflush(info -> file);
}

static
void print_data(FILE *out, const struct data_infofile data) {
  switch (data.type) {
  case is_s_infofile :
    // This is buggy, does not handle all C strings
    fprintf(out, "s:\"%s\"", data.s);
    break;
  case is_g_infofile:
    fprintf(out, "g:%g", data.g);
    break;
  case is_d_infofile:
    fprintf(out, "d:%d", data.d);
    break;
  case is_u_infofile:
    fprintf(out, "u:%u", data.u);
    break;
  case is_llu_infofile:
    fprintf(out, "llu:%llu", data.llu);
    break;
  case is_time_infofile:
    // This is buggy: at the moment times are added as string
    putc('s', out);
    char *timestr;
    timestr = ctime(&data.time);
    *strchr(timestr, '\n') = '\0';
    fprintf(out, ":\"%s\"", timestr);
    break;
  case is_list_infofile:
    putc('[', out);
    struct list_infofile *curr = data.list;
    while (curr) {
      print_data(out, curr -> data);
      curr = curr -> next;
      if (curr) {
	fputs(", ", out);
      }
    }
    putc(']', out);
    break;

  default:
    fprintf(stderr, "Could not serialize to infofile\n");
    exit(-20);
  }
}

void append_infofile(struct infofile *info, const char *what,
		     const struct data_infofile data) {
  struct records_infofile *curr = info -> content;

  if (!curr)
    curr = malloc(sizeof(struct records_infofile));
  else {
    while (curr->next) curr = curr -> next;
    curr -> next = malloc(sizeof(struct records_infofile));
    curr = curr -> next;
  }

  curr -> what = strdup(what);
  curr -> data = data;
  curr -> next = NULL;

  // Serialize
  fprintf(info -> file, "%s = ", what);
  print_data(info -> file, data);
  putc('\n', info -> file);
}
