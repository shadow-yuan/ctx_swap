#ifndef __COCTX_SWAP_H__
#define __COCTX_SWAP_H__

#ifdef __cplusplus
extern "C" {
#endif

struct coctx_t;
void ctx_swap(coctx_t* cur_ctx, coctx_t* new_ctx);

#ifdef __cplusplus
}
#endif

#endif // __COCTX_SWAP_H__
