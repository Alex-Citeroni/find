#ifndef LISTS_H
#define LISTS_H
#include "posix.h"

typedef struct lists
{
  char path[PATH_SIZE], word[PATH_SIZE], data[NAME_SIZE];
  unsigned int occurrences, total;
  struct posix *headPosix;
  struct lists *next, *headFilelist;
} lists;

typedef lists *list;
list addToList(list a, char *word);
void printList(list a),
    freeListScan(list head);

struct lists *appendLists(struct lists *list, struct lists *node);
int compareLists(struct lists *n1, struct lists *n2);
void fileList(struct lists *head),
    wordList(struct lists *head),
    fileListFile(struct lists *head, FILE *pFile),
    wordListFile(struct lists *head, char *output),
    freeWordList(lists *head);

#endif