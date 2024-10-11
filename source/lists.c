#include "lists.h"

struct lists *appendLists(struct lists *list, struct lists *node)
{
  struct lists *previous, *next;
  if (!list)
    list = node;
  else
  {
    previous = NULL;
    next = list;
    while (next && compareLists(node, next) > 0)
    {
      previous = next;
      next = next->next;
    }
    if (!next)
      previous->next = node;
    else if (previous)
    {
      node->next = previous->next;
      previous->next = node;
    }
    else
    {
      node->next = list;
      list = node;
    }
  }
  return list;
}

int compareLists(struct lists *n1, struct lists *n2)
{
  if ((n1->occurrences < n2->occurrences) || ((n1->occurrences == n2->occurrences) && (strcmp(n1->path, n2->path) > 0)) || (strcmp(n1->word, n2->word) > 0))
    return 1;
  return -1;
}

void wordList(struct lists *head)
{
  while (head)
  {
    printf("WORD %s\r\nTOTAL %i\r\n", head->word, head->total);
    fileList(head->headFilelist);
    head = head->next;
  }
}

void fileList(struct lists *head)
{
  while (head)
  {
    printf("FILE %s\r\nOCCURRENCES %i\r\n", head->path, head->occurrences);
    displayPosix(head->headPosix);
    head = head->next;
  }
}

void wordListFile(struct lists *head, char *output)
{
  FILE *file = fopen(output, "w");
  if (file == NULL)
    puts("Impossibile aprire il file");
  else
    while (head)
    {
      fprintf(file, "WORD %s\r\nTOTAL %i\r\n", head->word, head->total);
      fileListFile(head->headFilelist, file);
      head = head->next;
    }
  fclose(file);
}

void fileListFile(struct lists *head, FILE *pFile)
{
  while (head)
  {
    fprintf(pFile, "FILE %s\r\nOCCURRENCES %i\r\n", head->path, head->occurrences);
    displayPosixFile(head->headPosix, pFile);
    head = head->next;
  }
}

void freeWordList(lists *head)
{
  lists *previous = head, *cur = head;
  while (cur)
  {
    previous = cur;
    cur = previous->next;
    deletePosix(previous->headPosix);
    free(previous);
  }
}

void freeListScan(list head)
{
  list previous = head, cur = head;
  while (cur)
  {
    previous = cur;
    cur = previous->next;
    free(previous);
  }
}

list addToList(list lista, char *word)
{
  if (lista == NULL)
  {
    lista = malloc(sizeof(lists));
    strcpy(lista->data, word);
    lista->next = NULL;
  }
  else
  {
    list iterator = lista, temp = malloc(sizeof(lists));
    while (iterator->next != NULL)
      iterator = iterator->next;
    strcpy(temp->data, word);
    temp->next = NULL;
    iterator->next = temp;
  }
  return lista;
}

void printList(list lista)
{
  list temp = lista;
  printf("La lista contiene i seguenti elementi: \n");
  while (temp != NULL)
  {
    printf("%s\n", temp->data);
    temp = temp->next;
  }
}