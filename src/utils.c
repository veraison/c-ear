#include "base64.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Copy src to string dst of size sz.  At most sz-1 characters
 * will be copied.  Always NUL terminates (unless sz == 0).
 * Returns strlen(src); if retval >= sz, truncation occurred.
 */
size_t u_strlcpy(char *dst, const char *src, size_t sz) {
  char *d = dst;
  const char *s = src;
  size_t n = sz;

  /* Copy as many bytes as will fit */
  if (n != 0 && --n != 0) {
    do {
      if ((*d++ = *s++) == 0)
        break;
    } while (--n != 0);
  }

  /* Not enough room in dst, add NUL and traverse rest of src */
  if (n == 0) {
    if (sz != 0)
      *d = '\0'; /* NUL-terminate dst */
    while (*s++)
      ;
  }

  return (s - src - 1); /* count does not include NUL */
}

/*
 * base64 decode using the URL-safe alphabet.
 * On success (retval=0), the @p pout and @p pout_sz
 */
int u_b64url_decode(const char *in, uint8_t **pout, size_t *pout_sz) {
  uint8_t *out = NULL;
  int out_sz = 0;

  if (in == NULL || strlen(in) == 0) {
    goto err;
  }

  if ((out_sz = Base64decode_len(in)) <= 0) {
    goto err;
  }

  if ((out = calloc(1, out_sz)) == NULL) {
    goto err;
  }

  out_sz = Base64decode((char *)out, in);
  if (out_sz <= 0) {
    goto err;
  }

  *pout = out;
  *pout_sz = (size_t)out_sz;

  return 0;
err:
  if (out)
    free(out);

  return -1;
}
