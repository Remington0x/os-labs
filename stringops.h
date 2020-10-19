#ifndef _STRINGOPS_H_
#define _STRINGOPS_H_

#include <stdio.h>

#define FILENAME_LENGTH 255
#define STRLENGTH 4000 

void strnull(char *str);

int is_exit(char *str);

int get_length(char *str);

void str_invert(char *str);

#endif
