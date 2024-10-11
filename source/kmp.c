#include "kmp.h"

posix *searchWord(char *filename, char *word)
{
  FILE *file = fopen(filename, "r");
  char *line = NULL;
  size_t length = 0;
  ssize_t read;
  posix *headPosition = NULL;
  if (file == NULL)
    exit(EXIT_FAILURE);
  int rows = 1;
  while ((read = getline(&line, &length, file)) != -1)
  {
    for (int i = 0; line[i]; i++)
      line[i] = tolower(line[i]);
    for (int i = 0; word[i]; i++)
      word[i] = tolower(word[i]);
    headPosition = searchKMP(word, line, rows, headPosition);
    rows++;
  }
  fclose(file);
  if (line)
    free(line);
  return headPosition;
}

posix *searchKMP(char *word, char *text, int rows, posix *headPosition)
{
  int *temp = malloc(strlen(word) * sizeof(int)), j = 0, i = 0;
  computeLPSarray(word, strlen(word), temp);
  while (i < strlen(text))
  {
    if (word[j] == text[i])
    {
      j++;
      i++;
    }
    if (j == strlen(word))
    {
      headPosition = append(headPosition, rows, ((i - j) + 1) - correction(text, i - j));
      j = temp[j - 1];
    }
    else if (word[j] != text[i])
    {
      if (j != 0)
        j = temp[j - 1];
      else
        i++;
    }
  }
  free(temp);
  return headPosition;
}

void computeLPSarray(char *word, int max, int *lps)
{
  int length = 0, i = 1;
  lps[0] = 0;
  while (i < max)
    if (word[i] == word[length])
    {
      length++;
      lps[i] = length;
      i++;
    }
    else if (length != 0)
      length = lps[length - 1];
    else
    {
      lps[i] = 0;
      i++;
    }
}

int correction(char *string, size_t until)
{
  char *newString = malloc(until + 1);
  strncpy(newString, string, until);
  newString[until] = '\0';
  int shift = strlen(newString) - utf8len(newString);
  free(newString);
  return shift;
}

size_t utf8len(char *string)
{
  size_t length = 0;
  for (; *string; ++string)
    if ((*string & 0xC0) != 0x80)
      ++length;
  return length;
}