#include "parse.h"
#import <mach-o/loader.h>

void parse(void *mem) {
    unsigned long offset = 0;
    
    printf("-----------------> header <-----------------\n");
    struct mach_header_64 *header = (struct mach_header_64 *)mem;
    printf("magic:      %08X\t",header->magic);
    if (header->magic == MH_MAGIC_64) {
        printf("MH_MAGIC_64\n");
    }else if(header->magic == MH_CIGAM_64) {
        printf("MH_CIGAM_64\n");
    }else if(header->magic == MH_MAGIC) {
        printf("MH_MAGIC\n");
    }else if(header->magic == MH_CIGAM) {
        printf("MH_CIGAM\n");
    }else {
        printf("\n");
    }
    printf("cputype:    %08X\n",header->cputype);
    printf("cpusubtype: %08X\n",header->cpusubtype);
    printf("filetype:   %08X\t",header->filetype);
    if (header->filetype == MH_EXECUTE) {
        printf("MH_EXECUTE\n");
    }else if (header->filetype == 0x6) {
        printf("MH_DYLIB\n");
    }else {
        printf("\n");
    }
    printf("ncmds:      %08X\n",header->ncmds);       // Number of Load commands
    printf("sizeofcmds: %08X\n",header->sizeofcmds);  // Size of Load commands
    printf("flags:      %08X\n",header->flags);
    printf("reserved:   %08X\n",header->reserved);
    offset += sizeof(struct mach_header_64);
    
    printf("-----------------> Load commands <-----------------\n");
    for(uint32_t i = 0; i < header->ncmds; i++) {
        printf("---------> Load command[0x%x] <---------\n",i);
        printf("offset:0x%08lX\n",offset);
        struct load_command * load_commands = (struct load_command *)(&mem[offset]);
        uint32_t    cmd = load_commands->cmd;            /* LC_SEGMENT_64 */
        uint32_t    cmdsize = load_commands->cmdsize;    /* includes sizeof section_64 structs */
        printf("cmd:                %08X\n",cmd);
        printf("cmdsize:            %08X\n",cmdsize);
        if(cmd == LC_SEGMENT_64) {
            printf("cmd name:           LC_SEGMENT_64\n");
            struct segment_command_64 *segment_command = (struct segment_command_64 *)(&mem[offset]);
            char        segname[16];    /* segment name */
            memcpy(segname, segment_command->segname, 16);
            uint64_t    vmaddr = segment_command->vmaddr;        /* memory address of this segment */
            uint64_t    vmsize = segment_command->vmsize;        /* memory size of this segment */
            uint64_t    fileoff = segment_command->fileoff;    /* file offset of this segment */
            uint64_t    filesize = segment_command->filesize;    /* amount to map from the file */
            int32_t     maxprot = segment_command->maxprot;    /* maximum VM protection */
            int32_t     initprot = segment_command->initprot;    /* initial VM protection */
            uint32_t    nsects = segment_command->nsects;        /* number of sections in segment */
            uint32_t    flags = segment_command->flags;        /* flags */
            printf("segname:            %s\n",segname);
            printf("vmaddr:             %016llX\n",vmaddr);
            printf("vmsize:             %016llX\n",vmsize);
            printf("fileoff:            %016llX\n",fileoff);
            printf("filesize:           %016llX\n",filesize);
            printf("maxprot:            %016X\n",maxprot);
            printf("initprot:           %016X\n",initprot);
            printf("nsects:             %016X\n",nsects);
            printf("flags:              %016X\n",flags);
            
            unsigned long section_size = sizeof(struct section_64);
            //            printf("section_size:0x%lx\n",section_size);
            unsigned long section_start_offset = offset + sizeof(struct segment_command_64);
            for(unsigned long j = 0; j < segment_command->nsects; j++) {
                printf("----> Section[0x%lx] <----\n",j);
                struct section_64 *section = (struct section_64 *)(&mem[section_start_offset + j * section_size]);
                char        sectname[16];    /* name of this section */
                memcpy(sectname, section->sectname, 16);
                char        segname[16];    /* segment this section goes in */
                memcpy(segname, section->segname, 16);
                uint64_t    addr = section->addr;        /* memory address of this section */
                uint64_t    size = section->size;        /* size in bytes of this section */
                uint32_t    offset = section->offset;        /* file offset of this section */
                uint32_t    align = section->align;        /* section alignment (power of 2) */
                uint32_t    reloff = section->reloff;        /* file offset of relocation entries */
                uint32_t    nreloc = section->nreloc;        /* number of relocation entries */
                uint32_t    flags = section->flags;        /* flags (section type and attributes)*/
                uint32_t    reserved1 = section->reserved1;    /* reserved (for offset or index) */
                uint32_t    reserved2 = section->reserved2;    /* reserved (for count or sizeof) */
                uint32_t    reserved3 = section->reserved3;    /* reserved */
                printf("sectname:               %s\n",sectname);
                printf("segname:                %s\n",segname);
                printf("addr:                   %llX\n",addr);
                printf("size:                   %llX\n",size);
                printf("offset:                 %X\n",offset);
                printf("align:                  %X\n",align);
                printf("reloff:                 %X\n",reloff);
                printf("nreloc:                 %X\n",nreloc);
                printf("flags:                  %X\n",flags);
                printf("reserved1:              %X\n",reserved1);
                printf("reserved2:              %X\n",reserved2);
                printf("reserved3:              %X\n",reserved3);
            }
        }else if (cmd == LC_SYMTAB) {
            printf("cmd name:           LC_SYMTAB\n");
            struct symtab_command *command;
            command = (struct symtab_command *)(&mem[offset]);
            uint32_t    symoff = command->symoff;           /* symbol table offset */
            uint32_t    nsyms = command->nsyms;             /* number of symbol table entries */
            uint32_t    stroff = command->stroff;           /* string table offset */
            uint32_t    strsize = command->strsize;         /* string table size in bytes */
            printf("symoff:             %08X\n",symoff);
            printf("nsyms:              %08X\n",nsyms);
            printf("stroff:             %08X\n",stroff);
            printf("strsize:            %08X\n",strsize);
            
            struct symseg_command *symseg_cmd;
            for (unsigned long i = 0; i < nsyms; i++) {
                printf("----> LC_SYMSEG command[0x%lx] <----\n",i);
                unsigned long symseg_offset = symoff + (i * sizeof(struct symseg_command));
                printf("offset:             %lX\n",symseg_offset);
                symseg_cmd = (struct symseg_command *)&mem[symseg_offset];
                uint32_t    cmd = symseg_cmd->cmd;                  /* LC_SYMSEG */
                uint32_t    cmdsize = symseg_cmd->cmdsize;          /* sizeof(struct symseg_command) */
                uint32_t    offset = symseg_cmd->offset;;           /* symbol segment offset */
                uint32_t    size = symseg_cmd->size;;               /* symbol segment size in bytes */
                unsigned char *symbol_value = &mem[stroff + cmd];
                printf("cmd:                %08X\n", cmd);
                printf("cmdsize:            %08X\n", cmdsize);
                printf("offset:             %08X\n", offset);
                printf("size:               %08X\n", size);
                printf("symbol:             %s\n",symbol_value);
            }
        }else if (cmd == LC_DYSYMTAB) {
            printf("cmd name:           LC_DYSYMTAB\n");
            struct dysymtab_command *command = (struct dysymtab_command *)(&mem[offset]);
            uint32_t ilocalsym = command->ilocalsym;    /* index to local symbols */
            uint32_t nlocalsym = command->nlocalsym;    /* number of local symbols */
            uint32_t iextdefsym = command->iextdefsym;/* index to externally defined symbols */
            uint32_t nextdefsym = command->nextdefsym;/* number of externally defined symbols */
            uint32_t iundefsym = command->iundefsym;    /* index to undefined symbols */
            uint32_t nundefsym = command->nundefsym;    /* number of undefined symbols */
            uint32_t tocoff = command->tocoff;    /* file offset to table of contents */
            uint32_t ntoc = command->ntoc;    /* number of entries in table of contents */
            uint32_t modtaboff = command->modtaboff;    /* file offset to module table */
            uint32_t nmodtab = command->nmodtab;    /* number of module table entries */
            uint32_t extrefsymoff = command->extrefsymoff;    /* offset to referenced symbol table */
            uint32_t nextrefsyms = command->nextrefsyms;    /* number of referenced symbol table entries */
            uint32_t indirectsymoff = command->indirectsymoff; /* file offset to the indirect symbol table */
            uint32_t nindirectsyms = command->nindirectsyms;  /* number of indirect symbol table entries */
            uint32_t extreloff = command->extreloff;    /* offset to external relocation entries */
            uint32_t nextrel = command->nextrel;    /* number of external relocation entries */
            uint32_t locreloff = command->locreloff;    /* offset to local relocation entries */
            uint32_t nlocrel = command->nlocrel;    /* number of local relocation entries */
            printf("ilocalsym:          %08X\n",ilocalsym);
            printf("nlocalsym:          %08X\n",nlocalsym);
            printf("iextdefsym:         %08X\n",iextdefsym);
            printf("nextdefsym:         %08X\n",nextdefsym);
            printf("iundefsym:          %08X\n",iundefsym);
            printf("nundefsym:          %08X\n",nundefsym);
            printf("tocoff:             %08X\n",tocoff);
            printf("ntoc:               %08X\n",ntoc);
            printf("modtaboff:          %08X\n",modtaboff);
            printf("nmodtab:            %08X\n",nmodtab);
            printf("extrefsymoff:       %08X\n",extrefsymoff);
            printf("nextrefsyms:        %08X\n",nextrefsyms);
            printf("indirectsymoff:     %08X\n",indirectsymoff);
            printf("nindirectsyms:      %08X\n",nindirectsyms);
            printf("extreloff:          %08X\n",extreloff);
            printf("nextrel:            %08X\n",nextrel);
            printf("locreloff:          %08X\n",locreloff);
            printf("nlocrel:            %08X\n",nlocrel);
        }else if (cmd == LC_LOAD_DYLINKER) {
            printf("cmd name:           LC_LOAD_DYLINKER\n");
            struct dylinker_command *command = (struct dylinker_command *)(&mem[offset]);
            union lc_str    name = command->name;        /* dynamic linker's path name */
            //            printf("offset:             %08x\n",name.offset);
            char *linker_path = (char *)&mem[offset + name.offset];
            printf("linker_path:        %s\n",linker_path);
        }else if (cmd == LC_UUID) {
            printf("cmd name:           LC_UUID\n");
            struct uuid_command *command = (struct uuid_command *)(&mem[offset]);
            uint8_t    uuid_value[16];    /* the 128-bit uuid */
            memcpy(uuid_value, command->uuid, 16);
            printf("uuid:               ");
            for(int i = 0; i < 16; i++) {
                uint8_t c = uuid_value[i];
                printf("%X",c);
            }
            printf("\n");
        }else if (cmd == LC_SOURCE_VERSION) {
            printf("cmd name:           LC_SOURCE_VERSION\n");
            struct source_version_command *command = (struct source_version_command *)(&mem[offset]);
            uint64_t  version = command->version;       /* A.B.C.D.E packed as a24.b10.c10.d10.e10 */
            printf("version:            %08llX\n",version);
        }else if (cmd == LC_MAIN) {
            printf("cmd name:           LC_MAIN\n");
            struct entry_point_command *command = (struct entry_point_command *)(&mem[offset]);
            uint64_t  entryoff = command->entryoff;     /* file (__TEXT) offset of main() */
            uint64_t  stacksize = command->stacksize;   /* if not zero, initial stack size */
            printf("entryoff:           %016llX\n",entryoff);
            printf("stacksize:          %016llX\n",stacksize);
        }else if (cmd == LC_ENCRYPTION_INFO_64) {
            printf("cmd name:           LC_ENCRYPTION_INFO_64\n");
            struct encryption_info_command *command = (struct encryption_info_command *)(&mem[offset]);
            uint32_t    cryptoff = command->cryptoff;       /* file offset of encrypted range */
            uint32_t    cryptsize = command->cryptsize;     /* file size of encrypted range */
            uint32_t    cryptid = command->cryptid;         /* which enryption system, 0 means not-encrypted yet */
            printf("cryptoff:           %08X\n",cryptoff);
            printf("cryptsize:          %08X\n",cryptsize);
            printf("cryptid:            %08X\n",cryptid);
        }else if (cmd == LC_LOAD_DYLIB) {
            printf("cmd name:           LC_LOAD_DYLIB\n");
            struct dylib_command *command = (struct dylib_command *)(&mem[offset]);
            struct dylib dynamic_library;
            dynamic_library = command->dylib;
            union lc_str  name = dynamic_library.name;                                  /* library's path name */
            char *dylib_path = (char *)&mem[offset + name.offset];
            uint32_t timestamp = dynamic_library.timestamp;                             /* library's build time stamp */
            uint32_t current_version = dynamic_library.current_version;                 /* library's current version number */
            uint32_t compatibility_version = dynamic_library.compatibility_version;     /* library's compatibility vers number */
            printf("timestamp:                  %x\n",timestamp);
            printf("current_version:            %x\n",current_version);
            printf("compatibility_version:      %x\n",compatibility_version);
            printf("dylib path:                 %s\n",dylib_path);
        }else if (cmd == LC_FUNCTION_STARTS) {
            printf("cmd name:           LC_FUNCTION_STARTS\n");
            struct linkedit_data_command *command = (struct linkedit_data_command *)(&mem[offset]);
            uint32_t    dataoff = command->dataoff;         /* file offset of data in __LINKEDIT segment */
            uint32_t    datasize = command->datasize;       /* file size of data in __LINKEDIT segment  */
            printf("dataoff:                    %08X\n",dataoff);
            printf("datasize:                   %08X\n",datasize);
            
            uint8_t *func_ptr = (uint8_t *)&mem[dataoff];
            for (uint32_t i = 0; i < datasize; i++) {
                printf("%02X ", (unsigned int)func_ptr[i]);
                if ((i + 1) % 0x10 == 0)
                {
                    puts("");
                }
            }
            printf("\n");
        }else if (cmd == LC_DATA_IN_CODE) {
            printf("cmd name:           LC_DATA_IN_CODE\n");
            struct linkedit_data_command *command = (struct linkedit_data_command *)(&mem[offset]);
            uint32_t    dataoff = command->dataoff;         /* file offset of data in __LINKEDIT segment */
            uint32_t    datasize = command->datasize;       /* file size of data in __LINKEDIT segment  */
            printf("dataoff:                    %08X\n",dataoff);
            printf("datasize:                   %08X\n",datasize);
        }else if (cmd == LC_CODE_SIGNATURE) {
            printf("cmd name:           LC_CODE_SIGNATURE\n");
            struct linkedit_data_command *command = (struct linkedit_data_command *)(&mem[offset]);
            uint32_t    dataoff = command->dataoff;         /* file offset of data in __LINKEDIT segment */
            uint32_t    datasize = command->datasize;       /* file size of data in __LINKEDIT segment  */
            printf("dataoff:                    %08X\n",dataoff);
            printf("datasize:                   %08X\n",datasize);
        }
        
        offset += (unsigned long)cmdsize;
    }
}
