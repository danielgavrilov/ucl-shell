#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct Node {
  char *path;
  struct Node *next;
};

struct Node* List_add(struct Node **list, char *string);
void List_print(struct Node *list);
void List_free(struct Node *list);

#endif
