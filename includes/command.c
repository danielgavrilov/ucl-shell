#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>

#include "command.h"
#include "env.h"
#include "utils.h"
#include "linked_list.h"

int dir_contains(char *path, char *name) {
  DIR *dp = opendir(path);
  if (dp == NULL) {
    return 0;
  }
  struct dirent *entry;
  while((entry = readdir(dp)) != NULL) {
    if (streq(entry->d_name, name)) {
      return 1;
    }
  }
  closedir(dp);
  return 0;
}

char* find_executable(char *path, char *cmd) {
  struct Node *path_list = parse_path(path);
  char *result = NULL;
  while (path_list != NULL) {
    if (dir_contains(path_list->path, cmd)) {
      result = extract_chars(path_list->path, strlen(path_list->path));
      List_free(path_list);
      return result;
    }
    path_list = path_list->next;
  }
  return NULL;
}

void execute(char *dir, char *cmd, char *args[]) {

  int len = strlen(dir) + strlen(cmd) + 2;
  char fullpath[len];

  snprintf(fullpath, len, "%s/%s", dir, cmd);

  pid_t pid = fork();

  if (pid == -1) {
    fprintf(stderr, "fork() failed.");
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  } else {
    // we are the child
    execv(fullpath, args);
    // in case exec never returns.
    _exit(EXIT_FAILURE);
  }
}
