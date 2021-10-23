#include "./process.h"

/*
 * This function will parse the cmdline proc entry to retrieve
 * the executable name of the process.
 * */
char * get_process_name(int pid) {
  char cmdline[256], path[512], *p;
  int fd;
  snprintf(cmdline,50,"/proc/%d/cmdline",pid);
  // printf("cmdline : %s\n",cmdline);
  if((fd = open(cmdline,O_RDONLY)) < 0) {
    perror("open");
    exit(-1);
  }
  if(read(fd, path, 512) < 0) {
    perror("read");
    exit(-1);
  }
  if((p = strdup(path)) == NULL) {
    perror("strdup");
    exit(-1);
  }
  return p;
}

