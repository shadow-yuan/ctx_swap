#include "ctx_swap.h"

__declspec(naked) void ctx_swap(coctx_t* cur_ctx, coctx_t* new_ctx) {
    __asm {
        mov eax, dword ptr[esp + 4]
        mov dword ptr[eax + 28], esp
        mov dword ptr[eax + 24], ebp
        mov dword ptr[eax + 20], esi
        mov dword ptr[eax + 16], edi
        mov dword ptr[eax + 12], edx
        mov dword ptr[eax + 8], ecx
        mov dword ptr[eax + 4], ebx

        mov eax, dword ptr[esp + 8]
        mov ebx, dword ptr[eax + 4]
        mov ecx, dword ptr[eax + 8]
        mov edx, dword ptr[eax + 12]
        mov edi, dword ptr[eax + 16]
        mov esi, dword ptr[eax + 20]
        mov ebp, dword ptr[eax + 24]
        mov esp, dword ptr[eax + 28]

        ret
    }
}
