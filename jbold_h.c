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
#include <ctype.h>

#ifdef HAVE_JLIB
#include <j_lib2.h>
#include <j_lib2m.h>
#endif

#include "jbold.h"

#define USG_MSG_JBOLD_1  "\tBold Text for print on an epson printer.\n"
#define USG_MSG_JBOLD_2  "\tCan be used to bold output of pr().\n"
#define JBOLD_BOLD_TEXT  "number of overstrikes"
#define JBOLD_ERR_TEXT   "File Name for error file"
#define JBOLD_FORCE_TEXT "force overwrite"
#define JBOLD_OUT_TEXT   "File Name for output file"
#define JBOLD_TAB_TEXT   "Expand Tab Size"

/*
 * show_rev() -- Show Version information and exit
 */
int show_rev(struct s_work_area *w)

{

  fprintf(w->out.fp,"%s %s:\n", w->prog_name, LIT_REV);

#ifdef J_LIB2_H
  fprintf(w->out.fp, "\t%s %s\n", LIT_INFO_02, j2_get_build());
#endif

#ifdef OSTYPE
  fprintf(w->out.fp,"\t%s\n",OSTYPE);
#endif  /* OSTYPE  */
#ifdef PRETTY_NAME
  fprintf(w->out.fp,"\t%s\n",PRETTY_NAME);
#endif  /* PRETTY_NAME  */
  fprintf(w->out.fp, LIT_INFO_01, __DATE__, __TIME__);

  return(EXIT_FAILURE);

} /* show_rev() */

/*
 * show_help() -- displays show_help info and exits
 */
int show_help(struct s_work_area *w)

{

  fprintf(w->out.fp, USG_MSG_USAGE, w->prog_name);
  fprintf(w->out.fp, USG_MSG_JBOLD_1);
  fprintf(w->out.fp, USG_MSG_JBOLD_2);
  fprintf(w->out.fp, USG_MSG_OPTIONS);
  fprintf(w->out.fp, USG_MSG_ARG_BOLD,        SWITCH_CHAR, ARG_BOLD);
  fprintf(w->out.fp, USG_MSG_ARG_ERR,         SWITCH_CHAR, ARG_ERR);
  fprintf(w->out.fp, USG_MSG_ARG_FORCE,       SWITCH_CHAR, ARG_FORCE);
  fprintf(w->out.fp, USG_MSG_ARG_OUT,         SWITCH_CHAR, ARG_OUT);

  fprintf(w->out.fp, USG_MSG_ARG_HELP,        SWITCH_CHAR, ARG_HELP);
  fprintf(w->out.fp, USG_MSG_ARG_VERSION,     SWITCH_CHAR, ARG_VERSION);
  fprintf(w->out.fp, USG_MSG_ARG_TAB_SPACE_A, SWITCH_CHAR, ARG_TAB_SPACE);
  fprintf(w->out.fp, USG_MSG_ARG_TAB_SPACE_B);

  fprintf(w->out.fp, LIT_ENV_01);
  fprintf(w->out.fp, LIT_ENV_02, JBOLD_BOLD,  SWITCH_CHAR, ARG_BOLD,      JBOLD_BOLD_TEXT);
  fprintf(w->out.fp, LIT_ENV_02, JBOLD_ERR,   SWITCH_CHAR, ARG_ERR,       JBOLD_ERR_TEXT);
  fprintf(w->out.fp, LIT_ENV_02, JBOLD_FORCE, SWITCH_CHAR, ARG_FORCE,     JBOLD_FORCE_TEXT);
  fprintf(w->out.fp, LIT_ENV_02, JBOLD_OUT,   SWITCH_CHAR, ARG_OUT,       JBOLD_OUT_TEXT);
  fprintf(w->out.fp, LIT_ENV_02, JBOLD_TAB,   SWITCH_CHAR, ARG_TAB_SPACE, JBOLD_TAB_TEXT);
 
  return(EXIT_FAILURE);

} /* show_help() */
