/*
 * Copyright (c) 1994 1995 1996 ... 2023 2024
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

/*
 * jbold_j.c -- routines from lib j_lib2
 */

#ifndef _MSDOS
#include <sys/param.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "jbold.h"

#ifdef _AIX
#define HAS_GETLINE 1
#endif
#ifdef __NetBSD_Version__
#define HAS_GETLINE 1
#endif
#ifdef OpenBSD
#define HAS_GETLINE 1
#endif
#ifdef __FreeBSD__
#define HAS_GETLINE 1
#endif
#ifdef linux
#define HAS_GETLINE 1
#endif

#ifndef TAB_CHAR
#define TAB_CHAR 0x09
#endif

#define SIZE_GETLINE_BUF 256

/*
 * j2_bye_char() -- removes all occurances of a specific a char from a string
 */
int j2_bye_char(char *buffer, char c)

{
  char *temp_str;
  int i, j, num_c = 0;
  int found = (int) FALSE;
#ifdef OpenBSD
  size_t len;
#endif

  if (c == JLIB2_CHAR_NULL)
    return(0);

  i = strlen(buffer) + 2;
  temp_str = calloc(i, sizeof(char));
  if (temp_str == (char *) NULL)
    return(-1);

  for ( i=0, j=0; buffer[i] != JLIB2_CHAR_NULL; i++)
    {
      if (buffer[i] == c)
	{
	  ++num_c;
	  found = (int) TRUE;
	}
      else
	{
	  temp_str[j] = buffer[i];
	  ++j;
	}
    }

  if (found == (int) FALSE)
    {
      free(temp_str);
      return(num_c);
    }

  /* note: size of buffer will never increase and   */
  /*       we removed at least 1 embedded character */
#ifdef OpenBSD
  len = strlen(temp_str) + 1;
  strlcpy(buffer, temp_str, len);
#else
  strcpy(buffer, temp_str);
#endif

  free(temp_str);
  return(num_c);

} /* j2_bye_char() */

/*
 * j2_bye_last() -- When the last byte = x, replaces it with NULL 
 *                  and returns string length
 */
long int j2_bye_last(char *x, char lastc)

{
  int i;
  
  if ( x == (char *) NULL )
    return(0L);
  if (strlen(x) < 1)
    return(0L);
  
  i = strlen(x) - (size_t) 1;
  
  if (x[i] == lastc)
    x[i] = JLIB2_CHAR_NULL;
  
  return((long int) strlen(x));
  
} /* j2_bye_last() */

/*
 * j2_chg_char() -- replaces all occurrences of one character 
 *                  with another char
 */
long int j2_chg_char(char old, char new, char *s, SSIZE_T force_size)

{
  size_t found = (size_t) 0;

  if (s == (char *) NULL)
    return(found);

  if (force_size < (SSIZE_T) 1)
    {
      for ( ; (*s) != JLIB2_CHAR_NULL; s++)
        {
          if ((*s) == old)
	    {
	      found++;
	      (*s) = new;
	    }
        }
    }
  else
    {
      for ( ; found < force_size; found++)
        {
          if (s[found] == old)
	    (*s) = new;
        }
    }

  return((long int) found);

} /* j2_chg_char() */

/*
 * j2_f_exist() -- determines if a file exists
 */
int j2_f_exist(char *file_name)

{
  if (file_name == (char *) NULL)
    return((int) FALSE);

#ifdef _MSDOS
  if (access(file_name, 00) == -1)
    return (FALSE);
  else
    return (TRUE);
#else
  struct stat file_info;
  if (stat(file_name, &file_info) == 0)
    return (TRUE);
  else
    return (FALSE);
#endif

} /* j2_f_exist() */

/*
 * j2_clr_str() -- Clears a string with a char & ensure it ends with NULL
 */
long int j2_clr_str(char *s, char c, int size)

{
  if (s == (char *) NULL)
    return(0L);

  memset(s, (int) c, (size_t) size);
  s[size - 1 ] = JLIB2_CHAR_NULL;

  return((long int) strlen(s));

} /* j2_clr_str() */

/*
 * j2_expand_tab() -- expand tabs to spaces into a new string
 */
int j2_expand_tab(int tab_size, size_t *out_buf_size, char **out_buf, char *in_buf)

{

  long remd;
  long col, next_col;
  size_t new_size = 0;
  char *wbuf;

  if ((*out_buf) == (char *) NULL)
    (*out_buf_size) = (size_t) 0;

  /*** check for empty in_buf ***/
  if (in_buf == (char *) NULL)
    {
      if ( (*out_buf) != (char *) NULL )
	free(*out_buf);
      (*out_buf)      = (char *) NULL;
      (*out_buf_size) = (size_t) 0;
      return((int) TRUE);
    }

  /*** Verify Tab Size, if invalid replace with 8 ***/
  if (tab_size < 1)
    tab_size = 8;

  /*** if tab size = 1, do a simple replace and exit ***/
  if (tab_size == 1)
    {
      if ((*out_buf) != (char *) NULL)
	free((*out_buf));
      (*out_buf) = strdup(in_buf);
      j2_chg_char((char) TAB_CHAR, ' ', (*out_buf), 0);
      (*out_buf_size) = strlen((*out_buf));
      return((int) TRUE);
    }

  /*** Need to expand tabs                          ***/
  /*** Do we need more memory ?  if so, allocate it ***/
  new_size = ( strlen(in_buf) * tab_size) + 100;

  if (new_size > (*out_buf_size))
    {
      if ((*out_buf) != (char *) NULL)
	free((*out_buf));
      (*out_buf) = malloc((new_size * sizeof(char)) );
      if ((*out_buf) == (char *) NULL)
	return((int) FALSE);
      (*out_buf_size) = new_size;
    }

  j2_clr_str((*out_buf), JLIB2_CHAR_NULL, new_size);

  /*** Create new string ***/
  wbuf = (*out_buf);
  for ( col = 0; (*in_buf) != JLIB2_CHAR_NULL; in_buf++)
    {
      switch (*in_buf)
	{
	  case TAB_CHAR:
	    remd     = (col + 1) % tab_size;
	    next_col = (col + 1) / tab_size;
	    if (remd > 0L)
	      remd = 1L;
	    next_col = (next_col + remd) * tab_size;
	    for ( ; col < next_col; col++)
	      wbuf[col] = ' ';
	    break;
	  default:
	    wbuf[col] = (*in_buf);
	    col++;
	    wbuf[col] = JLIB2_CHAR_NULL;
	}
    }

  /*** Done ***/
  return((int) TRUE);

} /* j2_expand_tab() */

/*
 * j2_getline() -- A front end to getline(3) or a hack for
 *                 systems without getline(3)
 */
SSIZE_T j2_getline(char **buf, size_t *n, FILE *fp)
{
#ifdef HAS_GETLINE
  return(getline(buf, n, fp));
#else

  if ((*buf) == (char *) NULL)
    {
      (*n) = SIZE_GETLINE_BUF + 1;
      (*buf) = (char *) malloc(((*n) * sizeof(char)));
      if ((*buf) == (char *) NULL)
	return(-1);
      j2_clr_str((*buf), (*n), JLIB2_CHAR_NULL);
    }

  if (fgets((*buf), (*n), fp) == (char *) NULL)
    return(-1);
  return((SSIZE_T) strlen((*buf)));

#endif

} /* j2_getline() */

/*
 * j2_get_prgname() -- return filename or default name.
 *
 *                     basename() is not used.  Some OS Systems do not
 *                     have basename() so this can be a basename()
 *                     replacement
 */
char *j2_get_prgname(char *argv_0, char *default_name)

{
  int i = 0, j = 0;

  if (argv_0 == (char *) NULL)
    {
      if (default_name == (char *) NULL)
	return((char *) NULL);
      else
	return(strdup(default_name));
    }

  /* basename(3) not in Coherent or MS-DOS */

  for (i = 0, j = 0; argv_0[i] != JLIB2_CHAR_NULL; i++)
    {
      if (argv_0[i] == '/')
	j = i + 1;
      else
	{
	  if (argv_0[i] == '\\')
	    j = i + 1;
	}
    }

  if (argv_0[j] == JLIB2_CHAR_NULL)
    if (default_name == (char *) NULL)
      return((char *) NULL);
    else
      return(strdup(default_name));
  else
    return(strdup(&(argv_0[j])));

} /* j2_get_prgname() */

/*
 * j2_is_numr() -- determines if all characters are numeric
 */
int j2_is_numr(char *s)

{
  if (s == (char *) NULL)
    return((int) FALSE); /* NULL pointer */

  for ( ; (*s) != JLIB2_CHAR_NULL; s++)
    {
      if ( ! isdigit((int)(*s)) )
	return(FALSE);
    }

  return(TRUE);

} /* j2_is_numr() */

/*
 * j2_rtw() -- removes trailing white space
 */
long int j2_rtw(char *buffer)

{
  char *last_non_space;
  char *b = buffer;

  if (buffer == (char *) NULL)
    return(0L); /* NULL pointer */

  last_non_space = buffer;

  for ( ; (*buffer) != JLIB2_CHAR_NULL; buffer++)
    {
      if ( ! isspace((int)(*buffer)) )
	last_non_space = buffer;
    }

  if ( ! isspace ((int) *last_non_space) )
    last_non_space++;

  (*last_non_space) = JLIB2_CHAR_NULL;

  return((long int) strlen(b));

} /* j2_rtw() */
