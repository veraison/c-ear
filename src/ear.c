// Copyright 2023 Contributors to the Veraison project.
// SPDX-License-Identifier: Apache-2.0

#include "ear.h"
#include "ear_priv.h"
#include <assert.h>
#include <jwt.h>
#include <stdlib.h>
#include <string.h>

#define EAR_PROFILE "tag:github.com,2023:veraison/ear"

static json_t *get_submods(ear_t *ear, char err_msg[EAR_ERR_SZ]);
static int tier_from_string(const char *tier, ear_tier_t *ptier);
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
  assert(ear_jwt != NULL);
  assert(pkey != NULL);
  assert(pkey_sz > 0);
  assert(alg != NULL);
  assert(pear != NULL);

  int ret = 0;
  jwt_valid_t *jwt_valid = NULL;
  ear_t *ear = NULL;
  jwt_alg_t opt_alg;
  char e[EAR_ERR_SZ] = {'\0'};

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

  jwt_valid_free(jwt_valid), jwt_valid = NULL;

  const char *eat_profile = jwt_get_grant(ear->jwt, "eat_profile");

  if (eat_profile == NULL) {
    (void)snprintf(e, sizeof e, "missing mandatory eat_profile");
    goto err;
  }

  if (strcmp(eat_profile, EAR_PROFILE)) {
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

int ear_get_status(ear_t *ear, const char *app_rec, ear_tier_t *ptier,
                   char err_msg[EAR_ERR_SZ]) {
  assert(ear != NULL);
  assert(ear->jwt != NULL);
  assert(app_rec != NULL);
  assert(ptier != NULL);

  char e[EAR_ERR_SZ] = {'\0'};
  json_t *submods = NULL, *submod = NULL, *status = NULL;

  if ((submods = get_submods(ear, e)) == NULL) {
    goto err;
  }

  if ((submod = json_object_get(submods, app_rec)) == NULL) {
    (void)snprintf(e, sizeof e, "no appraisal record found for \"%s\"",
                   app_rec);
    goto err;
  }

  status = json_object_get(submod, "ear.status");
  if (!json_is_string(status)) {
    (void)snprintf(e, sizeof e, "\"ear.status\" not found");
    goto err;
  }

  const char *status_s = json_string_value(status);

  if (tier_from_string(status_s, ptier) == -1) {
    (void)snprintf(e, sizeof e, "unknown status \"%s\"", status_s);
    goto err;
  }

  json_decref(submods), submods = NULL;

  return 0;

err:
  if (err_msg != NULL)
    (void)u_strlcpy(err_msg, e, EAR_ERR_SZ);

  if (submods)
    json_decref(submods);

  return -1;
}

int ear_veraison_get_akpub(ear_t *ear, const char *app_rec, uint8_t **pakpub,
                           size_t *pakpub_sz, char err_msg[EAR_ERR_SZ]) {
  assert(ear != NULL);
  assert(ear->jwt != NULL);
  assert(app_rec != NULL);
  assert(pakpub != NULL);
  assert(pakpub_sz != NULL);

  char e[EAR_ERR_SZ] = {'\0'};
  json_t *submods = NULL, *submod = NULL, *key_attestation = NULL,
         *akpub = NULL;

  if ((submods = get_submods(ear, e)) == NULL) {
    goto err;
  }

  if ((submod = json_object_get(submods, app_rec)) == NULL) {
    (void)snprintf(e, sizeof e, "no appraisal record found for \"%s\"",
                   app_rec);
    goto err;
  }

  key_attestation = json_object_get(submod, "ear.veraison.key-attestation");
  if (key_attestation == NULL) {
    (void)snprintf(e, sizeof e, "\"ear.veraison.key-attestation\" not found");
    goto err;
  }

  akpub = json_object_get(key_attestation, "akpub");
  if (!json_is_string(akpub)) {
    (void)snprintf(e, sizeof e, "\"akpub\" not found");
    goto err;
  }

  const char *akpub_s = json_string_value(akpub);

  if (u_b64url_decode(akpub_s, pakpub, pakpub_sz) == -1) {
    (void)snprintf(e, sizeof e, "base64 decoding of \"akpub\" failed");
    goto err;
  }

  json_decref(submods);

  return 0;

err:
  if (submods)
    json_decref(submods);

  if (err_msg != NULL)
    (void)u_strlcpy(err_msg, e, EAR_ERR_SZ);

  return -1;
}

static json_t *get_submods(ear_t *ear, char err_msg[EAR_ERR_SZ]) {
  assert(ear != NULL);
  assert(ear->jwt != NULL);

  char e[EAR_ERR_SZ] = {'\0'};
  char *submods_js = NULL;
  json_t *submods = NULL;

  if ((submods_js = jwt_get_grants_json(ear->jwt, "submods")) == NULL) {
    (void)snprintf(e, sizeof e, "\"submods\" not found");
    goto err;
  }

  if ((submods = json_loads(submods_js, 0, NULL)) == NULL) {
    (void)snprintf(e, sizeof e, "\"submods\" does not contain valid JSON");
    goto err;
  }

  free(submods_js), submods_js = NULL;

  return submods;

err:
  if (submods_js != NULL)
    free(submods_js);

  if (submods)
    json_decref(submods);

  if (err_msg != NULL)
    (void)u_strlcpy(err_msg, e, EAR_ERR_SZ);

  return NULL;
}

static int tier_from_string(const char *tier, ear_tier_t *ptier) {
  struct tiers_map {
    const char *s;
    ear_tier_t e;
  } tiers[] = {
      {"affirming", EAR_TIER_AFFIRMING},
      {"contraindicated", EAR_TIER_CONTRAINDICATED},
      {"warning", EAR_TIER_WARNING},
      {"none", EAR_TIER_NONE},
  };

  for (unsigned i = 0; i < sizeof tiers / sizeof(struct tiers_map); i++) {
    if (!strcmp(tier, tiers[i].s)) {
      *ptier = tiers[i].e;
      return 0;
    }
  }

  return -1;
}
