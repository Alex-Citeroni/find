#include "posix.h"

posix *append(posix *head, int row, int character)
{
  if (!head)
    return create(row, character, NULL);
  posix *cursor = head;
  while (cursor->next != NULL)
    cursor = cursor->next;
  cursor->next = create(row, character, NULL);
  return head;
}

posix *create(int row, int character, posix *next)
{
  posix *newPosix = malloc(sizeof(posix));
  memset(newPosix, 0, sizeof(posix));
  if (newPosix == NULL)
  {
    printf("Errore nella creazione di un nuovo nodo\n");
    exit(0);
  }
  newPosix->row = row;
  newPosix->character = character;
  newPosix->next = next;
  return newPosix;
}

void deletePosix(posix *head)
{
  posix *prev = head, *cur = head;
  while (cur)
  {
    prev = cur;
    cur = prev->next;
    free(prev);
  }
}

void displayPosix(posix *head)
{
  posix *cursor = head;
  while (cursor != NULL)
  {
    printf("%d %d \n", cursor->row, cursor->character);
    cursor = cursor->next;
  }
}

void displayPosixFile(posix *head, FILE *pFile)
{
  posix *cursor = head;
  while (cursor != NULL)
  {
    fprintf(pFile, "%d %d \n", cursor->row, cursor->character);
    cursor = cursor->next;
  }
}