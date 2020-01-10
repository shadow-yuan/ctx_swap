; windows x64 version of the ctx_swap function
; ctx_swap(rcx, rdx);
.CODE
ctx_swap PROC
    mov rax, qword ptr[rsp]
    mov qword ptr[rcx + 104], rax
    mov qword ptr[rcx + 96], rbx
    mov qword ptr[rcx + 88], rcx
    mov qword ptr[rcx + 80], rdx
    mov rax, qword ptr [rax]
    mov qword ptr[rcx + 72], rax
    mov qword ptr[rcx + 64], rsi
    mov qword ptr[rcx + 56], rdi
    mov qword ptr[rcx + 48], rbp
    mov qword ptr[rcx + 40], r8
    mov qword ptr[rcx + 32], r9
    mov qword ptr[rcx + 24], r12
    mov qword ptr[rcx + 16], r13
    mov qword ptr[rcx + 8], r14
    mov qword ptr[rcx], r15

    xor rax, rax

    mov rbp, qword ptr[rdx + 48]
    mov rsp, qword ptr[rdx + 104]
    mov r15, qword ptr[rdx]
    mov r14, qword ptr[rdx + 8]
    mov r13, qword ptr[rdx + 16]
    mov r12, qword ptr[rdx + 24]
    mov r9,  qword ptr[rdx + 32]
    mov r8,  qword ptr[rdx + 40]
    mov rdi, qword ptr[rdx + 56]
    mov rsi, qword ptr[rdx + 64]
    mov rcx, qword ptr[rdx + 88]
    mov rbx, qword ptr[rdx + 96]
    ;mov rsp, qword ptr[rsp + 8]
    push qword ptr[rdx + 72]
	mov rdx, qword ptr[rdx + 80]
    ret
ctx_swap ENDP

END
