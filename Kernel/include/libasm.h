#ifndef LIBASM_H
#define LIBASM_H

#include <stdint.h>

// defined in asm/libasm.asm
extern char* asm_cpu_vendor(char* result);
extern int asm_rtc_gettime(int fmt);
extern char asm_kbd_active();
extern int asm_kbd_getkey();
extern uint64_t asm_getsp();
extern void asm_setreg();
extern void asm_printreg(uint32_t color);
extern void asm_sound(uint32_t freq, uint32_t time);
extern void asm_nosound();

#endif
