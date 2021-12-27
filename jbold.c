/*
 * Copyright (c) 2009 ... 2021 2022
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
#include <stdlib.h>
#include <string.h>
#ifdef UNIX
#include <unistd.h>
#endif
#ifdef _MSDOS
#include <getopt.h>
#endif
#include <errno.h>

#include <j_lib2.h>
#include <j_lib2m.h>
#include "jbold.h"

char *rev_jbold_c="$Id: jbold.c,v 2.1 2021/12/27 19:28:00 jmccue Exp $";

/*
 * close_in() -- Close an input file
 */
void close_in(FILE **in, char *fname)

{
  if (fname == (char *) NULL)
    return;
  if (strcmp(fname, FILE_NAME_STDIN) == 0)
    return;
  
  if ((*in) != (FILE *) NULL)
    {
      fclose((*in));
      (*in) = stdin;
    }

} /* close_in() */

/*
 * open_in() -- open in file
 */
int open_in(FILE **in, char *fname, FILE *fp_err)

{

  int errsave;

  if (fname == (char *) NULL)
    {
      (*in) = stdin;
      return((int) TRUE);
    }
  if (strcmp(fname, FILE_NAME_STDIN) == 0)
    {
      (*in) = stdin;
      return((int) TRUE);
    }

  (*in) = fopen(fname, "r");
  errsave = errno;

  if ((*in) == (FILE *) NULL)
    {
      fprintf(fp_err, MSG_WARN_W002, fname, strerror(errsave));
      return((int) FALSE);
    }

  return((int) TRUE);

} /* open_in() */

/*
 * print_line() -- print the line, one char at a time
 */
void print_line(struct s_work_area *w, char *b)

{
  char *out = (char *) NULL;
  char *o = (char *) NULL;
  size_t out_size = 0;
  int strike = 0;

  j2_bye_nl(b);

  /*** print the line ***/
  if (b == (char *) NULL)
    return;

  /*** ^L check ***/
  if (strlen(b) == 1)
    {
      if ((*b) == 12)  /* ^L */
	{
	  fprintf(w->out.fp, "%s", b);
	  return;
	}
    }

  /*** expand tabs ***/
  j2_rtw(b);
  out = (char *) NULL;
  out_size = 0;

  o = b;
  if ((w->tab_size > 0) && (strlen(b) > 0))
    {
      if (j2_expand_tab(w->tab_size, &out_size, &out, b) == 0)
	o = out; /* tabs expanded successfully */
    }

  /*** print the line ***/
  for ( ; (*o) != JLIB2_CHAR_NULL; o++)
    {
      fprintf(w->out.fp, "%c", (*o));
      if (w->overstrikes > 0)
	{
	  if ( ((*o) > 32) && (*o < 127))
	    {
	      for (strike = 0; strike < w->overstrikes; strike++)
		{
		  fprintf(w->out.fp, "%c", 8);
		  fprintf(w->out.fp, "%c", (*o));
		}
	    }
	}
    }

  /*** Done ***/
  if (out != (char *) NULL)
    free(out);

} /* print_line() */

/*
 * process_file()
 */
void process_file(struct s_work_area *w, char *fname)

{
  char *buf = (char *) NULL;
  ssize_t  buf_size = 0;
  FILE *fp = (FILE *) NULL;

  if ( ! open_in(&fp, fname, w->err.fp) )
    return;

  /*** process data ***/
  while (getline(&buf, &buf_size, fp) > -1)
    {
      print_line(w, buf);
      fprintf(w->out.fp, "\n");
    }

  /*** DONE ***/
  close_in(&fp, fname);

  return;

} /* process_file() */

/*
 * main()
 */
int main(int argc, char **argv)

{
  struct s_work_area w;
  int i;

  init(&w, argc, argv);

  for (i = optind; i < argc; i++)
    process_file(&w, argv[i]);
  if (i == optind)
    process_file(&w, FILE_NAME_STDIN);

  close_out(&(w.out));
  close_out(&(w.err));
  exit(EXIT_SUCCESS);

} /* main() */
