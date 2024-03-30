#include <stdio.h>
#include <unistd.h>
#import <mach-o/dyld.h>
#include <mach/mach.h>
#include <mach/task.h>
#include <mach-o/dyld_images.h>

#import <Security/Security.h>

#include "./dobby_symbol_resolver.h"

int main()
{
    void *vaddr = DobbySymbolResolver("Security", "SecKeyCreateWithData");
    printf("vaddr:%p\n",vaddr);
    return 0;
}