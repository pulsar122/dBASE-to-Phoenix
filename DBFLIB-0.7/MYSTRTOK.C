/*
 * strtok version, which will not skip empty fields,
 * needed by a2dbf.c.
 * published under the GNU GPL (see COPYING)
 *
 * rasca, berlin 1995, 1996
 * (rasca@marie.physik.tu-berlin.de)
 */

#include <errno.h>
#include <string.h>

static char *oldstring = NULL;

/* Parse "s" into tokens separated by characters in "delim".
 */
char *mystrtok(char *s, char *delim) {
  char *token;

	if (s == NULL) {
		if (oldstring == NULL) {
			errno = EINVAL;
			return NULL;
		} else {
			s = oldstring;
		}
	}
	if (*s == '\0') {
		oldstring = NULL;
		return NULL;
	}
	/* find the end of the token.
	 */
	token = s;
	s = strpbrk(token, delim);
	if (s == NULL) {
		/* this token finishes the string.
		 */
		oldstring = NULL;
	} else {
		/* terminate the token and make "oldstring" point past it.
		 */
		*s = '\0';
		oldstring = s + 1;
	}
	return token;
}

