// Copyright 2023 Contributors to the Veraison project.
// SPDX-License-Identifier: Apache-2.0

#ifndef EAR_H
#define EAR_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef EAR_ERR_SZ
#define EAR_ERR_SZ 128
#endif // !EAR_ERR_SZ

// forward declaration
typedef struct ear_s ear_t;

typedef enum {
  EAR_TIER_NONE,
  EAR_TIER_AFFIRMING,
  EAR_TIER_WARNING,
  EAR_TIER_CONTRAINDICATED
} ear_tier_t;

/**
 * @brief Verify an EAT Attestation Result in JWT format.
 *
 * Verify an EAT Attestation Result in JWT format using the supplied public key.
 * On success, the decoded EAR claims-set is returned.
 *
 * @param[in]   ear_jwt NUL-terminated C string with the JWT carrying the EAR
 *                      claims-set
 * @param[in]   pkey    The public key for verification.  The format is
 *                      described in Section 13 of RFC7468
 * @param[in]   pkey_sz Size in bytes of @p pkey
 * @param[in]   alg     NUL-terminated C string with the JWT algorithm to use
 *                      for verifying the EAR (e.g., "ES256", "RS256")
 * @param[out]  pear    Pointer to a ear_t object which, on success, will be
 *                      populated with the EAR claims-set.  This needs to be
 *                      allocated by the caller
 * @param[out]  err_msg pointer to a pre-allocated buffer (of at least
 *                      @c EAR_ERR_SZ bytes) which, on failure, will be filled
 *                      in by the callee with a human readable error message.
 *                      This can be set to NULL if no extra error reporting is
 *                      required
 *
 * @retval  0   on success
 * @retval  -1  on failure
 */
int ear_jwt_verify(const char *ear_jwt, const uint8_t *pkey, size_t pkey_sz,
                   const char *alg, ear_t **pear, char err_msg[EAR_ERR_SZ]);

/**
 * @brief Return the "ear.status" value of the specified appraisal record
 *
 * On success, the "ear.status" value of the specified appraisal record is
 * returned as a ear_tier_t codepoint.  On error, if supplied, the @p err_msg
 * is filled with the failure reason.
 *
 * @param[in]   ear     an ear_t object returned from a successful invocation of
 *                      ear_jwt_verify
 * @param[in]   app_rec the submod name for the appraisal record
 * @param[out]  ptier   Pointer to a ear_tier_t object which, on success, is
 *                      populated with the status codepoint.  This needs to be
 *                      allocated by the caller
 * @param[out]  err_msg pointer to a pre-allocated buffer (of at least @c
 *                      EAR_ERR_SZ bytes) which, on failure, will be filled in
 *                      by the callee with a human readable error message.  This
 *                      can be set to NULL if no extra error reporting is
 *                      required
 *
 * @retval  0   on success
 * @retval  -1  on failure
 */
int ear_get_status(ear_t *ear, const char *app_rec, ear_tier_t *ptier,
                   char err_msg[EAR_ERR_SZ]);

/**
 * @brief Free an ear_t object allocated by ear_jwt_verify
 *
 * @param ear the ear_t object to free
 */
void ear_free(ear_t *ear);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !EAR_H