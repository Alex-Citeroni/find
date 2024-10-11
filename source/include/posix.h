#ifndef POSIX_H
#define POSIX_H
#define SHOW 100
#define EXT_MAX 20
#define NAME_SIZE 255
#define PATH_SIZE 4096
#define INNER_FILE 200
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct posix
{
  int row, character;
  struct posix *next;
} posix;

typedef struct parameters
{
  char word[PATH_SIZE], input[PATH_SIZE], output[PATH_SIZE], report[PATH_SIZE],
      toExclude[EXT_MAX], wordToShow[NAME_SIZE], innerFile[PATH_SIZE];
  int wordOccurrences, isVerbose;
} parameter;

posix *create(int row, int character, posix *next), *append(posix *head, int row, int character);
void deletePosix(posix *head), displayPosix(posix *head),
    displayPosixFile(posix *head, FILE *pFile);

#endif