
// Copyright 2023 Contributors to the Veraison project.
// SPDX-License-Identifier: Apache-2.0

#include "ear.h"
#include "ear_priv.h"
#include "unity.h"
#include <stdlib.h>

void setUp(void) {}
void tearDown(void) {}

uint8_t pkey[] = {
    0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x20, 0x50,
    0x55, 0x42, 0x4c, 0x49, 0x43, 0x20, 0x4b, 0x45, 0x59, 0x2d, 0x2d, 0x2d,
    0x2d, 0x2d, 0x0a, 0x4d, 0x46, 0x6b, 0x77, 0x45, 0x77, 0x59, 0x48, 0x4b,
    0x6f, 0x5a, 0x49, 0x7a, 0x6a, 0x30, 0x43, 0x41, 0x51, 0x59, 0x49, 0x4b,
    0x6f, 0x5a, 0x49, 0x7a, 0x6a, 0x30, 0x44, 0x41, 0x51, 0x63, 0x44, 0x51,
    0x67, 0x41, 0x45, 0x75, 0x73, 0x57, 0x78, 0x48, 0x4b, 0x32, 0x50, 0x6d,
    0x66, 0x6e, 0x48, 0x4b, 0x77, 0x58, 0x50, 0x53, 0x35, 0x34, 0x6d, 0x30,
    0x6b, 0x54, 0x63, 0x47, 0x4a, 0x39, 0x30, 0x0a, 0x55, 0x69, 0x67, 0x6c,
    0x57, 0x69, 0x47, 0x61, 0x68, 0x74, 0x61, 0x67, 0x6e, 0x76, 0x38, 0x67,
    0x45, 0x34, 0x76, 0x34, 0x4c, 0x63, 0x47, 0x32, 0x31, 0x57, 0x4b, 0x2b,
    0x44, 0x36, 0x56, 0x4b, 0x74, 0x34, 0x42, 0x4b, 0x4f, 0x6d, 0x53, 0x32,
    0x31, 0x79, 0x7a, 0x50, 0x37, 0x57, 0x74, 0x76, 0x74, 0x75, 0x30, 0x6f,
    0x75, 0x2f, 0x77, 0x52, 0x66, 0x67, 0x3d, 0x3d, 0x0a, 0x2d, 0x2d, 0x2d,
    0x2d, 0x2d, 0x45, 0x4e, 0x44, 0x20, 0x50, 0x55, 0x42, 0x4c, 0x49, 0x43,
    0x20, 0x4b, 0x45, 0x59, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x0a};

size_t pkey_sz = sizeof pkey;

const char *valid_ear =
    "eyJhbGciOiJFUzI1NiIsInR5cCI6IkpXVCJ9"
    "."
    "eyJlYXIucmF3LWV2aWRlbmNlIjoiTnpRM01qWTVOek0yTlRZek56UUsiLCJlYXIudmVyaWZp"
    "ZXItaWQiOnsiYnVpbGQiOiJ2dHMgMC4wLjEiLCJkZXZlbG9wZXIiOiJodHRwczovL3ZlcmFp"
    "c29uLXByb2plY3Qub3JnIn0sImVhdF9wcm9maWxlIjoidGFnOmdpdGh1Yi5jb20sMjAyMzp2"
    "ZXJhaXNvbi9lYXIiLCJpYXQiOjEuNjY2NTI5MTg0ZSswOSwianRpIjoiNTViOGIzZmFkOGRk"
    "MWQ4ZWFjNGU0OGYxMTdmZTUwOGIxMWY4NDRkOWYwMTg5YmZlZDliODc1MTVhNjc1NDI2NCIs"
    "Im5iZiI6MTY3NzI0Nzg3OSwic3VibW9kcyI6eyJQQVJTRUNfVFBNIjp7ImVhci5hcHByYWlz"
    "YWwtcG9saWN5LWlkIjoiaHR0cHM6Ly92ZXJhaXNvbi5leGFtcGxlL3BvbGljeS8xLzYwYTAw"
    "NjhkIiwiZWFyLnN0YXR1cyI6ImFmZmlybWluZyIsImVhci50cnVzdHdvcnRoaW5lc3MtdmVj"
    "dG9yIjp7ImV4ZWN1dGFibGVzIjoyLCJoYXJkd2FyZSI6MiwiaW5zdGFuY2UtaWRlbnRpdHki"
    "OjJ9LCJlYXIudmVyYWlzb24ua2V5LWF0dGVzdGF0aW9uIjp7ImFrcHViIjoiTUZrd0V3WUhL"
    "b1pJemowQ0FRWUlLb1pJemowREFRY0RRZ0FFY2pTcDhfTVdNM2d5OFR1Z1dPMVRwUVNqX3ZJ"
    "a3NMcEMtZzhsNVMzbHBHYjdQV1dHb0NBakVQOF9BNTlWWndMWGd3b1p6TjBXeHVCUGpwYVdp"
    "V3NmQ1EifX19fQ"
    "."
    "3Ym-f1LEgamxePUM7h6Y2RJDGh9eeL0xKor0n1wE9jdAnLNwm3rTKFV2S2LbqVFoDtK9QGal"
    "T2t5RnUdfwZNmg";

const uint8_t parsec_tpm_akpub[] = {
    0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02,
    0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03,
    0x42, 0x00, 0x04, 0x72, 0x34, 0xa9, 0xf3, 0xf3, 0x16, 0x33, 0x78, 0x32,
    0xf1, 0x3b, 0xa0, 0x58, 0xed, 0x53, 0xa5, 0x04, 0xa3, 0xfe, 0xf2, 0x24,
    0xb0, 0xba, 0x42, 0xfa, 0x0f, 0x25, 0xe5, 0x2d, 0xe5, 0xa4, 0x66, 0xfb,
    0x3d, 0x65, 0x86, 0xa0, 0x20, 0x23, 0x10, 0xff, 0x3f, 0x03, 0x9f, 0x55,
    0x67, 0x02, 0xd7, 0x83, 0x0a, 0x19, 0xcc, 0xdd, 0x16, 0xc6, 0xe0, 0x4f,
    0x8e, 0x96, 0x96, 0x89, 0x6b, 0x1f, 0x09};

void test_jwt_verify_valid_ear(void) {
  ear_t *ear;
  int ret = ear_jwt_verify(valid_ear, pkey, pkey_sz, "ES256", &ear, NULL);
  TEST_ASSERT(ret == 0);

  ear_free(ear);
}

void test_get_status_affirming(void) {
  ear_t *ear;
  int ret = ear_jwt_verify(valid_ear, pkey, pkey_sz, "ES256", &ear, NULL);
  TEST_ASSERT(ret == 0);

  ear_tier_t tier;

  ret = ear_get_status(ear, "PARSEC_TPM", &tier, NULL);
  TEST_ASSERT(ret == 0);
  TEST_ASSERT_EQUAL_INT(EAR_TIER_AFFIRMING, tier);

  ear_free(ear);
}

void test_veraison_get_akpub(void) {
  ear_t *ear;
  int ret = ear_jwt_verify(valid_ear, pkey, pkey_sz, "ES256", &ear, NULL);
  TEST_ASSERT(ret == 0);

  uint8_t *akpub;
  size_t akpub_sz;

  ret = ear_veraison_get_akpub(ear, "PARSEC_TPM", &akpub, &akpub_sz, NULL);
  TEST_ASSERT(ret == 0);
  TEST_ASSERT_EQUAL_size_t(sizeof parsec_tpm_akpub, akpub_sz);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(parsec_tpm_akpub, akpub,
                                sizeof parsec_tpm_akpub);

  free(akpub);
  ear_free(ear);
}

void test_get_app_recs(void) {
  ear_t *ear;
  const char **app_rec_list;
  size_t app_rec_count;
  int ret = ear_jwt_verify(valid_ear, pkey, pkey_sz, "ES256", &ear, NULL);
  TEST_ASSERT(ret == 0);

  ret = ear_get_app_recs(ear, &app_rec_list, &app_rec_count);
  TEST_ASSERT(ret == 0);
  TEST_ASSERT_NOT_NULL(app_rec_list);
  TEST_ASSERT_EQUAL_size_t(1, app_rec_count);
  TEST_ASSERT_EQUAL_STRING("PARSEC_TPM", app_rec_list[0]);

  free(app_rec_list);
  ear_free(ear);
}

// Output goes to ${BUILD_DIR}/Testing/Temporary/LastTest.log
static void DBG_print_buf(const uint8_t *b, size_t b_sz) {
  (void)printf("%p[%zu]:\n", b, b_sz);

  if (b == NULL || b_sz == 0)
    return;

  for (size_t i = 1; i <= b_sz; i++) {
    (void)printf("%02x", b[i - 1]);

    if (i % 15 == 0)
      puts("");
  }

  puts("");
}

#define FRESET(p)                                                              \
  do {                                                                         \
    if (p)                                                                     \
      free(p);                                                                 \
    p = NULL;                                                                  \
  } while (0);

void test_b64(void) {
  uint8_t *b = NULL;
  size_t b_sz;

  struct tc {
    const char *tv;
    struct {
      int ret;
      size_t sz;
      const uint8_t *val;
    } exp;
  } tcs[] = {
      // negatives
      {"", .exp = {.ret = -1}},
      {"*", .exp = {.ret = -1}},
      {"====", .exp = {.ret = -1}},
      {"====_w", .exp = {.ret = -1}},
      // positives
      {"_w", .exp = {.ret = 0, .val = NULL, .sz = 1}},
      {"Yw==", .exp = {.ret = 0, .val = NULL, .sz = 1}},
      {"Yw", .exp = {.ret = 0, .val = NULL, .sz = 1}},
      {"Y2k", .exp = {.ret = 0, .val = NULL, .sz = 2}},
      {"Y2lh", .exp = {.ret = 0, .val = NULL, .sz = 3}},
      {"Y2lhbw", .exp = {.ret = 0, .val = NULL, .sz = 4}},
      {"MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEcjSp8_MWM3gy8TugWO1TpQSj_vIksLpC-"
       "g8l5S3lpGb7PWWGoCAjEP8_A59VZwLXgwoZzN0WxuBPjpaWiWsfCQ",
       .exp =
           {
               .ret = 0,
               .val = parsec_tpm_akpub,
               .sz = sizeof parsec_tpm_akpub,
           }},
      // borderline
      {"_w==(())", .exp = {.ret = 0, .sz = 1}},
  };

  for (size_t i = 0; i < sizeof tcs / sizeof(struct tc); i++) {
    int ret = u_b64url_decode(tcs[i].tv, &b, &b_sz);
    TEST_ASSERT_EQUAL_INT(tcs[i].exp.ret, ret);

    if (tcs[i].exp.ret == 0) {
      DBG_print_buf(b, b_sz);
      TEST_ASSERT_EQUAL_size_t(tcs[i].exp.sz, b_sz);
      if (tcs[i].exp.val != NULL)
        TEST_ASSERT_EQUAL_UINT8_ARRAY(tcs[i].exp.val, b, b_sz);
    }

    FRESET(b);
    b_sz = 0;
  }
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_jwt_verify_valid_ear);
  RUN_TEST(test_get_status_affirming);
  RUN_TEST(test_veraison_get_akpub);
  RUN_TEST(test_get_app_recs);
  RUN_TEST(test_b64);
  return UNITY_END();
}
