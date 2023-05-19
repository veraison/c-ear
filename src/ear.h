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
 *                      populated with the EAR claims-set.
 *                      The object is owned by the caller who needs to take
 *                      care of its disposal using ear_free()
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
 * @brief Output a list of all of the appraisal records in the given EAR.
 *
 * This function can be used to obtain the names of the appraisal records in
 * advance of querying the status or properties of any individual record in
 * the given EAR.
 *
 * The data is returned as an array of NUL-terminated strings, along with the
 * size of the array (which is the number of appraisal records).
 *
 * @param[in]   ear         an ear_t object returned from a successful invocation of
 *                          ear_jwt_verify
 * @param[out]  papp_rec    Upon successful return, receives a freshly-allocated
 *                          array of pointers to constant strings. The caller takes
 *                          ownership of this array, and is responsible for
 *                          releasing it with free() when no longer needed. Each
 *                          member of the array is a constant char* pointer to a
 *                          NUL-terminated string giving the name of the appraisal
 *                          record. These individual pointers remain owned by the
 *                          library and must not be mutated or freed by the caller.
 * @param[out]  papp_rec_sz Upon successful return, receives the number of entries
 *                          in the appraisal record array. 
 * @retval  0   on success
 * @retval  -1  on failure
 */
int ear_get_app_recs(ear_t *ear, const char ***papp_rec, size_t *papp_rec_sz);

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
 * @brief Return the attested public key from the the specified appraisal record
 *
 * Return the attested public key from the Veraison key attestation extension
 * claims.  The key buffer must be free'd by the caller.
 *
 * @param[in]   ear       an ear_t object returned from a successful invocation
 *                        of ear_jwt_verify
 * @param[in]   app_rec   the submod name for the appraisal record
 * @param[out]  pakpub    Pointer to a byte buffer which, on success, is
 *                        populated with the attested public key.
 *                        The buffer is owned by the caller who needs to take
 *                        care of its disposal using free()
 * @param[out]  pakpub_sz Pointer to a size_t object that, on success, will be
 *                        assigned the length in bytes of the key
 * @param[out]  err_msg   pointer to a pre-allocated buffer (of at least @c
 *                        EAR_ERR_SZ bytes) which, on failure, will be filled in
 *                        by the callee with a human readable error message.
 *                        This can be set to NULL if no extra error reporting is
 *                        required
 *
 * @retval  0   on success
 * @retval  -1  on failure
 */
int ear_veraison_get_akpub(ear_t *ear, const char *app_rec, uint8_t **pakpub,
                           size_t *pakpub_sz, char err_msg[EAR_ERR_SZ]);

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
