#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "infofile.h"
#include "parse.h"

void main() {
  struct records_infofile *test;
  int error;
  test = parse_infofile(stdin, &error);

  struct infofile info;
  info.file = stdout;
  info.content = NULL;

  while (test) {
    append_infofile(&info, test -> what,
		    test -> data);
    test = test -> next;
  }

  printf("%d\n", error);
}
