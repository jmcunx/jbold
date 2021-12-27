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
#ifndef REV_JBOLD_H

#define REV_JBOLD_H "$Id: jbold.h,v 2.1 2021/12/27 19:28:00 jmccue Exp $"

#define PROG_NAME   "jbold"
#define JBOLD_BOLD  "JBOLD_BOLD"    /* default for overstrikes */
#define JBOLD_ERR   "JBOLD_ERR"     /* default for error  file */
#define JBOLD_FORCE "JBOLD_FORCE"   /* default for overwrite   */
#define JBOLD_OUT   "JBOLD_OUT"     /* default for output file */
#define JBOLD_TAB   "JBOLD_TAB"     /* default for Tab Size    */

struct s_file_info
{
  FILE *fp;
  char *fname;
} ;

struct s_work_area
{
  char *prog_name;
  int  overstrikes;       /* default      0 */
  int  tab_size;          /* default      8 */
  int  force;             /* TRUE or FALSE  */
  struct s_file_info out;
  struct s_file_info err;
} ;

/* global prototypes */
void init(struct s_work_area *, int, char **);
void close_out(struct s_file_info *);
int  show_rev(struct s_work_area *);
int  show_help(struct s_work_area *);
int  getenv_int(int default_val, FILE *fp, char *, char *, int, int);
char *get_progname(char *, char *);
void init_file(struct s_file_info *);
#ifdef JHELP_LONG
int show_help_long(FILE *);
#endif

#endif  /* END: REV_JBOLD_H */
