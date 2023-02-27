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
char *to_cstr(const uint8_t *b, size_t b_sz);
void parse_opts(int ac, char **av, args_t *pargs);
int read_from_file(const char *fn, uint8_t **pb, size_t *pb_sz);
void usage(const char *name);

int main(int argc, char *argv[]) {
  args_t args = {{'\0'}, {'\0'}, {'\0'}};
  uint8_t *key = NULL, *ear_jwt = NULL;
  size_t key_sz, ear_jwt_sz;
  ear_t *ear = NULL;
  char *ear_jwt_cstr = NULL;
  char err_msg[EAR_ERR_SZ];

  parse_opts(argc, argv, &args);

  if (read_from_file(args.key_fn, &key, &key_sz) == -1) {
    warn("error reading key from %s", args.key_fn);
    goto err;
  }

  if (read_from_file(args.ear_fn, &ear_jwt, &ear_jwt_sz) == -1) {
    warn("error reading EAR JWT from %s", args.ear_fn);
    goto err;
  }

  if ((ear_jwt_cstr = to_cstr(ear_jwt, ear_jwt_sz)) == NULL) {
    warn("allocating EAR JWT string");
    goto err;
  }

  if (ear_jwt_verify(ear_jwt_cstr, key, key_sz, args.alg, &ear, err_msg) != 0) {
    warnx("failed to verify EAR: %s", err_msg);
    goto err;
  }

  free(ear_jwt), ear_jwt = NULL;
  free(key), key = NULL;
  free(ear_jwt_cstr), ear_jwt_cstr = NULL;

  puts("EAR verified");

  ear_tier_t tier;
  const char *app_rec = "PARSEC_TPM";

  if (ear_get_status(ear, app_rec, &tier, err_msg) == -1) {
    warnx("failed to retrieve EAR status for %s appraisal: %s", app_rec,
          err_msg);
    goto err;
  }

  printf("%s status: tier(%d)\n", app_rec, tier);

  ear_free(ear), ear = NULL;

  return 0;

err:
  if (key)
    free(key);
  if (ear_jwt)
    free(ear_jwt);
  if (ear_jwt_cstr)
    free(ear_jwt_cstr);
  if (ear)
    ear_free(ear);

  return -1;
}

int read_from_file(const char *fn, uint8_t **pb, size_t *pb_sz) {
  long sz;
  uint8_t *b = NULL;
  FILE *fp = NULL;

  if ((fp = fopen(fn, "rb")) == NULL || (fseek(fp, 0L, SEEK_END) == -1)) {
    goto err;
  }

  /* Get file length. */
  sz = ftell(fp);
  rewind(fp);

  if ((b = malloc(sz)) == NULL) {
    goto err;
  }

  if (fread(b, sz, 1, fp) != 1) {
    goto err;
  }

  (void)fclose(fp), fp = NULL;

  *pb = b;
  *pb_sz = sz;

  return 0;

err:
  if (fp != NULL)
    (void)fclose(fp);
  if (b != NULL)
    free(b);

  return -1;
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

char *to_cstr(const uint8_t *b, size_t b_sz) {
  char *s = calloc(1, b_sz + 1);

  memcpy(s, b, b_sz);
  s[b_sz] = '\0';

  return s;
}
