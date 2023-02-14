// Copyright 2023 Contributors to the Veraison project.
// SPDX-License-Identifier: Apache-2.0

#include "ear.h"
#include <err.h>
#include <errno.h>
#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct args_s {
  char key_fn[1024];
  char alg[16];
  char ear_fn[1024];
} args_t;

// from utils.c
extern size_t u_strlcpy(char *dst, const char *src, size_t sz);
const char *to_cstr(const uint8_t *b, size_t b_sz);
void parse_opts(int ac, char **av, args_t *pargs);
void read_from_file(const char *fn, uint8_t **pb, size_t *pb_sz);
void usage(const char *name);

int main(int argc, char *argv[]) {
  args_t args = {{'\0'}, {'\0'}, {'\0'}};
  uint8_t *key, *ear_jwt;
  size_t key_sz, ear_jwt_sz;
  ear_t *ear;
  char err_msg[EAR_ERR_SZ];

  parse_opts(argc, argv, &args);

  read_from_file(args.key_fn, &key, &key_sz);
  read_from_file(args.ear_fn, &ear_jwt, &ear_jwt_sz);

  if (ear_jwt_verify(to_cstr(ear_jwt, ear_jwt_sz), key, key_sz, &ear,
                     err_msg) != 0) {
    err(EXIT_FAILURE, "failed to verify EAR: %s", err_msg);
  }

  puts("EAR verified");

  const char *status;
  if ((status = ear_get_status(ear, err_msg)) == NULL) {
    err(EXIT_FAILURE, "failed to retrieve EAR status: %s", err_msg);
  }

  puts(status);

  return 0;
}

void read_from_file(const char *fn, uint8_t **pb, size_t *pb_sz) {
  long sz;
  uint8_t *b;
  FILE *fp;

  if ((fp = fopen(fn, "rb")) == NULL)
    err(EXIT_FAILURE, "%s", fn);

  if (fseek(fp, 0L, SEEK_END) == -1)
    err(EXIT_FAILURE, "seeking into %s", fn);

  /* Get file length. */
  sz = ftell(fp);
  rewind(fp);

  if ((b = malloc(sz)) == NULL)
    err(EXIT_FAILURE, "getting memory for reading from %s", fn);

  if (fread(b, sz, 1, fp) != 1)
    err(EXIT_FAILURE, "reading from %s", fn);

  (void)fclose(fp);

  *pb = b;
  *pb_sz = sz;

  return;
}

void usage(const char *name) {
  const char *fmt =
      "\nUsage: %s [opts] <ear_jwt>\n\n"
      "    where \'ear_jwt\' is a EAR in JWT format, and \'opts\' is:\n\n"
      "  -k KEY  The key to use for verification\n"
      "  -a ALG  The algorithm to use for verification\n";

  (void)fprintf(stderr, fmt, name);

  exit(EXIT_FAILURE);
}

void parse_opts(int ac, char **av, args_t *pargs) {
  int c;

  while ((c = getopt(ac, av, "a:k:")) != -1) {
    switch (c) {
    case 'k':
      u_strlcpy(pargs->key_fn, optarg, sizeof pargs->key_fn);
      break;
    case 'a':
      u_strlcpy(pargs->alg, optarg, sizeof pargs->alg);
      break;
    default:
      usage(av[0]);
    }
  }

  if (optind == ac) {
    usage(av[0]);
  }

  u_strlcpy(pargs->ear_fn, av[optind], sizeof pargs->ear_fn);

  return;
}

const char *to_cstr(const uint8_t *b, size_t b_sz) {
  char *s = calloc(1, b_sz + 1);

  memcpy(s, b, b_sz);
  s[b_sz] = '\0';

  return s;
}
