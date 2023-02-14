// Copyright 2023 Contributors to the Veraison project.
// SPDX-License-Identifier: Apache-2.0

#include "ear.h"
#include "ear_priv.h"
#include <assert.h>
#include <jwt.h>
#include <stdlib.h>
#include <string.h>

#define EAR_PROFILE_2022 "tag:github.com,2022:veraison/ear"

static ear_t *ear_new() { return (ear_t *)calloc(1, sizeof(ear_t)); }

void ear_free(ear_t *ear) {
  if (ear == NULL)
    return;

  if (ear->jwt != NULL)
    jwt_free(ear->jwt);

  free(ear);
}

int ear_jwt_verify(const char *ear_jwt, const uint8_t *pkey, size_t pkey_sz,
                   const char *alg, ear_t **pear, char err_msg[EAR_ERR_SZ]) {
  int ret = 0;
  jwt_valid_t *jwt_valid = NULL;
  ear_t *ear = NULL;
  jwt_alg_t opt_alg;
  char e[EAR_ERR_SZ] = {'\0'};

  assert(ear_jwt != NULL);
  assert(pkey != NULL);
  assert(pkey_sz > 0);
  assert(alg != NULL);
  assert(pear != NULL);

  opt_alg = jwt_str_alg(alg);
  if (opt_alg == JWT_ALG_INVAL) {
    (void)snprintf(e, sizeof e, "unknown JWT algorithm \"%s\"", alg);
    goto err;
  }

  ret = jwt_valid_new(&jwt_valid, opt_alg);
  if (ret != 0 || jwt_valid == NULL) {
    (void)snprintf(e, sizeof e,
                   "cannot initialise JWT validation object (jwt_valid_new=%d)",
                   ret);
    goto err;
  }

  jwt_valid_set_headers(jwt_valid, 1);
  jwt_valid_set_now(jwt_valid, time(NULL));

  if ((ear = ear_new()) == NULL) {
    (void)snprintf(e, sizeof e, "cannot initialise the EAR object");
    goto err;
  }

  ret = jwt_decode(&ear->jwt, ear_jwt, pkey, pkey_sz);
  if (ret != 0 || ear->jwt == NULL) {
    (void)snprintf(e, sizeof e, "cannot verify EAR JWT (jwt_decode=%d)", ret);
    goto err;
  }

  ret = jwt_validate(ear->jwt, jwt_valid);
  if (ret != 0) {
    (void)snprintf(e, sizeof e, "cannot validate EAR JWT (jwt_validate=%d)",
                   ret);
    goto err;
  }

  const char *eat_profile = jwt_get_grant(ear->jwt, "eat_profile");

  if (eat_profile == NULL) {
    (void)snprintf(e, sizeof e, "missing mandatory eat_profile");
    goto err;
  }

  if (strcmp(eat_profile, EAR_PROFILE_2022)) {
    (void)snprintf(e, sizeof e, "unknown eat_profile \"%s\"", eat_profile);
    goto err;
  }

  *pear = ear;

  return 0;

err:
  if (ear != NULL)
    ear_free(ear);

  if (jwt_valid != NULL)
    jwt_valid_free(jwt_valid);

  if (err_msg != NULL)
    (void)u_strlcpy(err_msg, e, EAR_ERR_SZ);

  return -1;
}

const char *ear_get_status(ear_t *ear, char err_msg[EAR_ERR_SZ]) {
  char e[EAR_ERR_SZ] = {'\0'};
  const char *tiers[] = {"affirming", "contraindicated", "warning", "none"};

  assert(ear != NULL);
  assert(ear->jwt != NULL);

  const char *status = jwt_get_grant(ear->jwt, "ear.status");
  if (status == NULL) {
    (void)snprintf(e, sizeof e, "\"ear.status\" not found");
    goto err;
  }

  for (unsigned i = 0; i < sizeof tiers / sizeof(char *); i++) {
    if (!strcmp(status, tiers[i])) {
      return status;
    }
  }

  (void)snprintf(err_msg, EAR_ERR_SZ, "unknown status \"%s\"", status);

  // fallthrough
err:
  if (err_msg != NULL)
    (void)u_strlcpy(err_msg, e, EAR_ERR_SZ);

  return NULL;
}