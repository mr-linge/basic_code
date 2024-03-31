#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/dyld.h>
#include <mach/mach.h>
#include <mach/task.h>
#include <mach-o/dyld_images.h>

#import <Foundation/Foundation.h>
#import <Security/Security.h>

#include "apple/dyld_cache_format.h"

// void show_all_image_info() {
//     const char *image_name;
//     unsigned long vmaddr_slide;
//     const struct mach_header *image_header = NULL;
//     for (unsigned int i = 0 ; i < _dyld_image_count(); ++i) {
//         image_name = _dyld_get_image_name(i);
//         vmaddr_slide = _dyld_get_image_vmaddr_slide(i);
//         image_header = _dyld_get_image_header(i);
//         printf("%3d name:%s vmaddr_slide:0x%lx header:%p\n",i,image_name,vmaddr_slide,image_header);
//     }
// }

int lookup_module_info(const struct mach_header **image_header, char *module_name_pattern)
{
    const char *image_name = NULL;
    unsigned long vmaddr_slide;
    for (uint32_t i = 0; i < _dyld_image_count(); i++) {
        image_name = _dyld_get_image_name(i);
        if(strstr(image_name, module_name_pattern) != NULL) { // 采用模糊匹配,模块名包含module即返回
            *image_header = _dyld_get_image_header(i);
            vmaddr_slide = _dyld_get_image_vmaddr_slide(i);
            printf("%d name:%s vmaddr_slide:0x%lx header:%p\n",i,image_name,vmaddr_slide,*image_header);
            return 0;
        }
    }
    return -1;
}

void parse(const struct mach_header *image_header, char *symbol_name_pattern) {
    task_dyld_info_data_t task_dyld_info;
    mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
    kern_return_t ret = task_info(mach_task_self(), TASK_DYLD_INFO, (task_info_t)&task_dyld_info, &count);
    if (ret != KERN_SUCCESS) {
        printf("task_info failed, ret: %d\n", ret);
        return;
    }

    struct dyld_all_image_infos * infos = (struct dyld_all_image_infos *)(uintptr_t)task_dyld_info.all_image_info_addr;
    printf("version:                           %x\n",infos->version);
    printf("infoArrayCount:                    %x\n",infos->infoArrayCount);
    printf("dyldImageLoadAddress:              %p\n",infos->dyldImageLoadAddress);
    struct dyld_all_image_infos *dyldAllImageInfosAddress = infos->dyldAllImageInfosAddress;
    // printf("infos vaddr:                       %p\n",infos);
    printf("dyldAllImageInfosAddress:          %p\n",infos->dyldAllImageInfosAddress); // 就是 infos 内存地址,与上面一行相同
    uintptr_t sharedCacheSlide  = infos->sharedCacheSlide;
    printf("sharedCacheSlide:                  0x%lx\n",sharedCacheSlide);
    const char*						dyldVersion = infos->dyldVersion;
    printf("dyldVersion:                       %s\n",dyldVersion);
	  const char*						errorMessage = infos->errorMessage;
    printf("errorMessage:                      %s\n",errorMessage);
	  uintptr_t						terminationFlags = infos->terminationFlags;
    printf("terminationFlags:                  0x%lx\n",terminationFlags);
    struct dyld_cache_header  *shared_cache_base_address = (struct dyld_cache_header *)infos->sharedCacheBaseAddress;
    printf("shared_cache_base_address:         %p\n",shared_cache_base_address);
    uint64_t sharedRegionStart = shared_cache_base_address->sharedRegionStart;
    printf("sharedRegionStart:                 0x%llx\n",sharedRegionStart);
    uint64_t sharedRegionSize = shared_cache_base_address->sharedRegionSize;
    printf("sharedRegionSize:                  0x%llx\n",sharedRegionSize);

    // 判断待解析镜像 是否在 dyld cache 缓存中
    uintptr_t region_start = infos->sharedCacheBaseAddress;
    uintptr_t region_end = region_start + sharedRegionSize;
    printf("region_start:0x%lx,region_end:0x%lx\n",region_start,region_end);
    if (!((uintptr_t)image_header >= region_start && (uintptr_t)image_header < region_end)) {
        printf("This image is not within dyld share cache\n");
        return;
    }

    // 将 dyld 缓存的符号映射到内存空间, 为了获取字符串表
    const char *shared_cache_path = "/System/Library/Caches/com.apple.dyld/dyld_shared_cache_arm64.symbols";
    int fd = open(shared_cache_path, O_RDONLY, 0);
    struct stat st;
    int status = stat(shared_cache_path, &st);
    if (status != 0) {
        printf("mmap %s failed\n", shared_cache_path);
        return;
    }
    struct dyld_cache_header *symbols_file_mmap = (struct dyld_cache_header *)mmap(0, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    
    uint64_t localSymbolsOffset = symbols_file_mmap->localSymbolsOffset;
    struct dyld_cache_local_symbols_info * localInfo = (struct dyld_cache_local_symbols_info *) ((char *) symbols_file_mmap + localSymbolsOffset);
    struct dyld_cache_local_symbols_entry_64 * localEntries_64 = (struct dyld_cache_local_symbols_entry_64 *) ((char *) localInfo +
     localInfo->entriesOffset);

    struct nlist_64 *local_info_nlist_offset = (struct nlist_64 *) ((char *) localInfo + localInfo->nlistOffset);
    char *strtab = ((char *) localInfo) + localInfo->stringsOffset;
    if(!strtab) {
        printf("dyld cache string table is null!\n");
        return;
    }

    // 获取 symtab 符号表
    struct nlist_64 *symtab = NULL;
    uint32_t symtab_count = 0;

    const uint32_t entriesCount = localInfo->entriesCount;
    uint64_t textOffsetInCache = (uint64_t)image_header - (uint64_t)shared_cache_base_address;
    for (uint32_t i = 0; i < entriesCount; ++i) {
        if (localEntries_64[i].dylibOffset == textOffsetInCache) {
          uint32_t localNlistStart = localEntries_64[i].nlistStartIndex;
          symtab_count = localEntries_64[i].nlistCount;
          symtab = &local_info_nlist_offset[localNlistStart];
          break;
        }
    }

    if(!symtab) {
      printf("dyld cache symbol table is null!\n");
      return;
    }

    printf("\nsymtab detail list:\n");
    // 解析 symtab 符号表,获取 符号 和 地址
    for (uint32_t i = 0; i < symtab_count; i++) {
      if (symtab[i].n_value) {
          uint32_t strtab_offset = symtab[i].n_un.n_strx;
          char *symbol_name = strtab + strtab_offset;
          if(symbol_name_pattern == NULL) {
             printf("index:%d symbol_name:%s, offset:0x%llx, vaddr:0x%llx\n",i, symbol_name, symtab[i].n_value, symtab[i].n_value + sharedCacheSlide);
             continue;
          }
          if (strstr(symbol_name,symbol_name_pattern) != NULL) {
              printf("index:%d symbol_name:%s, offset:0x%llx, vaddr:0x%llx\n",i, symbol_name, symtab[i].n_value, symtab[i].n_value + sharedCacheSlide);
          }
      }
    }
}

void dylib_info(const void *addr) {
    char *error;
	Dl_info *info = (Dl_info *)malloc(sizeof(Dl_info));
	int status = dladdr(addr, info);
	// printf("status:%d\n",status);
	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, error);
		return;
	}
	printf("dli_fname:%s\n",info->dli_fname);
	printf("dli_fbase:%p\n",info->dli_fbase);
	printf("dli_sname:%s\n",info->dli_sname);
	printf("dli_saddr:%p\n",info->dli_saddr);
}

// https://opensource.apple.com/source/dyld/dyld-195.6/include/mach-o/dyld_images.h.auto.html
int main(int argc, char *argv[])
{
    if(!(argc >= 2 && argc <= 3)) {
        printf("param is not right!\n");
        return -1;
    }
	// show_all_image_info();
    // printf("dlopen\n");
    // dylib_info(&dlopen);
    // printf("dlsym\n");
    // dylib_info(&dlsym);
    // printf("printf\n");
    // dylib_info(&printf);
    // printf("puts\n");
    // dylib_info(&puts);
    // printf("malloc\n");
    // dylib_info(&malloc);
    printf("SecKeyCreateWithData\n");
    dylib_info(&SecKeyCreateWithData);

	char *module_name_pattern = argv[1];
    char *symbol_name_pattern = NULL;
    if(argc == 3) {
        symbol_name_pattern = argv[2];
    }

    if(!(module_name_pattern)) {
        printf("image pattern  must not be null\n");
        return -1;
    }

    const struct mach_header *image_header = NULL;
    int retval = lookup_module_info(&image_header, module_name_pattern);
    if(retval < 0) {
        printf("%s not find!\n", module_name_pattern);
        return -1;
    }

    parse(image_header, symbol_name_pattern);

	return 0;
}
