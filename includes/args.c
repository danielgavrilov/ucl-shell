#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

// Given a string, it replaces spaces in it with '\0' (null characters) and
// returns an array of pointers to the separated strings.
// Freeing the original string frees all separated strings.
char** convert_to_args(char *str) {
  char *delimiter = " ";
  int len = count_delimiter(delimiter[0], str);
  char **args = malloc(sizeof(char*) * (len + 1));
  int i = 0;
  char *token;
  for (
    token = strtok(str, delimiter);
    token != NULL;
    token = strtok(NULL, delimiter)
  ) {
    args[i] = token;
    i++;
  }
  args[i] = NULL;
  return args;
}

// Given an array of pointers to strings, it prints each as "<index>: <string>".
// Used for debugging.
void print_args(char **args) {
  int i = 0;
  while (args[i] != NULL) {
    printf("%d: %s\n", i, args[i]);
    i++;
  }
}
