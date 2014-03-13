
// This is partially copied from Linus Torvalds GIT distribution
// Not that this is a specifically original piece of code
// but all the best to him for his very usefull operating
// system Linux and his equally usefull distributed content
// managament system Git

#include "hex.h"

static const signed char hexval[256] = {
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 00-07 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 08-0f */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 10-17 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 18-1f */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 20-27 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 28-2f */
  0,  1,  2,  3,  4,  5,  6,  7,		/* 30-37 */
  8,  9, -1, -1, -1, -1, -1, -1,		/* 38-3f */
  -1, 10, 11, 12, 13, 14, 15, -1,		/* 40-47 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 48-4f */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 50-57 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 58-5f */
  -1, 10, 11, 12, 13, 14, 15, -1,		/* 60-67 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 68-67 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 70-77 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 78-7f */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 80-87 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 88-8f */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 90-97 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* 98-9f */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* a0-a7 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* a8-af */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* b0-b7 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* b8-bf */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* c0-c7 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* c8-cf */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* d0-d7 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* d8-df */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* e0-e7 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* e8-ef */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* f0-f7 */
  -1, -1, -1, -1, -1, -1, -1, -1,		/* f8-ff */
};

int get_dsfmt_hex(const char *hex, dsfmt_t *dsfmtp)
{
  int i;
  unsigned char *dsf = (unsigned char *)dsfmtp;
  for (i = 0; i < sizeof(dsfmt_t); i++) {
    unsigned int val;
    /*
     * hex[1]=='\0' is caught when val is checked below,
     * but if hex[0] is NUL we have to avoid reading
     * past the end of the string:
     */

    if (!hex[0])
      return -1;
    val = (hexval[hex[0]] << 4) | hexval[hex[1]];
    if (val & ~0xff)
      return -1;
    *dsf++ = val;
    hex += 2;
  }
  return 0;
}

char *dsfmt_to_hex(const dsfmt_t *dsfmtp)
{
  static int bufno;
  static char hexbuffer[4][2*sizeof(dsfmt_t) + 1];
  static const char hex[] = "0123456789abcdef";
  unsigned char *dsf = (unsigned char *)dsfmtp;
  char *buffer = hexbuffer[3 & ++bufno], *buf = buffer;
  int i;

  for (i = 0; i < sizeof(dsfmt_t); i++) {
    unsigned int val = *dsf++;
    *buf++ = hex[val >> 4];
    *buf++ = hex[val & 0xf];
  }
  *buf = '\0';

  return buffer;
}

int get_saved_dsfmt(const char*filename, dsfmt_t *dsf) {
  FILE *temp = fopen(filename, "r");
  int retval = !(temp);
  if (temp) {
    char buf[2*sizeof(dsfmt_t)+1];
    fgets(buf, 2*sizeof(dsfmt_t) + 1, temp);
    retval = get_dsfmt_hex(buf, dsf);
    fclose(temp);
  }

  return retval;
}

void write_dsfmt_file(dsfmt_t *dsf, FILE *towhere) {
  char *buf = dsfmt_to_hex(dsf);
  fputs(buf, towhere);
  putc('\n', towhere);
}
