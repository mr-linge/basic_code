strip命令通过除去绑定程序和符号调试程序使用的信息，降低扩展公共对象文件格式（XCOFF）的对象文件的大小。

file xxx.so 可以查看文件是否经过 strip 处理。
经过strip 处理后的ELF文件 没有 section 信息、无法调试,所有不需要加载到进程空间的section都会删掉，也就是只有 section flag 为 ALLOC 的会保留.
此时 .symtab .strtab 完全就不存在了，无法在 .sybtab 里遍历查找符号信息.
但 .dynsym .dynstr 都还存在，可以通过遍历 .dynsym 查找符号信息.


.dynsym .dynstr 符号表所在数据结构
Elf64_Sym
结构如下:
typedef struct
{
  Elf64_Word	st_name;		/* Symbol name (string tbl index) */
  unsigned char	st_info;		/* Symbol type and binding */
  unsigned char st_other;		/* Symbol visibility */
  Elf64_Section	st_shndx;		/* Section index */
  Elf64_Addr	st_value;		/* Symbol value */
  Elf64_Xword	st_size;		/* Symbol size */
} Elf64_Sym;
其中 st_info 指定符号类型及绑定属性.
st_info
The st_info specifies the symbol type and binding attributes. For a complete list of 
these types and attributes, consult the ELF(5) man page. The symbol types start with 
STT whereas the symbol bindings start with STB. As an example, a few common 
ones are as explained in the next sections.
Symbol types
We've got the following symbol types:
• STT_NOTYPE: The symbols type is undefined
• STT_FUNC: The symbol is associated with a function or other executable code
• STT_OBJECT: The symbol is associated with a data object
Symbol bindings
We've got the following symbol bindings:
• STB_LOCAL: Local symbols are not visible outside the object file containing 
their definition, such as a function declared static.
• STB_GLOBAL: Global symbols are visible to all object files being combined. 
One file's definition of a global symbol will satisfy another file's undefined 
reference to the same symbol.
• STB_WEAK: Similar to global binding, but with less precedence, meaning that 
the binding is weak and may be overridden by another symbol (with the 
same name) that is not marked as STB_WEAK.
There are macros for packing and unpacking the binding and type fields:
• ELF32_ST_BIND(info) or ELF64_ST_BIND(info) extract a binding from an 
st_info value
• ELF32_ST_TYPE(info) or ELF64_ST_TYPE(info) extract a type from an 
st_info value
• ELF32_ST_INFO(bind, type) or ELF64_ST_INFO(bind, type) convert a 
binding and a type into an st_info value

这些 宏定义在 <elf.h> 中，需要时可以自己查看。
这里我列出 bind 和 type 宏对应的数字

/* Legal values for ST_BIND subfield of st_info (symbol binding).  */

#define STB_LOCAL	0		/* Local symbol */
#define STB_GLOBAL	1		/* Global symbol */
#define STB_WEAK	2		/* Weak symbol */
#define	STB_NUM		3		/* Number of defined types.  */
#define STB_LOOS	10		/* Start of OS-specific */
#define STB_GNU_UNIQUE	10		/* Unique symbol.  */
#define STB_HIOS	12		/* End of OS-specific */
#define STB_LOPROC	13		/* Start of processor-specific */
#define STB_HIPROC	15		/* End of processor-specific */

/* Legal values for ST_TYPE subfield of st_info (symbol type).  */

#define STT_NOTYPE	0		/* Symbol type is unspecified */
#define STT_OBJECT	1		/* Symbol is a data object */
#define STT_FUNC	2		/* Symbol is a code object */
#define STT_SECTION	3		/* Symbol associated with a section */
#define STT_FILE	4		/* Symbol's name is file name */
#define STT_COMMON	5		/* Symbol is a common data object */
#define STT_TLS		6		/* Symbol is thread-local data object*/
#define	STT_NUM		7		/* Number of defined types.  */
#define STT_LOOS	10		/* Start of OS-specific */
#define STT_GNU_IFUNC	10		/* Symbol is indirect code object */
#define STT_HIOS	12		/* End of OS-specific */
#define STT_LOPROC	13		/* Start of processor-specific */
#define STT_HIPROC	15		/* End of processor-specific */
