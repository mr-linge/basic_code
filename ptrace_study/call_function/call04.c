#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <dlfcn.h>

#include "ptrace_util.h"
#include "vaddr_by_symbol.h"


long long get_return(pid_t pid) {
	wait_breakpoint(pid);
        struct user_regs_struct regs;
        get_registers(pid,&regs);
	return regs.rax;
}

long long call_dlopen(pid_t pid) {
	size_t func_addr = get_vaddr(pid, "dlopen", "libc-");
	printf("dlopen:          0x%lx\n", func_addr);
	const unsigned int num_param = 2;
	long parameters[num_param];
	parameters[0] = 0x4020e8;
	parameters[1] = RTLD_NOW;//RTLD_LAZY;

	call_function(pid, func_addr, parameters, num_param);
	long long return_value = get_return(pid);
	printf("%s return_value = 0x%llx\n",__FUNCTION__ ,return_value);
 
	return return_value;
}

long long call_open(pid_t pid) {
	size_t func_addr = get_vaddr(pid, "open", "libc-");
	printf("open:          0x%lx\n", func_addr);
	const unsigned int num_param = 2;
	long parameters[num_param];
	parameters[0] = 0x4020e2;;
	parameters[1] = O_RDONLY | O_WRONLY;

	call_function(pid, func_addr, parameters, num_param);
	long long return_value = get_return(pid);
	printf("%s return_value = 0x%llx\n",__FUNCTION__ ,return_value);
 
	return return_value;
}

long long call_mmap(pid_t pid) {
	size_t func_addr = get_vaddr(pid, "mmap", "libc-");
	printf("mmap:          0x%lx\n", func_addr);
	const unsigned int num_param = 6;
	void * start;
	struct stat sb;
	int fd = open("./loaded_file", O_RDONLY | O_WRONLY);
	fstat(fd, &sb);
	//start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	long parameters[num_param];
	parameters[0] = 0;				// 设置为NULL表示让系统自动选择分配内存的地址
	parameters[1] = sb.st_size;//0x1000;		// 映射内存的大小
	parameters[2] = PROT_READ | PROT_WRITE | PROT_EXEC; // 表示映射内存区域可读可写可执行
	parameters[3] = MAP_PRIVATE | MAP_ANONYMOUS;	// 建立匿名映射
	int fd2 = (int)call_open(pid);
	parameters[4] = 3;				// 若需要映射文件到内存中，则为文件的fd
	parameters[5] = 0;				// 文件映射偏移量

	call_function(pid, func_addr, parameters, num_param);
	long long return_value = get_return(pid);
	printf("%s return_value = 0x%llx\n",__FUNCTION__ ,return_value);

	//uint8_t *test_data = "hello hope you be success";
	//unsigned long len = strlen(test_data);
	//putdata(pid, func_addr, test_data, len);
	unsigned long len = sb.st_size;
	uint8_t data[len];
	unsigned long mmap_vaddr = (unsigned long)return_value;
	getdata(pid, mmap_vaddr, data, len);
	printf("data -> %p:\n%s\n",(void *)mmap_vaddr,data);
	for(int i = 0;i < len; i++) {
		printf("%02x ", data[i]);
	}
	puts("");
	
	return return_value;
}

// int write_char_table(pid_t pid, unsigned long vaddr) {
// 	uint8_t char_table[] = {'H','e','l','l','o','\0','W','o','r', 'l','l','d','\0'};
// 	unsigned long len = sizeof(char_table);

// 	putdata(pid, vaddr, char_table, len);

// 	return 0;
// }

void call_func2(pid_t pid) {
	size_t func2Addr = 0x401241;
	const unsigned int num_param2 = 2;
	long parameters2[num_param2];
	parameters2[0] = 0x4;
	parameters2[1] = 0x4;//sb.st_size;
	call_function(pid,func2Addr,parameters2,num_param2);
	long long return_value = get_return(pid);
	printf("%s return_value = 0x%llx\n",__FUNCTION__ ,return_value);
}

void call_print(pid_t pid) {
	size_t funcAddr = 0x401367;
	const unsigned int num_param = 1;
	long parameters[num_param];
	parameters[0] = 0x4020e8;
	call_function(pid,funcAddr,parameters,num_param);
	get_return(pid);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage:\n\t%s pid\n", argv[0]);
		return -1;
	}
	pid_t pid = atoi(argv[1]);

	ptrace_attach(pid);

	size_t func1Addr = 0x401221;//0x401231;
	set_breakpoint(pid, func1Addr);
	ptrace_cont(pid);

	wait_breakpoint(pid);
	struct user_regs_struct backup_regs;
	get_registers(pid,&backup_regs);


	//call_func2(pid);
//	call_mmap(pid);
	call_dlopen(pid);
	//wait_breakpoint(pid);
	call_print(pid);

	recovery_breakpoint(pid,backup_regs);
	
	return 0;
}
