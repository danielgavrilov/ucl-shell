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

// Given a directory path and a filename, it returns 1 if the filename exists,
// 0 otherwise.
int dir_contains(char *filepath, char *filename) {
  DIR *dp = opendir(filepath);
  if (dp == NULL) {
    closedir(dp);
    return 0;
  }
  struct dirent *entry;
  while((entry = readdir(dp)) != NULL) {
    if (streq(entry->d_name, filename)) {
      closedir(dp);
      return 1;
    }
  }
  closedir(dp);
  return 0;
}

// Given a PATH string and a filename, it goes through the directory paths (they
// are separated with colons ':') and returns the first one that contains the
// given filename.
// If none contain it, it returns NULL.
// The returned string is allocated on the heap and needs to be freed.
char* find_executable(char *path, char *filename) {
  struct Node *path_list = parse_path(path);
  char *result = NULL;
  while (path_list != NULL) {
    if (dir_contains(path_list->path, filename)) {
      result = extract_chars(path_list->path, strlen(path_list->path));
      break;
    }
    path_list = path_list->next;
  }
  List_free(path_list);
  return result;
}

// Given a directory path, filename and arguments, it executes the filename with
// the provided arguments in a child process and halts until it exits.
void execute(char *dir, char *filename, char *args[]) {

  int len = strlen(dir) + strlen(filename) + 2;
  char fullpath[len];

  snprintf(fullpath, len, "%s/%s", dir, filename);

  pid_t pid = fork();

  if (pid == -1) {
    fprintf(stderr, "fork() failed.");
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  } else {
    execv(fullpath, args);
    // in case exec never returns.
    _exit(EXIT_FAILURE);
  }
}
