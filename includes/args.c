#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

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

void print_args(char **args) {
  int i = 0;
  while (args[i] != NULL) {
    printf("%d: %s\n", i, args[i]);
    i++;
  }
}
