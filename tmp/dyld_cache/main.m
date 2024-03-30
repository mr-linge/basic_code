#include <stdio.h>
#include <unistd.h>
#import <mach-o/dyld.h>
#include <mach/mach.h>
#include <mach/task.h>
#include <mach-o/dyld_images.h>

#import <Security/Security.h>

#include "parse.h"

void show_all_image_info() {
    unsigned int count = _dyld_image_count();
    const char *image_name;
    unsigned long vmaddr_slide;
    const struct mach_header *image_header = NULL;
    for (unsigned int i = 0 ; i < count; ++i) {
        image_name = _dyld_get_image_name(i);
        vmaddr_slide = _dyld_get_image_vmaddr_slide(i);
        image_header = _dyld_get_image_header(i);
        printf("%3d name:%s vmaddr_slide:0x%lx header:%p\n",i,image_name,vmaddr_slide,image_header);
    }
}

const struct mach_header * lookup_image(char *image_name, int flag)
{
	task_dyld_info_data_t task_dyld_info;
    mach_msg_type_number_t count = sizeof(task_dyld_info_data_t) / sizeof(natural_t);
    kern_return_t ret = task_info(mach_task_self(), TASK_DYLD_INFO, (task_info_t)&task_dyld_info, &count);
    if (ret != KERN_SUCCESS) {
        printf("task_info failed, ret: %d\n", ret);
        return NULL;
    }
    struct dyld_all_image_infos *infos = (struct dyld_all_image_infos *)task_dyld_info.all_image_info_addr;

	const struct dyld_image_info *infoArray = infos->infoArray;
    for(uint32_t i = 0; i < infos->infoArrayCount; i++) {
        struct dyld_image_info image_info = infoArray[i];
        const char *			    imageFilePath = image_info.imageFilePath;
		if (flag == 0) {
            if(strstr(imageFilePath, image_name) != NULL) { // 采用模糊匹配,模块名包含module即返回
                const struct mach_header *	imageLoadAddress = image_info.imageLoadAddress;
                printf("imageLoadAddress:%p,",imageLoadAddress);
                printf("imageFilePath:%s\n",imageFilePath);
				return imageLoadAddress;
            }
        }else {  
            if(strcmp(imageFilePath, image_name) == 0) {   // strcmp 准确匹配
                const struct mach_header *	imageLoadAddress = image_info.imageLoadAddress;
				return imageLoadAddress;
            }
        } 
    }

    return NULL;
}

// https://opensource.apple.com/source/dyld/dyld-195.6/include/mach-o/dyld_images.h.auto.html
int main(int argc, char *argv[])
{
	printf("****************** pid:%d *******************\n", getpid());
	show_all_image_info();

    task_dyld_info_data_t task_dyld_info;
    mach_msg_type_number_t count = sizeof(task_dyld_info_data_t) / sizeof(natural_t);
    kern_return_t ret = task_info(mach_task_self(), TASK_DYLD_INFO, (task_info_t)&task_dyld_info, &count);
    if (ret != KERN_SUCCESS) {
        printf("task_info failed, ret: %d\n", ret);
        return -1;
    }

    struct dyld_all_image_infos *infos = (struct dyld_all_image_infos *)task_dyld_info.all_image_info_addr;
    // printf("version:                           %x\n",infos->version);
    // printf("infoArrayCount:                    %x\n",infos->infoArrayCount);
    // printf("dyldImageLoadAddress:              %p\n",infos->dyldImageLoadAddress);
    // struct dyld_all_image_infos *dyldAllImageInfosAddress = infos->dyldAllImageInfosAddress;
    // // printf("infos vaddr:                       %p\n",infos);
    // printf("dyldAllImageInfosAddress:          %p\n",infos->dyldAllImageInfosAddress); // 就是 infos 内存地址,与上面一行相同
    // uintptr_t sharedCacheSlide  = infos->sharedCacheSlide;
    // printf("sharedCacheSlide:                  0x%lx\n",sharedCacheSlide);
    // const char*						dyldVersion = infos->dyldVersion;
    // printf("dyldVersion:                       %s\n",dyldVersion);
	// // const char*						errorMessage = infos->errorMessage;
    // // printf("errorMessage:                      %s\n",errorMessage);
	// // uintptr_t						terminationFlags = infos->terminationFlags;
    // // printf("terminationFlags:                  0x%lx\n",terminationFlags);
    // // struct dyld_all_image_infos *shared_cache = (struct dyld_cache_header *)infos->sharedCacheBaseAddress;
    // // printf("shared_cache:                      %p\n",shared_cache );

    // printf("images not in dyld shared cache:\n");
    // uintptr_t						uuidArrayCount = infos->uuidArrayCount;
	// const struct dyld_uuid_info *	uuidArray = infos->uuidArray;
    // for(uint32_t i = 0; i < uuidArrayCount; i++) {
    //     struct dyld_uuid_info uuid_info = uuidArray[i];
    //     const struct mach_header *	imageLoadAddress = uuid_info.imageLoadAddress;	/* base address image is mapped into */
	//     unsigned char *          		   imageUUID = uuid_info.imageUUID;
    //     printf("imageLoadAddress:%p",imageLoadAddress);
    //     printf(",imageUUID:");
    //     for(uint32_t j = 0; j < 16; j++) {
    //         printf("%02X",(unsigned int)imageUUID[j]);
    //     }
    //     printf("\n");
    // }

    printf("dyld_image_info:\n");
    // const struct dyld_image_info *infoArray = infos->infoArray;
    // for(uint32_t i = 0; i < infos->infoArrayCount; i++) {
    //     struct dyld_image_info image_info = infoArray[i];
    //     const struct mach_header *	imageLoadAddress = image_info.imageLoadAddress;	/* base address image is mapped into */
    //     const char *			    imageFilePath = image_info.imageFilePath;		/* path dyld used to load the image */
    //     uintptr_t					imageFileModDate = image_info.imageFileModDate;
    //     printf("imageLoadAddress:%p,",imageLoadAddress);
    //     printf("imageFilePath:%s,",imageFilePath);
    //     printf("imageFileModDate:%lu\n",imageFileModDate);
    // }
    // struct dyld_image_info image_info = infoArray[1];
    // const char *			    imageFilePath = image_info.imageFilePath;
    // const struct mach_header *	imageLoadAddress = image_info.imageLoadAddress;	
    // printf("imageLoadAddress:%p,",imageLoadAddress);
    // printf("imageFilePath:%s\n",imageFilePath);
    // parse((void *)imageLoadAddress);

    char *image_name = "libobjc.A.dylib";
    const struct mach_header *image_header = lookup_image(image_name, 0);
    // printf("%s vaddr:%p\n", image_name, image_header);
    if(!image_header) {
        printf("%s not find!\n", image_name);
        return 0;
    }
    parse((void *)image_header);

	return 0;
}
