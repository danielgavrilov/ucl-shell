#ifndef ENV_H
#define ENV_H

struct Env {
  char *PATH;
  char *HOME;
};

int is_var(char *var, char *str);
struct Node* parse_path(char *path);
struct Env* get_env(char *profile_path);
char* get_cwd();

#endif
