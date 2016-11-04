#ifndef COMMAND_H
#define COMMAND_H

int dir_contains(char *path, char *name);
char* find_executable(char *path, char *cmd);
void execute(char *dir, char *cmd, char *args[]);

#endif
