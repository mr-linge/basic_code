#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "ptrace_util.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		return -1;
	}
	pid_t target_pid = atoi(argv[1]);

	ptrace_attach(target_pid);
	int status;
	wait(&status);

	size_t func1Addr = 0x401211;
	set_breakpoint(target_pid,func1Addr);

	ptrace_cont(target_pid);

	struct user_regs_struct breakpoint_regs;
	if (is_hit_breakpoint(target_pid,&breakpoint_regs) == -1) {
		printf("%s:%d not hit breakpoint...\n",__FILE__,__LINE__);
		return -1;
	}
	show_registers(breakpoint_regs);
	//getchar();


	size_t func_addr = 0x7f4e539cda20;
	const unsigned int num_param = 6;
	void * start;
	struct stat sb;
	int fd = open("./loaded_file", O_RDONLY | O_WRONLY);
	fstat(fd, &sb);
	//start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	long parameters[num_param];
	parameters[0] = 0;  // 设置为NULL表示让系统自动选择分配内存的地址
	parameters[1] = sb.st_size; // 映射内存的大小
	parameters[2] = PROT_READ | PROT_WRITE | PROT_EXEC;  // 表示映射内存区域可读可写可执行
	parameters[3] =  MAP_ANONYMOUS | MAP_PRIVATE; // 建立匿名映射
	parameters[4] = fd; //  若需要映射文件到内存中，则为文件的fd
	parameters[5] = 0; //文件映射偏移量

	long result;
	call_function(target_pid,func_addr,parameters,num_param,breakpoint_regs,&result);
	printf("call_function return value = 0x%lx\n",result);

      
	size_t read_mem = 0x401273;
	const unsigned int num_param2 = 2;
	long parameters2[num_param2];
	parameters2[0] = result;//0x15eb2a0;
	parameters2[1] = sb.st_size;
	call_function(target_pid,read_mem,parameters2,num_param2,breakpoint_regs,NULL);


	uint8_t dst[sb.st_size];
	getdata(target_pid,result, dst, sb.st_size);
	printf("file size : %lu\n",sb.st_size);
	for(int k = 0; k < 0x1000; k++) {
		printf("%02x ",*(dst + k));
	}
	puts("");

	if (is_hit_breakpoint(target_pid,&breakpoint_regs) == -1) {
		printf("%s:%d not hit breakpoint...\n",__FILE__,__LINE__);
		return -1;
	}
	recovery_breakpoint(target_pid, breakpoint_regs);

	ptrace_cont(target_pid);

	return 0;
}
