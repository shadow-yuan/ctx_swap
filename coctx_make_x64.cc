#include "coctx.h"

#ifdef _WIN64

enum {
    kRDX = 10,
    kRCX = 11,
};

int coctx_make(coctx_t* ctx, coroutine_proc pfn, const void* s, const void* s1) {
    char* sp = ctx->ss_sp + ctx->ss_size - sizeof(void*);
    sp = (char*)((unsigned long long)sp & -16LL);

    memset(ctx->regs, 0, sizeof(ctx->regs));
    void** ret_addr = (void**)(sp);
    *ret_addr = (void*)pfn;

    ctx->regs[kRSP] = sp;

    ctx->regs[kRETAddr] = (char*)pfn;

    ctx->regs[kRCX] = (char*)s;
    ctx->regs[kRDX] = (char*)s1;
    return 0;
}

#endif
