/*
 * Copyright (c) 2009 ... 2023 2024
 *     John McCue <jmccue@jmcunx.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef _MSDOS
#include <sys/param.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef HAVE_JLIB
#include <j_lib2.h>
#include <j_lib2m.h>
#endif

#include "jbold.h"

/*
 * getenv_int() -- convert an environment variable to an int
 */
int getenv_int(int default_val, FILE *fp, char *pname, char *strenv, int low, int high)

{
  int v;
  char *benv = (char *) NULL;

  if (strenv == (char *) NULL)
    return(default_val);

  benv = getenv(strenv);
  if (benv == (char *) NULL)
    return(default_val);
  
  v = atoi(benv);

  if ( ! j2_is_numr(benv))
    {
      fprintf(fp, MSG_ERR_E009, strenv, benv);
      fprintf(fp, MSG_ERR_E000, pname, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }

  if ((low != 0) && (high != 0))
    {
      if ((v < low) || (v > high))
	{
	  fprintf(fp, MSG_ERR_E009, strenv, benv);
	  fprintf(fp, MSG_ERR_E000, pname, SWITCH_CHAR, ARG_HELP);
	  exit(EXIT_FAILURE);
	}
    }
  
  return(v);

}  /* getenv_int() */
