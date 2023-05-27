global asm_cli
global asm_sti
global asm_hlt

global asm_pic_master_mask
global asm_pic_slave_mask

global asm_irq00_handler
global asm_irq01_handler
global asm_irq02_handler
global asm_irq03_handler
global asm_irq04_handler
global asm_irq05_handler

global asm_syscall_handler
global asm_exception00_handler
global asm_exception01_handler

extern irq_dispatcher
extern exception_dispatcher
extern syscall_dispatcher
extern asm_print_regs

section .text

%macro push_state 0
   push rbx
   push rcx
   push rdx
   push rbp
   push rdi
   push rsi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15
%endmacro

%macro push_state_full 0
   push rax
   push_state
%endmacro

%macro pop_state 0
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rsi
   pop rdi
   pop rbp
   pop rdx
   pop rcx
   pop rbx
%endmacro

%macro pop_state_full 0
    pop_state
    pop rax
%endmacro

%macro irq_handler 1
   push rsp
   push_state_full

   mov rdi,%1 ; pasaje de parametro
   call irq_dispatcher

   ; signal pic EOI (End of Interrupt)
   mov al,20h
   out 20h,al

   pop_state_full
   pop rsp
   iretq
%endmacro

%macro excepction_handler 1
   ;mov rdi,1
   ;mov rsi,2
   ;mov rdx,3
   ;mov r10,4
   ;mov r9,5
   ;mov r8,6
   ; TEST PARA LA EXCEPTION
   push_state_full

   mov rdi,%1   ; pasaje de parametro
   mov rsi,rsp  ; paso el stack-pointer para imprimir los registros
   call exception_dispatcher

   pop_state_full

   iretq
   ;ret  ; UNA VEZ QUE TERMINEMOS EL TESTING VOLVER A IRETQ
%endmacro


asm_hlt:
    sti
    hlt
    ret

asm_cli:
    cli
    ret

asm_sti:
    sti
    ret

asm_pic_master_mask:
    push rbp
    mov rbp,rsp

    mov ax,di
    out 21h,al

    pop rbp
    retn

asm_pic_slave_mask:
    push rbp
    mov rbp,rsp

    mov ax,di  ; ax = mascara de 16 bits
    out 0A1h,al

    pop rbp
    retn

; 8254 Timer (Timer Tick)
asm_irq00_handler:
    irq_handler 0

; Keyboard interrupt. IRQ 0x01, INT 0x21
asm_irq01_handler:
    irq_handler 1

; Cascade pic never called
asm_irq02_handler:
   irq_handler 2

; Serial Port 2 and 4
asm_irq03_handler:
   irq_handler 3

; Serial Port 1 and 3
asm_irq04_handler:
   irq_handler 4

; USB
asm_irq05_handler:
   irq_handler 5

; Zero Division Exception
asm_exception00_handler:
   excepction_handler 0

; Invalid Operator Exception
asm_exception01_handler:
   excepction_handler 1

; Calls the syscall handler
asm_syscall_handler:
   push_state
   call syscall_dispatcher
   pop_state
   iretq
