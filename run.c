#include <u.h>
#include <libc.h>
#include <bio.h>
#include <mach.h>
#define Extern extern
#include "arm64.h"

void
run(void)
{
	do {
		reg.ir = ifetch(reg.pc);
		ci = 0;
		if(ci && ci->func){
			ci->count++;
			(*ci->func)(reg.ir);
		} else {
			if(ci && ci->name && trace)
				itrace("%s\t[not yet done]", ci->name);
			else
				undef(reg.ir);
		}
		reg.pc += 4;
		if(bplist)
			brkchk(reg.pc, Instruction);
	}while(--count);
}

void
ilock(int)
{
}

void
undef(ulong ir)
{
/*	Bprint(bioout, "op=%d op2=%d op3=%d\n", ir>>30, (ir>>21)&0x7, (ir>>19)&0x3f); */
	Bprint(bioout, "illegal_instruction IR #%.8lux (op=%ld/%ld, pc=#%.8lux)\n", ir, getop(ir), getxo(ir), reg.pc);
	if(ci && ci->name && ci->func==0)
		Bprint(bioout, "(%s not yet implemented)\n", ci->name);
	longjmp(errjmp, 0);
}

void
unimp(ulong ir)
{
/*	Bprint(bioout, "op=%d op2=%d op3=%d\n", ir>>30, (ir>>21)&0x7, (ir>>19)&0x3f); */
	Bprint(bioout, "illegal_instruction IR #%.8lux (op=%ld/%ld, pc=#%.8lux) %s not in MPC601\n", ir, getop(ir), getxo(ir), reg.pc, ci->name?ci->name: "-");
	longjmp(errjmp, 0);
}
