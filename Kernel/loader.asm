global loader

extern main
extern init_kernel_binary

loader:
    call init_kernel_binary  ; Set up the kernel binary, and get thet stack address
    mov rsp,rax              ; Set up the stack with the returned address
    call main

hang:
    cli
    hlt                      ; halt machine should kernel return
    jmp hang
