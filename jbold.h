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
#ifndef JBOLD_H

#define JBOLD_H "INCLUDED"

#define PROG_NAME   "jbold"
#define JBOLD_BOLD  "JBOLD_BOLD"    /* default for overstrikes */
#define JBOLD_ERR   "JBOLD_ERR"     /* default for error  file */
#define JBOLD_FORCE "JBOLD_FORCE"   /* default for overwrite   */
#define JBOLD_OUT   "JBOLD_OUT"     /* default for output file */
#define JBOLD_TAB   "JBOLD_TAB"     /* default for Tab Size    */

#ifndef JLIB2_CHAR_NULL
#define NO_JLIB 1
#define JLIB2_CHAR_NULL  ((char) '\0')
#endif

#ifndef NULL
#define NULL '\0'
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifdef _MSDOS
#define SSIZE_T unsigned long int
#endif
#ifndef SSIZE_T
#define SSIZE_T ssize_t
#endif

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

/* messages */
#ifdef NO_JLIB
#define ARG_BOLD          'B'  /* Show BOLD characters               */
#define ARG_ERR           'e'  /* Output Error File                  */
#define ARG_FORCE         'f'  /* force create files                 */
#define ARG_HELP          'h'  /* Show Help                          */
#define ARG_OUT           'o'  /* Output File                        */
#define ARG_TAB_SPACE     'T'  /* Expand tabs to # characters        */
#define ARG_VERSION       'V'  /* Show Version Information           */
#define FILE_NAME_STDIN   "-"
#define LIT_ENV_01        "Environment Variables:\n"
#define LIT_ENV_02        "\t%-15s : Default (%c%c) %s\n"
#define LIT_INFO_01       "\tBuild: %s %s\n"
#define LIT_REV           "Revision"
#define MSG_ERR_E000      "Try '%s %c%c' for more information\n"
#define MSG_ERR_E002      "ERROR E002: Cannot open '%s' for write, processing aborted\n"
#define MSG_ERR_E007      "ERROR E007: '%s' is an invalid value for %c%c, must be > %d and < %d\n"
#define MSG_ERR_E008      "ERROR E008: '%s' is an invalid value for %c%c, must be numeric\n"
#define MSG_ERR_E009      "ERROR E009: Environment Variable '%s' value '%s' invalid\n"
#define MSG_ERR_E025      "ERROR E025: File %s cannot be created, already exists\n"
#define MSG_WARN_W002     "W002: Open Error Bypass File '%s' : %s\n"
#define SWITCH_CHAR       '-'
#define USG_MSG_ARG_BOLD          "\t%c%c num\t\t: Overrstike caracters 'num' times\n"
#define USG_MSG_ARG_ERR           "\t%c%c file\t\t: Write errors to file 'file', default stderr\n"
#define USG_MSG_ARG_FORCE         "\t%c%c\t\t: force create of files when found\n"
#define USG_MSG_ARG_HELP          "\t%c%c\t\t: Show brief help and exit\n"
#define USG_MSG_ARG_OUT           "\t%c%c file\t\t: Write output to file 'file', default stdout\n"
#define USG_MSG_ARG_TAB_SPACE_A   "\t%c%c num\t\t: Expand tabs to 'num' spaces.\n"
#define USG_MSG_ARG_TAB_SPACE_B   "\t\t\t  default, do NOT expand tabs\n"
#define USG_MSG_ARG_VERSION       "\t%c%c\t\t: Show revision information and exit\n"
#define USG_MSG_OPTIONS           "Options\n"
#define USG_MSG_USAGE             "usage:\t%s [OPTIONS] [FILES ...]\n"
#endif /* NO_JLIB */

/* global prototypes */
void init(struct s_work_area *, int, char **);
void close_out(struct s_file_info *);
int  show_rev(struct s_work_area *);
int  show_help(struct s_work_area *);
int  getenv_int(int default_val, FILE *fp, char *, char *, int, int);
char *get_progname(char *, char *);
void init_file(struct s_file_info *);

#ifdef NO_JLIB
SSIZE_T j2_getline(char **buf, size_t *n, FILE *fp);
char *j2_get_prgname(char *argv_0, char *default_name);
int j2_bye_char(char *buffer, char c);
int j2_expand_tab(int tab_size, size_t *out_buf_size, char **out_buf, char *in_buf);
int j2_f_exist(char *file_name);
int j2_is_numr(char *s);
long int j2_bye_last(char *x, char lastc);
long int j2_chg_char(char old, char new, char *s, SSIZE_T force_size);
long int j2_clr_str(char *s, char c, int size);
long int j2_rtw(char *buffer);
#define j2_bye_nl(x)    j2_bye_last((x), '\n')
#define j2_bye_ctlm(x)  j2_bye_last((x), '\r')
#endif /* NO_JLIB */

#endif  /* JBOLD_H */
