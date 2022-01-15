#include <stdio.h>
#include <sys/ptrace.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "arm64_arch.h"

/**********
  aarch64-linux-gnu-gcc arm64_arch.c write.c -o write
  ./write `pidof ./target` 94619969200800  zzzzz
 * ***********/

int main(int argc, char **argv) {
	if (argc != 4) {
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		exit(1);
	}

	pid_t target_pid = atoi(argv[1]);
	unsigned long addr = atol(argv[2]);
	// printf("addr : %lx\n", addr);
	char *src = argv[3];
	unsigned long len = strlen(src);//sizeof(src);
	printf("src:%s, size = %lu\n", src, len);

	attach_process(target_pid);

	putdata(target_pid, addr, src, len);

	continue_process(target_pid);
	//      while(1) {
	//              printf("dst:%p\n", (void *) addr);
	//              sleep(3);
	//      }

	end_tracke_process(target_pid);

	return 0;
}

