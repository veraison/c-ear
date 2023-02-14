#include <stddef.h>

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
