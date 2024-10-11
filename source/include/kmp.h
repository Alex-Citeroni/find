#ifndef KMP_H
#define KMP_H
#include <ctype.h>
#include <unistd.h>
#include "posix.h"

size_t utf8len(char *s);
int correction(char *string, size_t until);
void computeLPSarray(char *word, int M, int *lps);
posix *searchKMP(char *word, char *text, int rows, posix *headPosix),
    *searchWord(char *filename, char *word);

#endif