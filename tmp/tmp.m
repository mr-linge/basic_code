
struct dyld_cache_header *shared_cache_get_load_addr() {
    // task info
    task_dyld_info_data_t task_dyld_info;
    mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
    kern_return_t ret = task_info(mach_task_self(), TASK_DYLD_INFO, (task_info_t)&task_dyld_info, &count);
    if (ret != KERN_SUCCESS) {
        printf("task_info failed, ret: %d\n", ret);
        return NULL;
    }

    // get dyld load address
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
    struct dyld_cache_header  *shared_cache = (struct dyld_cache_header *)infos->sharedCacheBaseAddress;
    printf("shared_cache:                      %p\n",shared_cache );

    return shared_cache;
}

int shared_cache_load_symbols(shared_cache_ctx_t *ctx) {
    int retval = 0;
    uint64_t localSymbolsOffset = 0;

    const char *shared_cache_path = "/System/Library/Caches/com.apple.dyld/dyld_shared_cache_arm64.symbols";

    int fd = open(shared_cache_path, O_RDONLY, 0);

    struct stat st;
    retval = stat(shared_cache_path, &st);
    if (retval != 0) {
        printf("mmap %s failed\n", shared_cache_path);
        return -1;
    }

    void * mmap_buffer = (uint8_t *)mmap(0, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    ctx->mmap_shared_cache = mmap_buffer;
    localSymbolsOffset = ctx->mmap_shared_cache->localSymbolsOffset;

    ctx->latest_shared_cache_format = true;//latest_shared_cache_format;

    struct dyld_cache_header * mmap_shared_cache = ctx->mmap_shared_cache;
    struct dyld_cache_local_symbols_info * localInfo = (struct dyld_cache_local_symbols_info *) ((char *) mmap_shared_cache + localSymbolsOffset);
    ctx->local_symbols_info = localInfo;

    // if (ctx->latest_shared_cache_format) {
        struct dyld_cache_local_symbols_entry_64 * localEntries_64 = (struct dyld_cache_local_symbols_entry_64 *) ((char *) localInfo +
                                                                                 localInfo->entriesOffset);
        ctx->local_symbols_entries_64 = localEntries_64;
    // } else {
    //     struct dyld_cache_local_symbols_entry * localEntries = (struct dyld_cache_local_symbols_entry *) ((char *) localInfo +
    //                                                                        localInfo->entriesOffset);
    //     ctx->local_symbols_entries = localEntries;
    // }

    ctx->symtab = (nlist_t *) ((char *) localInfo + localInfo->nlistOffset);
    ctx->strtab = ((char *) localInfo) + localInfo->stringsOffset;

    return 0;
}

int shared_cache_ctx_init(shared_cache_ctx_t *ctx) {
  memset(ctx, 0, sizeof(shared_cache_ctx_t));

  struct dyld_cache_header  *runtime_shared_cache = shared_cache_get_load_addr();
  if (!runtime_shared_cache) {
    return -1;
  }
  ctx->runtime_shared_cache = runtime_shared_cache;

  // shared cache slide
  struct dyld_cache_mapping_info * mappings = (struct dyld_cache_mapping_info *)((char *)runtime_shared_cache + runtime_shared_cache->mappingOffset);
  uintptr_t slide = (uintptr_t)runtime_shared_cache - (uintptr_t)(mappings[0].address);
  ctx->runtime_slide = slide;

  return 0;
}

bool shared_cache_is_contain(shared_cache_ctx_t *ctx, addr_t addr, size_t length) {
  struct dyld_cache_header *runtime_shared_cache;
  runtime_shared_cache = ctx->runtime_shared_cache;

  addr_t region_start = runtime_shared_cache->sharedRegionStart + ctx->runtime_slide;
  addr_t region_end = region_start + runtime_shared_cache->sharedRegionSize;
  if (addr >= region_start && addr < region_end)
    return true;

  return false;
}

int shared_cache_get_symbol_table(shared_cache_ctx_t *ctx, mach_header_t *image_header, nlist_t **out_symtab,
                                  uint32_t *out_symtab_count, char **out_strtab) {
  uint64_t textOffsetInCache = (uint64_t)image_header - (uint64_t)ctx->runtime_shared_cache;
  nlist_t *localNlists = NULL;
  uint32_t localNlistCount = 0;
  const char *localStrings = NULL;

  const uint32_t entriesCount = ctx->local_symbols_info->entriesCount;
  for (uint32_t i = 0; i < entriesCount; ++i) {
      if (ctx->local_symbols_entries_64[i].dylibOffset == textOffsetInCache) {
        printf("%d textOffsetInCache:0x%llx\n",__LINE__,textOffsetInCache);
        uint32_t localNlistStart = ctx->local_symbols_entries_64[i].nlistStartIndex;
        localNlistCount = ctx->local_symbols_entries_64[i].nlistCount;
        localNlists = &ctx->symtab[localNlistStart];
        break;
      }
  }
  *out_symtab = localNlists;
  *out_symtab_count = (uint32_t)localNlistCount;
  *out_strtab = (char *)ctx->strtab;
  return 0;
}

uintptr_t test3(uint32_t index) {
    uintptr_t result = 0;
    // char * symbol_name_pattern = "sqlite3_open";//"SecKeyCreateWithData";
    const struct mach_header *header = _dyld_get_image_header(index);

    nlist_t *symtab = NULL;
    uint32_t symtab_count = 0;
    char *strtab = NULL;

    static shared_cache_ctx_t shared_cache_ctx;
    printf("%d shared_cache_ctx.runtime_slide:0x%lx\n",__LINE__,shared_cache_ctx.runtime_slide);
    shared_cache_ctx_init(&shared_cache_ctx);
    shared_cache_load_symbols(&shared_cache_ctx);
    if (shared_cache_ctx.mmap_shared_cache) {
      // shared cache library
      if (shared_cache_is_contain(&shared_cache_ctx, (addr_t)header, 0)) {
        printf("%d is_contain\n",__LINE__);
        shared_cache_get_symbol_table(&shared_cache_ctx, (mach_header_t *)header, &symtab, &symtab_count, &strtab);
      }
    }
    // printf("%d symtab->n_un.n_str:  0x%x\n",__LINE__,symtab->n_un.n_strx);
    // printf("%d symtab->n_sect:      0x%hhx\n",__LINE__,symtab->n_sect);
    // printf("%d symtab->n_desc:      0x%hx\n",__LINE__,symtab->n_desc);
    // printf("%d symtab->n_value:     0x%llx\n",__LINE__,symtab->n_value);
    // printf("%d symtab_count:        0x%x\n",__LINE__,symtab_count);
    // printf("%d strtab:              %s\n",__LINE__,strtab);

    if (symtab && strtab) {
        for (uint32_t i = 0; i < symtab_count; i++) {
          if (symtab[i].n_value) {
            uint32_t strtab_offset = symtab[i].n_un.n_strx;
            char *symbol_name = strtab + strtab_offset;
            // if (strstr(symbol_name,"SecKey") != NULL) {
                printf("index:%d symbol_name:%s, offset:0x%llx, vaddr:0x%llx\n",i, symbol_name, symtab[i].n_value, symtab[i].n_value +shared_cache_ctx.runtime_slide);
            // }
          }
        }
    }
    return result;
}