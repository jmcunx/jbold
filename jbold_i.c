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
#include <ctype.h>
#include <errno.h>
#ifdef UNIX
#include <unistd.h>
#endif
#ifdef _MSDOS
#include <getopt.h>
#endif
#ifndef __COHERENT__
#include <libgen.h>
#endif
#include <j_lib2.h>
#include <j_lib2m.h>

#include "jbold.h"

#define MIN_TAB_SIZE     0
#define MAX_TAB_SIZE     100000
#define MIN_BOLD_SIZE    0
#define MAX_BOLD_SIZE    10

/* global variables */
char *rev_jbold_i_c="$Id: jbold_i.c,v 2.1 2021/12/27 19:28:00 jmccue Exp $";

/*
 * init_file() -- initialize file structure
 */
void init_file(struct s_file_info *f)

{

  f->fp    = (FILE *) NULL;
  f->fname = (char *) NULL;

} /* init_file() */

/*
 * init_work_area() -- initialize
 */
void init_work_area(struct s_work_area *w, char *pname)

{

  init_file(&(w->out));
  init_file(&(w->err));
  w->err.fp            = stderr;
  w->out.fp            = stdout;

  w->prog_name         = j2_get_prgname(pname, PROG_NAME);
  w->overstrikes       = getenv_int(1, w->err.fp, w->prog_name, JBOLD_BOLD,  MIN_BOLD_SIZE, MAX_BOLD_SIZE);
  w->tab_size          = getenv_int(0, w->err.fp, w->prog_name, JBOLD_TAB,   MIN_TAB_SIZE,  MAX_TAB_SIZE);
  w->force             = getenv_int(0, w->err.fp, w->prog_name, JBOLD_FORCE, 0,             1);

  w->err.fname = getenv(JBOLD_ERR);
  w->out.fname = getenv(JBOLD_OUT);

} /* init_work_area() */

/*
 * init_load_numeric() -- convert an argument to a numeric value
 */
int init_load_numeric(int rmode, 
                      char argsw, 
                      char *arg, 
                      int lowval, 
                      int highval, 
                      struct s_work_area *w)
{

  int val;

  if ( !j2_is_numr(arg))
    {
	fprintf(w->err.fp, MSG_ERR_E008, optarg, SWITCH_CHAR, argsw);
	fprintf(w->err.fp, MSG_ERR_E000, w->prog_name, SWITCH_CHAR, ARG_HELP);
	exit(EXIT_FAILURE);
    }

    
  val = atoi(optarg);

  if (rmode == (int) TRUE)
    {
      if ( (val < lowval) || (val > highval) )
	{
	  fprintf(w->err.fp, MSG_ERR_E007, optarg, SWITCH_CHAR, argsw, lowval, highval);
	  fprintf(w->err.fp, MSG_ERR_E000, w->prog_name, SWITCH_CHAR, ARG_HELP);
	  exit(EXIT_FAILURE);
	}
    }

  return(val);

} /* init_load_numeric() */

/*
 * close_out() -- close output
 */
void close_out(struct s_file_info *f)
{

  if (f->fname != (char *) NULL)
    {
      fclose(f->fp);
      free(f->fname);
      f->fname = (char *) NULL;
    }

} /* close_out() */

/*
 * open_out() -- save the file anem and check status
 */
int open_out(FILE *wfp, struct s_file_info *f, char *fname, int force)

{

  int errsave;

  if (fname == (char *) NULL)
    return((int) TRUE);

  if (force == (int) FALSE)
    {
      if ( j2_f_exist(fname) )
	{
	  fprintf(wfp, MSG_ERR_E025, fname);
	  return((int) FALSE);
	}
    }


  f->fp = fopen(fname, "w");
  errsave = errno;

  if (f->fp == (FILE *) NULL)
    {
      f->fp = stderr;  /* needs to be something */
      fprintf(wfp, MSG_ERR_E002, fname);
      fprintf(wfp, "\t%s\n", strerror(errsave));
      return((int) FALSE);
    }

  /*** success, save file name ***/
  f->fname = strdup(fname);
  return((int) TRUE);

} /* open_out() */

/*
 * get_args() -- load arguments
 */
void get_args(struct s_work_area *w, int argc, char **argv)

{
  char ckarg[80];
  int opt;
  char *out_file   = (char *) NULL;
  char *err_file   = (char *) NULL;
  int display_rev  = (int) FALSE;
  int display_help = (int) FALSE;
  int ok;

  /* ensure we have the default from the environment */
  out_file = w->out.fname;
  err_file = w->err.fname;

#ifdef JHELP_LONG
  int display_help_long = (int) FALSE;
#endif

#ifdef JHELP_LONG
  sprintf(ckarg, "%c:%c:%c:%c:%c%c%c%c", 
	  ARG_TAB_SPACE,
	  ARG_BOLD,
	  ARG_ERR, 
	  ARG_OUT, 
	  ARG_HELP_LONG, 
	  ARG_HELP, 
	  ARG_FORCE, 
	  ARG_VERSION);
#else
  sprintf(ckarg, "%c:%c:%c:%c:%c%c%c", 
	  ARG_TAB_SPACE,
	  ARG_BOLD,
	  ARG_ERR, 
	  ARG_OUT, 
	  ARG_HELP, 
	  ARG_FORCE, 
	  ARG_VERSION);
#endif

  while ((opt = getopt(argc, argv, ckarg)) != -1)
    {
      switch (opt)
	{
	  case ARG_BOLD:
	    w->overstrikes = init_load_numeric(TRUE, ARG_BOLD, optarg, MIN_BOLD_SIZE, MAX_BOLD_SIZE, w);
	    break;
	  case ARG_TAB_SPACE:
	    w->tab_size = init_load_numeric(TRUE, ARG_TAB_SPACE, optarg, MIN_TAB_SIZE, MAX_TAB_SIZE, w);
	    break;
	  case ARG_FORCE:
	    w->force = (int) TRUE;
	    break;
	  case ARG_HELP:
	    display_help = (int) TRUE;
	    break;
#ifdef JHELP_LONG
	  case ARG_HELP_LONG:
	    display_help_long = (int) TRUE;
	    break;
#endif
	  case ARG_VERSION:
	    display_rev = (int) TRUE;
	    break;
	  case ARG_OUT:
	    out_file = optarg;
	    break;
	  case ARG_ERR:
	    err_file = optarg;
	    break;
	  default:
	    fprintf(w->err.fp, MSG_ERR_E000, w->prog_name, SWITCH_CHAR, ARG_HELP);
	    exit(EXIT_FAILURE);
	    break;
	}
    }

  /*** if necessary - save stdout/err files and open ***/
  ok = open_out(stderr, &(w->err), err_file, w->force);
  if ( ok )
    ok = open_out(w->err.fp, &(w->out), out_file, w->force);
  if ( ! ok )
    {
      fprintf(w->err.fp, MSG_ERR_E000, w->prog_name, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }

  /* show help/rev, exit if displyed */
  ok = EXIT_SUCCESS;
#ifdef JHELP_LONG
  if (display_help_long)
    ok = show_help_long(w->out.fp);
#endif
  if (display_help)
    ok = show_help(w);
  if (display_rev)
    ok = show_rev(w);
  if (ok != EXIT_SUCCESS)
    {
      close_out(&(w->out));
      close_out(&(w->err));
      exit(ok);
    }

} /* end: get_args() */

/*
 * init() -- initialize
 */
void init(struct s_work_area *w, int argc, char **argv)

{

  init_work_area(w, argv[0]);

  get_args(w, argc, argv);

} /* init() */
