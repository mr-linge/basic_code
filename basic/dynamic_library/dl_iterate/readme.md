名称
dl_iterate_phdr-遍历共享对象列表

语法
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <link.h>

int dl_iterate_phdr(
          int (*callback) (struct dl_phdr_info *info,
                           size_t size, void *data),
          void *data);
说明
dl_iterate_phdr()函数允许应用程序在运行时进行查询，以了解其已加载了哪些共享库以及它们的加载顺序。

dl_iterate_phdr()函数遍历应用程序共享对象的列表，并为每个对象调用一次函数回调，直到所有共享对象都已处理或回调返回非零值为止。

每个对回调的调用都接收三个参数：info，它是指向包含有关共享库信息的结构的指针；大小，即info指向的结构的大小；和数据，它是调用程序作为对dl_iterate_phdr()的调用中的第二个参数(也称为数据)传递的任何值的副本。

info参数是以下类型的结构：

struct dl_phdr_info {
    ElfW(Addr)        dlpi_addr;  /* Base address of object */
    const char       *dlpi_name;  /* (Null-terminated) name of
                                     object */
    const ElfW(Phdr) *dlpi_phdr;  /* Pointer to array of
                                     ELF program headers
                                     for this object */
    ElfW(Half)        dlpi_phnum; /* # of items in dlpi_phdr */

    /* The following fields were added in glibc 2.4, after the first
       version of this structure was available.  Check the size
       argument passed to the dl_iterate_phdr callback to determine
       whether or not each later member is available.  */

    unsigned long long int dlpi_adds;
                    /* Incremented when a new object may
                       have been added */
    unsigned long long int dlpi_subs;
                    /* Incremented when an object may
                       have been removed */
    size_t dlpi_tls_modid;
                    /* If there is a PT_TLS segment, its module
                       ID as used in TLS relocations, else zero */
    void  *dlpi_tls_data;
                    /* The address of the calling thread's instance
                       of this module's PT_TLS segment, if it has
                       one and it has been allocated in the calling
                       thread, otherwise a null pointer */
};
(ElfW()宏定义将其参数转换为适合硬件体系结构的ELF数据类型的名称。例如，在32位平台上，ElfW(Addr)产生数据类型名称Elf32_Addr。有关这些类型的更多信息可以在和头文件中找到。)

dlpi_addr字段指示共享库的基地址(即，共享库的虚拟内存地址与该对象在从中加载该文件的文件中的偏移量之间的差)。 dlpi_name字段是一个以空字符结尾的字符串，提供了从中加载共享库的路径名。

为了理解dlpi_phdr和dlpi_phnum字段的含义，我们需要意识到ELF共享库由许多段组成，每个段都有一个描述该段的相应程序头。 dlpi_phdr字段是指向此共享库程序头数组的指针。 dlpi_phnum字段指示此数组的大小。

这些程序头是以下形式的结构：

typedef struct {
    Elf32_Word  p_type;    /* Segment type */
    Elf32_Off   p_offset;  /* Segment file offset */
    Elf32_Addr  p_vaddr;   /* Segment virtual address */
    Elf32_Addr  p_paddr;   /* Segment physical address */
    Elf32_Word  p_filesz;  /* Segment size in file */
    Elf32_Word  p_memsz;   /* Segment size in memory */
    Elf32_Word  p_flags;   /* Segment flags */
    Elf32_Word  p_align;   /* Segment alignment */
} Elf32_Phdr;
注意，我们可以使用以下公式计算特定程序头x在虚拟内存中的位置：

addr == info->dlpi_addr + info->dlpi_phdr[x].p_vaddr;
p_type的可能值包括以下内容(有关更多详细信息，请参见)：

#define PT_LOAD         1    /* Loadable program segment */
#define PT_DYNAMIC      2    /* Dynamic linking information */
#define PT_INTERP       3    /* Program interpreter */
#define PT_NOTE         4    /* Auxiliary information */
#define PT_SHLIB        5    /* Reserved */
#define PT_PHDR         6    /* Entry for header table itself */
#define PT_TLS          7    /* Thread-local storage segment */
#define PT_GNU_EH_FRAME 0x6474e550 /* GCC .eh_frame_hdr segment */
#define PT_GNU_STACK  0x6474e551 /* Indicates stack executability */
#define PT_GNU_RELRO  0x6474e552 /* Read-only after relocation */
返回值
dl_iterate_phdr()函数返回上次对回调的调用返回的值。
