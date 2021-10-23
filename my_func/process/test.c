#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./process.h"

int main(int argc, char * argv[]) {
   if(argc < 2) {
     printf("Parameters less than 2");
     return -1;
   }
   int pid = atoi(argv[1]);
   char * name = get_process_name(pid);   
   printf("process pid = %d, it's name is %s\n",pid,name);
  return 0;
}
