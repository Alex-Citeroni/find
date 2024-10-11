#ifndef FIND_H
#define FIND_H
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <getopt.h>
#include "lists.h"
#include "kmp.h"

int isDirectory(char *pathname);
const char *filenameExtension(const char *filename, const char *exclude);
list takeWordFromFile(char *filename);
void findWord(char *filePath, char *fileWord, char *excludeExt, char *output, int isVerbose),
    findPathFromReport(char *input, char *report, int occurrences),
    findOccurrencesFromFile(char *input, char *report, char *file), help();
struct lists *elaborateWord(char *word, char *filePath, char *exclude, int isVerbose),
    *searchWordInDirectory(char *basePath, int recursively, lists *headPathList, const char *exclude, char *word, int isVerbose);

#endif