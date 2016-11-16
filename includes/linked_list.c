#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

// Creates a single, null-initialised node for a linked list.
struct Node* Node_create() {
  struct Node *node = malloc(sizeof(struct Node));
  node->path = NULL;
  node->next = NULL;
  return node;
}

// Adds the given string to the end of the list.
struct Node* List_add(struct Node **list, char *string) {

  struct Node *current = Node_create();
  current->path = string;

  if (*list == NULL) {
    *list = current;
  } else {
    struct Node *last = *list;
    while (last->next != NULL) {
      last = last->next;
    }
    last->next = current;
  }

  return current;
}

// Frees the given linked list--the nodes it contains and their strings.
void List_free(struct Node *list) {
  if (list != NULL) {
    if (list->next != NULL) {
      List_free(list->next);
    } else {
      free(list->path);
      free(list);
    }
  }
}

// Prints a given linked list in the format "<index>: <value>".
// Used for debugging.
void List_print(struct Node *list) {
  int i = 0;
  while (list != NULL) {
    printf("%d: %s\n", i++, list->path);
    list = list->next;
  }
}
