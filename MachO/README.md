# Mach-O 文件格式

Mach-O 主要由 3 部分组成:

Mach-O 头(Mach Header):这里描述了 Mach-O 的 CPU 架构、文件类型以及加载命令等信息
加载命令(Load Command):描述了文件中数据的具体组织结构,不同的数据类型使用不同的加载命令表示
数据区(Data):Data 中每一个段(Segment)的数据都保存在此,段的概念和 ELF 文件中段的概念类似,都拥有一个或多个 Section,用来存放数据和代码

## Mach-O 头

与 Mach-O 文件格式有关的结构体定义都可以从 /usr/include/mach-o/loader.h 中找到,也就是 <mach-o/loader.h> 头

#define	MH_MAGIC	0xfeedface	/* the mach magic number */
#define MH_CIGAM	0xcefaedfe	/* NXSwapInt(MH_MAGIC) */

struct mach_header_64 {
	uint32_t	magic;		/* mach magic 标识符 */
	cpu_type_t	cputype;	/* CPU 类型标识符,同通用二进制格式中的定义 */
	cpu_subtype_t	cpusubtype;	/* CPU 子类型标识符,同通用二级制格式中的定义 */
	uint32_t	filetype;	/* 文件类型 */
	uint32_t	ncmds;		/* 加载器中加载命令的条数 */
	uint32_t	sizeofcmds;	/* 加载器中加载命令的总大小 */
	uint32_t	flags;		/* dyld 的标志 */
	uint32_t	reserved;	/* 64 位的保留字段 */
};

由于 Mach-O 支持多种类型文件,所以此处引入了 filetype 字段来标明,这些文件类型定义在 loader.h 文件中同样可以找到。
#define	MH_OBJECT	0x1		/* Target 文件:编译器对源码编译后得到的中间结果 */
#define	MH_EXECUTE	0x2		/* 可执行二进制文件 */
#define	MH_FVMLIB	0x3		/* VM 共享库文件(还不清楚是什么东西) */
#define	MH_CORE		0x4		/* Core 文件,一般在 App Crash 产生 */
#define	MH_PRELOAD	0x5		/* preloaded executable file */
#define	MH_DYLIB	0x6		/* 动态库 */
#define	MH_DYLINKER	0x7		/* 动态连接器 /usr/lib/dyld */
#define	MH_BUNDLE	0x8		/* 非独立的二进制文件,往往通过 gcc-bundle 生成 */
#define	MH_DYLIB_STUB	0x9		/* 静态链接文件(还不清楚是什么东西) */
#define	MH_DSYM		0xa		/* 符号文件以及调试信息,在解析堆栈符号中常用 */
#define	MH_KEXT_BUNDLE	0xb		/* x86_64 内核扩展 */

另外在 loader.h 中还可以找到 flags 中所取值的全部定义,这里只介绍常用的:
#define	MH_NOUNDEFS	0x1		/* Target 文件中没有带未定义的符号,常为静态二进制文件 */
#define MH_SPLIT_SEGS	0x20  /* Target 文件中的只读 Segment 和可读写 Segment 分开  */
#define MH_TWOLEVEL	0x80		/* 该 Image 使用二级命名空间(two name space binding)绑定方案 */
#define MH_FORCE_FLAT	0x100 /* 使用扁平命名空间(flat name space binding)绑定(与 MH_TWOLEVEL 互斥) */
#define MH_WEAK_DEFINES	0x8000 /* 二进制文件使用了弱符号 */
#define MH_BINDS_TO_WEAK 0x10000 /* 二进制文件链接了弱符号 */
#define MH_ALLOW_STACK_EXECUTION 0x20000/* 允许 Stack 可执行 */
#define	MH_PIE 0x200000  /* 对可执行的文件类型启用地址空间 layout 随机化 */
#define MH_NO_HEAP_EXECUTION 0x1000000 /* 将 Heap 标记为不可执行,可防止 heap spray 攻击 */
Mach-O 文件头主要目的是为加载命令提供信息。加载命令过程紧跟在头之后,并且 ncmds 和 sizeofcmds 来能个字段将会用在加载命令的过程中。

## Mach-O Data

加载命令在 Mach-O 文件加载解析时,会被内核加载器或者动态链接器调用。这些指令都采用 Type-Size-Value 这种格式,即:32 位的 cmd 值(表示类型),32 位的 cmdsize 值(32 位二级制位 4 的倍数,64 位位 8 的倍数),以及命令本身(由 cmdsize 指定的长度)。

在正式进入加载命令这一过程之前,先来学习一下 Mach-O 的 Data 区域,其中由 Segment 段和 Section 节组成。先来说 Segment 的组成,以下代码仍旧来自 loader.h:

#define	SEG_PAGEZERO	"__PAGEZERO" /* 当时 MH_EXECUTE 文件时,捕获到空指针 */
#define	SEG_TEXT	"__TEXT" /* 代码/只读数据段 */
#define	SEG_DATA	"__DATA" /* 数据段 */
#define	SEG_OBJC	"__OBJC" /* Objective-C runtime 段 */
#define	SEG_LINKEDIT	"__LINKEDIT" /* 包含需要被动态链接器使用的符号和其他表,包括符号表、字符串表等 */
进而来看一下 Segment 的数据结构具体是什么样的(同样这里也只放出 64 位的代码,与 32 位的区别就是其中 uint64_t 类型的几个字段取代了原先 32 位类型字段):

struct segment_command_64 { 
	uint32_t	cmd;		    /* LC_SEGMENT_64 */
	uint32_t	cmdsize;	    /* section_64 结构体所需要的空间 */
	char		segname[16];	/* segment 名字,上述宏中的定义 */
	uint64_t	vmaddr;		    /* 所描述段的虚拟内存地址 */
	uint64_t	vmsize;		    /* 为当前段分配的虚拟内存大小 */
	uint64_t	fileoff;	    /* 当前段在文件中的偏移量 */
	uint64_t	filesize;	    /* 当前段在文件中占用的字节 */
	vm_prot_t	maxprot;	    /* 段所在页所需要的最高内存保护,用八进制表示 */
	vm_prot_t	initprot;	    /* 段所在页原始内存保护 */
	uint32_t	nsects;		    /* 段中 Section 数量 */
	uint32_t	flags;		    /* 标识符 */
};
部分的 Segment (主要指的 __TEXT 和 __DATA)可以进一步分解为 Section。之所以按照 Segment -> Section 的结构组织方式,是因为在同一个 Segment 下的 Section,可以控制相同的权限,也可以不完全按照 Page 的大小进行内存对其,节省内存的空间。而 Segment 对外整体暴露,在程序载入阶段映射成一个完整的虚拟内存,更好的做到内存对齐(可以继续参考 OS X & iOS Kernel Programming 一书的第一章内容)。下面给出 Section 具体的数据结构:

struct section_64 { 
	char		sectname[16];	/* Section 名字 */
	char		segname[16];	/* Section 所在的 Segment 名称 */
	uint64_t	addr;		    /* Section 所在的内存地址 */
	uint64_t	size;		    /* Section 的大小 */
	uint32_t	offset;		    /* Section 所在的文件偏移 */
	uint32_t	align;		    /* Section 的内存对齐边界 (2 的次幂) */
	uint32_t	reloff;		    /* 重定位信息的文件偏移 */
	uint32_t	nreloc;		    /* 重定位条目的数目 */
	uint32_t	flags;		    /* 标志属性 */
	uint32_t	reserved1;	    /* 保留字段1 (for offset or index) */
	uint32_t	reserved2;	    /* 保留字段2 (for count or sizeof) */
	uint32_t	reserved3;	    /* 保留字段3 */
};

Section                         用途
__TEXT.__text	                主程序代码
__TEXT.__unwind_info	        用于存储处理异常情况信息
__TEXT.__eh_frame	            调试辅助信息
__TEXT.__cstring	            C 语言字符串
__TEXT.__const	                const 关键字修饰的常量
__TEXT.__stubs		            符号桩。本质上是一小段会直接跳入lazybinding的表对应项指针指向的地址的代码。
__TEXT.__stubs_helper	        辅助函数。上述提到的lazybinding的表中对应项的指针在没有找到真正的符号地址的时候,都指向这。
__TEXT.__objc_methname	        Objective-C 方法名称
__TEXT.__objc_methtype	        Objective-C 方法类型
__TEXT.__objc_classname	        Objective-C 类名称
__DATA.__data	                初始化过的可变数据
__DATA.__bss	                BSS存放未初始化的全局变量
__DATA.__la_symbol_ptr	        lazy binding 的指针表,表中的指针一开始都指向 __stub_helper
__DATA.__nl_symbol_ptr	        非 lazy binding 的指针表,每个表项中的指针都指向一个在装载过程中,被动态链机器搜索完成的符号
__DATA.__cfstring	            程序中使用的 Core Foundation 字符串(CFStringRefs)
__DATA.__common	                没有初始化过的符号声明
__DATA.__objc_classlist	        Objective-C 类列表
__DATA.__objc_protolist	        Objective-C 原型
__DATA.__objc_imginfo	        Objective-C 镜像信息
__DATA.__objc_selfrefs	        Objective-C self 引用
__DATA.__objc_protorefs	        Objective-C 原型引用
__DATA.__objc_superrefs	        Objective-C 超类引用
__DATA.__mod_init_func	        初始化函数,在main之前调用
__DATA.__mod_term_func	        终止函数,在main返回之后调用
