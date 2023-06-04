#include <exceptions.h>
#include <libc.h>
#include <text.h>
#include <video.h>

#define BUFF_SIZE 30
#define ERROR_MSG 0xff0000
#define INFO_MSG 0x0000ff

enum exceptions
{
	ZERO_DIVISION = 0,
	INVALID_OPCODE = 6
};

typedef struct
{
	uint64_t ip, sp;
} RestorePoint;

static char* registers[] = {
	"R15: 0x", "R14: 0x", "R13: 0x", "R12: 0x", "R11: 0x", "R10: 0x", "R9:  0x", "R8:  0x", "RSI: 0x",
	"RDI: 0x", "RBP: 0x", "RDX: 0x", "RCX: 0x", "RBX: 0x", "RAX: 0x", "IP:  0x", "RSP: 0x",
};
static uint32_t registers_len = sizeof(registers) / sizeof(registers[0]);
static char buff[BUFF_SIZE];
static RestorePoint rp;

static void exception_handler(char* msg);
static void restore_state(uint64_t* stack);

void
exception_dispatcher(uint32_t exception, uint64_t* stack)
{
	switch (exception) {
		case ZERO_DIVISION: {
			exception_handler("Zero Division Error Exception");
		} break;

		case INVALID_OPCODE: {
			exception_handler("Invalid Opcode Exception");
		} break;
	}
	exc_printreg(stack, ERROR_MSG);
	restore_state(stack);
}

void
exc_printreg(uint64_t* stack, uint32_t color)
{
	// stack [] = [dir_ret,r15,r14,r13,r12,r11,r10,r9,r8,rsi,rdi,rbp,rdx,rcx,rbx,rax, RIP, CS, EFLAGS, RSP]    si vengo
	// desde interrupción ioe stack [] = [dir_ret, r15,r14,r13,r12,r11,r10,r9,r8,rsi,rdi,rbp,rdx,rcx,rbx,rax, RIP, CS,
	// EFLAGS, RSP]    si vengo desde interrupción zde

	if (stack == 0) {
		tx_put_word("You have to press 'Ctrl+r' to set the registers at some point\n", color);
		return;
	}

	for (int i = 0; i < registers_len - 1; i++) {
		tx_put_word(registers[i], color);
		uint_to_base(stack[i], buff, HEX);
		tx_put_word(buff, color);
		tx_put_char('\n', color);
	}

	tx_put_word(registers[registers_len - 1], color);
	uint_to_base((uint64_t)stack[registers_len - 1], buff, HEX);
	tx_put_word(buff, color);
	tx_put_char('\n', color);
}

void
exc_set_restore_point(uint64_t ip, uint64_t sp)
{
	rp.ip = ip;
	rp.sp = sp;
}

static void
exception_handler(char* msg)
{
	tx_put_word(msg, ERROR_MSG);
	tx_put_char('\n', ERROR_MSG);
}

static void
restore_state(uint64_t* stack)
{
	// stack [] = [dir_ret, 0 ,r15,r14,r13,r12,r11,r10,r9,r8,rsi,rdi,rbp,rdx,rcx,rbx,rax,rbp, RIP, CS, EFLAGS,
	// RSP(viejo)] vd_set_color(0xFFFFFF, 0x2020CF);
	tx_put_word("Restoring state from: IP=0x", INFO_MSG);
	uint_to_base(rp.ip, buff, HEX);
	tx_put_word(buff, INFO_MSG);
	tx_put_word("  SP=0x", INFO_MSG);
	uint_to_base(rp.sp, buff, HEX);
	tx_put_word(buff, INFO_MSG);
	tx_put_word("\n\n", INFO_MSG);

	stack[registers_len - 2] = rp.ip;
	stack[registers_len + 1] = rp.sp;
}
