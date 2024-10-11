#include "find.h"

void findWord(char *filePath, char *fileWord, char *excludeExtension, char *output, int isVerbose)
{
  lists *headWordList = NULL;
  list words = takeWordFromFile(fileWord);
  while (words)
  {
    if (isVerbose)
      printf("Inizio elaborazione parola: %s\n", words->data);
    headWordList = appendLists(headWordList, elaborateWord(words->data, filePath, excludeExtension, isVerbose));
    if (isVerbose)
      printf("Fine elaborazione parola: %s\n", words->data);
    words = words->next;
  }
  if (strcmp(output, "NOT_SET") == 0)
    wordList(headWordList);
  else
    wordListFile(headWordList, output);
  freeWordList(headWordList);
  freeListScan(takeWordFromFile(fileWord));
}

list takeWordFromFile(char *filename)
{
  FILE *fp = fopen(filename, "r");
  char *line = NULL;
  size_t length = 0;
  ssize_t read;
  list word = NULL;
  if (fp == NULL)
    exit(EXIT_FAILURE);
  while ((read = getline(&line, &length, fp)) != -1)
  {
    if (line[strlen(line) - 1] == '\n')
      line[strlen(line) - 1] = '\0';
    word = addToList(word, line);
  }
  fclose(fp);
  if (line)
    free(line);
  return word;
}

struct lists *elaborateWord(char *word, char *filePath, char *exclude, int isVerbose)
{
  FILE *file = fopen(filePath, "r");
  char *line = NULL;
  size_t length = 0;
  ssize_t read;
  clock_t start_file_t;
  lists *tempWordList = malloc(sizeof(lists)), *headPathList = NULL;
  memset(tempWordList, 0, sizeof(lists));
  tempWordList->total = 0;
  strcpy(tempWordList->word, word);
  if (file == NULL)
  {
    printf("Il percorso passato è nullo");
    exit(EXIT_FAILURE);
  }
  while ((read = getline(&line, &length, file)) != -1)
  {
    char *ptr = strtok(line, " ");
    if (ptr[strlen(ptr) - 1] == '\n')
      ptr[strlen(ptr) - 1] = '\0';
    if (isDirectory(ptr) == 1)
    {
      char *temp = ptr;
      ptr = strtok(NULL, " ");
      if (ptr != NULL)
        headPathList = searchWordInDirectory(temp, 1, headPathList, exclude, word, isVerbose);
      else
        headPathList = searchWordInDirectory(temp, 0, headPathList, exclude, word, isVerbose);
    }
    else if (isDirectory(ptr) == -2 && filenameExtension(ptr, exclude) != 0)
    {
      if (isVerbose)
      {
        printf("%s %s \n", "Inizio elaborazione file:", ptr);
        start_file_t = clock();
      }
      lists *tempPathList = malloc(sizeof(lists));
      memset(tempPathList, 0, sizeof(lists));
      tempPathList->occurrences = 0;
      char buffer[PATH_SIZE];
      strcpy(tempPathList->path, realpath(ptr, buffer));
      posix *tempPosix = searchWord(ptr, word);
      tempPathList->headPosix = tempPosix;
      while (tempPosix)
      {
        tempPathList->occurrences++;
        tempPosix = tempPosix->next;
      }
      headPathList = appendLists(headPathList, tempPathList);
      tempPathList = NULL;
      if (isVerbose)
        printf("Fine elaborazione file: %s (%f sec)\n", ptr, ((double)clock() - start_file_t) / CLOCKS_PER_SEC);
    }
    else if (isDirectory(ptr) == -1)
      printf("Il percorso '%s' non è né un file né una cartella, verrà saltato.\n", ptr);
  }
  fclose(file);
  if (line)
    free(line);
  lists *countFileList = headPathList;
  while (countFileList)
  {
    tempWordList->total += countFileList->occurrences;
    countFileList = countFileList->next;
  }
  tempWordList->headFilelist = headPathList;
  return tempWordList;
}

struct lists *searchWordInDirectory(char *basePath, int recursively, lists *headPathList, const char *exclude, char *word, int isVerbose)
{
  char path[PATH_SIZE];
  struct dirent *dp;
  DIR *dir = opendir(basePath);
  struct stat sb;
  clock_t start_dir_t, start_file_t;
  if (dir)
  {
    if (isDirectory(basePath) && isVerbose)
    {
      start_dir_t = clock();
      printf("%s %s \n", "Inizio elaborazione directory:", basePath);
    }
    while ((dp = readdir(dir)) != NULL)
    {
      if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
      {
        strcpy(path, basePath);
        strcat(path, "/");
        strcat(path, dp->d_name);
        if (lstat(path, &sb) == 0 && S_ISREG(sb.st_mode) && (exclude ? (filenameExtension(path, exclude) != 0) : 1))
        {
          if (isVerbose)
          {
            printf("%s %s \n", "Inizio elaborazione file:", path);
            start_file_t = clock();
          }
          lists *tempPathList = malloc(sizeof(lists));
          memset(tempPathList, 0, sizeof(lists));
          tempPathList->occurrences = 0;
          char buf[PATH_SIZE];
          strcpy(tempPathList->path, realpath(path, buf));
          posix *tempPosix = searchWord(path, word);
          tempPathList->headPosix = tempPosix;
          while (tempPosix)
          {
            tempPathList->occurrences++;
            tempPosix = tempPosix->next;
          }
          headPathList = appendLists(headPathList, tempPathList);
          tempPathList = NULL;
          if (isVerbose)
            printf("Fine elaborazione file: %s (%f sec)\n", path, ((double)clock() - start_file_t) / CLOCKS_PER_SEC);
        }
        if (recursively)
          headPathList = searchWordInDirectory(path, 1, headPathList, exclude, word, isVerbose);
      }
    }
    closedir(dir);
    if (isDirectory(basePath) && isVerbose)
      printf("Fine elaborazione directory: %s (%f sec)\n", basePath, ((double)clock() - start_dir_t) / CLOCKS_PER_SEC);
  }
  return headPathList;
}

int isDirectory(char *pathname)
{
  DIR *dirptr = opendir(pathname);
  if (access(pathname, F_OK) != -1)
    if (dirptr != NULL)
      closedir(dirptr);
    else
      return -2;
  else
    return -1;
  return 1;
}

const char *filenameExtension(const char *filename, const char *exclude)
{
  char *temp = malloc(sizeof(exclude));
  strcpy(temp, exclude);
  char *ext = strtok(temp, "_");
  while (ext != NULL)
  {
    if (strcmp(strrchr(filename, '.') + 1, ext) == 0)
      return 0;
    ext = strtok(NULL, "_");
  }
  return strrchr(filename, '.');
}

void findPathFromReport(char *input, char *report, int occurrences)
{
  FILE *pFile = fopen(report, "r");
  if (pFile == NULL)
  {
    printf("%s\n", report);
    perror("Impossibile aprire il file\n");
  }
  char line[PATH_SIZE], temp[PATH_SIZE],
      path[PATH_SIZE], word[NAME_SIZE];
  int fileOccurrences;
  strcpy(word, "WORD ");
  strcat(word, input);
  while (fgets(line, PATH_SIZE, pFile) != NULL)
    if (strstr(line, word) != NULL)
    {
      printf("%s", line);
      while ((fgets(line, PATH_SIZE, pFile) != NULL) && (sscanf(line, "WORD %s\n", temp) == 0))
        if (sscanf(line, "FILE %s\n", path) == 1)
        {
          fgets(line, PATH_SIZE, pFile);
          sscanf(line, "OCCURRENCES %d\n", &fileOccurrences);
          if (fileOccurrences >= occurrences)
            printf("FILE  %s\nOCCURRENCES %d\n\n", path, fileOccurrences);
        }
    }
  fclose(pFile);
}

void findOccurrencesFromFile(char *input, char *report, char *file)
{
  FILE *pFile = fopen(report, "r");
  if (pFile == NULL)
    printf("\nFile inserito non valido\n");
  char line[PATH_SIZE], buf[PATH_SIZE],
      *absolutePath = realpath(file, buf),
      file2[PATH_SIZE] = "FILE ", word[NAME_SIZE];
  strcat(file2, absolutePath);
  strcpy(word, "WORD ");
  strcat(word, input);
  strcat(word, "\r\n");
  strcat(file2, "\r\n");
  while (fgets(line, PATH_SIZE, pFile) != NULL)
    if (strcmp(line, word) == 0)
    {
      printf("%s", line);
      while ((fgets(line, PATH_SIZE, pFile) != NULL))
        if (strcmp(line, file2) == 0)
        {
          fgets(line, PATH_SIZE, pFile);
          char *temp = malloc(strlen(line));
          strcpy(temp, line);
          if (atoi(strrchr(temp, ' ')) == 0)
            return;
          for (int i = 0; i < atoi(strrchr(temp, ' ')); i++)
          {
            fgets(line, PATH_SIZE, pFile);
            printf("%s", line);
          }
          return;
        }
    }
  fclose(pFile);
}