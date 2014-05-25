#include <u.h>
#include <libc.h>
#include <bio.h>
#include <mach.h>
#define Extern extern
#include "arm64.h"

void	cmpb(ulong);
void	condb(ulong);
void	syscall(ulong);
void	tb(ulong);
void	uncondbimm(ulong);
void	uncondbreg(ulong);
void	addsubimm(ulong);
void	bitfield(ulong);
void	extract(ulong);
void	logimm(ulong);
void	movwimm(ulong);
void	pcrel(ulong);
void	addsubreg(ulong);
void	addsubsreg(ulong);
void	addsubc(ulong);
void	condcmpimm(ulong);
void	condcmpreg(ulong);
void	condsel(ulong);
void	dp1(ulong);
void	dp2(ulong);
void	dp3(ulong);
void	logsreg(ulong);
void	ldstreg(ulong);
void	ldstex(ulong);
void	ldstnoallocp(ulong);
void	ldstregimmpost(ulong);
void	ldstregimmpre(ulong);
void	ldstregoff(ulong);
void	ldstregupriv(ulong);
void	ldstreguscaleimm(ulong);
void	ldstregusignimm(ulong);
void	ldstregpoff(ulong);
void	ldstregppost(ulong);
void	ldstregppre(ulong);

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

/* compare and branch
params: imm19<23,5> Rt<4,0> 
ops: sf<31> op<24> 
	CBZ   	sf=0	op=0	
	CBZ   	sf=1	op=0	
	CBNZ  	sf=0	op=1	
	CBNZ  	sf=1	op=1	
*/
void
cmpb(ulong ir)
{
	ulong sf, op, imm19, Rt;

	getcmpb(ir);
	USED(sf, op, imm19, Rt);
}

/* conditional branch
params: o1<24> imm19<23,5> cond<3,0> 
ops: o0<4> 
	B.cond	o0=0	
*/
void
condb(ulong ir)
{
	ulong o1, imm19, o0, cond;

	getcb(ir);
	USED(o1, imm19, o0, cond);
}

/* system
params: op1<18,16> CRn<15,12> CRm<11,8> op2<7,5> Rt<4,0> 
ops: L<21> op0<20,19> 
	SYS   	L=0	op0=1	
*/
void
syscall(ulong ir)
{
	ulong L, op0, op1, CRn, CRm, op2, Rt;

	getsys(ir);
	USED(L, op0, op1, CRn, CRm, op2, Rt);
}

/* test and branch
params: b5<31> b40<23,19> imm14<18,5> Rt<4,0> 
ops: op<24> 
	TBZ   	op=0	
	TBNZ  	op=1	
*/
void
tb(ulong ir)
{
	ulong b5, op, b40, imm14, Rt;

	gettb(ir);
	USED(b5, op, b40, imm14, Rt);
}

/* unconditional branch imm
params: imm26<25,0> 
ops: op<31> 
	B     	op=0	
	BL    	op=1	
*/
void
uncondbimm(ulong ir)
{
	ulong op, imm26;

	getubi(ir);
	USED(op, imm26);
}

/* unconditional branch reg
params: Rn<9,5> 
ops: opc<24,21> op2<20,16> 
	BR    	opc=0	op2=31	
	BLR   	opc=1	op2=31	
	RET   	opc=2	op2=31	
*/
void
uncondbreg(ulong ir)
{
	ulong opc, op2, Rn;

	getubr(ir);
	USED(opc, op2, Rn);
}

/* add/sub imm
params: shift<23,22> imm12<21,10> Rn<9,5> Rd<4,0> 
ops: sf<31> op<30> S<29> 
	ADD   	sf=0	op=0	S=0	
	ADDS  	sf=0	op=0	S=1	
	SUB   	sf=0	op=1	S=0	
	SUBS  	sf=0	op=1	S=1	
	ADD   	sf=1	op=0	S=0	
	ADDS  	sf=1	op=0	S=1	
	SUB   	sf=1	op=1	S=0	
	SUBS  	sf=1	op=1	S=1	
*/
void
addsubimm(ulong ir)
{
	ulong sf, op, S, shift, imm12, Rn, Rd;

	getai(ir);
	USED(sf, op, S, shift, imm12, Rn, Rd);
}

/* bitfield
params: immr<21,16> imms<15,10> Rn<9,5> Rd<4,0> 
ops: sf<31> opc<30,29> N<22> 
	SBFM  	sf=0	opc=0	N=0	
	BFM   	sf=0	opc=1	N=0	
	UBFM  	sf=0	opc=2	N=0	
	SBFM  	sf=1	opc=0	N=1	
	BFM   	sf=1	opc=1	N=1	
	UBFM  	sf=1	opc=2	N=1	
*/
void
bitfield(ulong ir)
{
	ulong sf, opc, N, immr, imms, Rn, Rd;

	getab(ir);
	USED(sf, opc, N, immr, imms, Rn, Rd);
}

/* extract
params: Rm<20,16> imms<15,10> Rn<9,5> Rd<4,0> 
ops: sf<31> op21<30,29> N<22> o0<21> 
	EXTR  	sf=0	op21=0	N=0	o0=0	
	EXTR  	sf=1	op21=0	N=1	o0=0	
*/
void
extract(ulong ir)
{
	ulong sf, op21, N, o0, Rm, imms, Rn, Rd;

	getax(ir);
	USED(sf, op21, N, o0, Rm, imms, Rn, Rd);
}

/* logic imm
params: N<22> immr<21,16> imms<15,10> Rn<9,5> Rd<4,0> 
ops: sf<31> opc<30,29> 
	AND   	sf=0	opc=0	
	ORR   	sf=0	opc=1	
	EOR   	sf=0	opc=2	
	ANDS  	sf=0	opc=3	
	AND   	sf=1	opc=0	
	ORR   	sf=1	opc=1	
	EOR   	sf=1	opc=2	
	ANDS  	sf=1	opc=3	
*/
void
logimm(ulong ir)
{
	ulong sf, opc, N, immr, imms, Rn, Rd;

	getali(ir);
	USED(sf, opc, N, immr, imms, Rn, Rd);
}

/* move wide imm
params: hw<22,21> imm16<20,5> Rd<4,0> 
ops: sf<31> opc<30,29> 
	MOVN  	sf=0	opc=0	
	MOVZ  	sf=0	opc=2	
	MOVK  	sf=0	opc=3	
	MOVN  	sf=1	opc=0	
	MOVZ  	sf=1	opc=2	
	MOVK  	sf=1	opc=3	
*/
void
movwimm(ulong ir)
{
	ulong sf, opc, hw, imm16, Rd;

	getamwi(ir);
	USED(sf, opc, hw, imm16, Rd);
}

/* PC-rel addr
params: immhi<23,5> Rd<4,0> 
ops: op<31> immlo<30,29> 
	ADR   	op=0	immlo=0	
	ADRP  	op=1	immlo=0	
*/
void
pcrel(ulong ir)
{
	ulong op, immlo, immhi, Rd;

	getapcr(ir);
	USED(op, immlo, immhi, Rd);
}

/* add/sub extended reg
params: Rm<20,16> option<15,13> imm3<12,10> Rn<9,5> Rd<4,0> 
ops: sf<31> op<30> S<29> opt<23,22> 
	ADD   	sf=0	op=0	S=0	opt=0	
	ADDS  	sf=0	op=0	S=1	opt=0	
	SUB   	sf=0	op=1	S=0	opt=0	
	SUBS  	sf=0	op=1	S=1	opt=0	
	ADD   	sf=1	op=0	S=0	opt=0	
	ADDS  	sf=1	op=0	S=1	opt=0	
	SUB   	sf=1	op=1	S=0	opt=0	
	SUBS  	sf=1	op=1	S=1	opt=0	
*/
void
addsubreg(ulong ir)
{
	ulong sf, op, S, opt, Rm, option, imm3, Rn, Rd;

	getar(ir);
	USED(sf, op, S, opt, Rm, option, imm3, Rn, Rd);
}

/* add/sub shift-reg
params: Rm<20,16> imm6<15,10> Rn<9,5> Rd<4,0> 
ops: sf<31> op<30> S<29> shift<23,22> 
	ADD   	sf=0	op=0	S=0	shift=0	
	ADDS  	sf=0	op=0	S=1	shift=0	
	ADD   	sf=0	op=1	S=0	shift=0	
	ADDS  	sf=0	op=1	S=1	shift=0	
	ADD   	sf=1	op=0	S=0	shift=0	
	ADDS  	sf=1	op=0	S=1	shift=0	
	ADD   	sf=1	op=1	S=0	shift=0	
	ADDS  	sf=1	op=1	S=1	shift=0	
*/
void
addsubsreg(ulong ir)
{
	ulong sf, op, S, shift, Rm, imm6, Rn, Rd;

	getasr(ir);
	USED(sf, op, S, shift, Rm, imm6, Rn, Rd);
}

/* add/sub carry
params: Rm<20,16> Rn<9,5> Rd<4,0> 
ops: sf<31> op<30> S<29> 
	ADC   	sf=0	op=0	S=0	
	ADCS  	sf=0	op=0	S=1	
	SBC   	sf=0	op=1	S=0	
	SBCS  	sf=0	op=1	S=1	
	ADC   	sf=1	op=0	S=0	
	ADCS  	sf=1	op=0	S=1	
	SBC   	sf=1	op=1	S=0	
	SBCS  	sf=1	op=1	S=1	
*/
void
addsubc(ulong ir)
{
	ulong sf, op, S, Rm, Rn, Rd;

	getac(ir);
	USED(sf, op, S, Rm, Rn, Rd);
}

/* cond compare imm
params: imm5<20,16> cond<15,12> Rn<9,5> nzcv<3,0> 
ops: sf<31> op<30> S<29> 
	CCMN  	sf=0	op=0	S=1	
	CCMP  	sf=0	op=1	S=1	
	CCMN  	sf=1	op=0	S=1	
	CCMP  	sf=1	op=1	S=1	
*/
void
condcmpimm(ulong ir)
{
	ulong sf, op, S, imm5, cond, Rn, nzcv;

	getaci(ir);
	USED(sf, op, S, imm5, cond, Rn, nzcv);
}

/* cond compare reg
params: Rm<20,16> cond<15,12> Rn<9,5> nzcv<3,0> 
ops: sf<31> op<30> S<29> 
	CCMN  	sf=0	op=0	S=1	
	CCMP  	sf=0	op=1	S=1	
	CCMN  	sf=1	op=0	S=1	
	CCMP  	sf=1	op=1	S=1	
*/
void
condcmpreg(ulong ir)
{
	ulong sf, op, S, Rm, cond, Rn, nzcv;

	getacr(ir);
	USED(sf, op, S, Rm, cond, Rn, nzcv);
}

/* cond select
params: Rm<20,16> cond<15,12> Rn<9,5> Rd<4,0> 
ops: sf<31> op<30> S<29> op2<11,10> 
	CSEL  	sf=0	op=0	S=0	op2=0	
	CSINC 	sf=0	op=0	S=0	op2=1	
	CSINV 	sf=0	op=1	S=0	op2=0	
	CSNEG 	sf=0	op=1	S=0	op2=1	
	CSEL  	sf=1	op=0	S=0	op2=0	
	CSINC 	sf=1	op=0	S=0	op2=1	
	CSINV 	sf=1	op=1	S=0	op2=0	
	CSNEG 	sf=1	op=1	S=0	op2=1	
*/
void
condsel(ulong ir)
{
	ulong sf, op, S, Rm, cond, op2, Rn, Rd;

	getacs(ir);
	USED(sf, op, S, Rm, cond, op2, Rn, Rd);
}

/* data proc 1 src
params: Rn<9,5> Rd<4,0> 
ops: sf<31> S<29> opcode<12,10> 
	RBIT  	sf=0	S=0	opcode=0	
	REV16 	sf=0	S=0	opcode=1	
	REV   	sf=0	S=0	opcode=2	
	CLZ   	sf=0	S=0	opcode=4	
	CLZ   	sf=0	S=0	opcode=5	
	RBIT  	sf=1	S=0	opcode=0	
	REV16 	sf=1	S=0	opcode=1	
	REV32 	sf=1	S=0	opcode=2	
	REV   	sf=1	S=0	opcode=3	
	CLZ   	sf=1	S=0	opcode=4	
	CLZ   	sf=1	S=0	opcode=5	
*/
void
dp1(ulong ir)
{
	ulong sf, S, opcode, Rn, Rd;

	geta1(ir);
	USED(sf, S, opcode, Rn, Rd);
}

/* data proc 2 src
params: Rm<20,16> Rn<9,5> Rd<4,0> 
ops: sf<31> opcode<14,10> 
	UDIV  	sf=0	opcode=2	
	SDIV  	sf=0	opcode=3	
	LSLV  	sf=0	opcode=8	
	LSRV  	sf=0	opcode=9	
	ASRV  	sf=0	opcode=10	
	RORV  	sf=0	opcode=11	
	UDIV  	sf=1	opcode=2	
	SDIV  	sf=1	opcode=3	
	LSLV  	sf=1	opcode=8	
	LSRV  	sf=1	opcode=9	
	ASRV  	sf=1	opcode=10	
	RORV  	sf=1	opcode=11	
*/
void
dp2(ulong ir)
{
	ulong sf, Rm, opcode, Rn, Rd;

	geta2(ir);
	USED(sf, Rm, opcode, Rn, Rd);
}

/* data proc 3 src
params: Rm<20,16> Ra<14,10> Rn<9,5> Rd<4,0> 
ops: sf<31> op31<23,21> o0<15> 
	MADD  	sf=0	op31=0	o0=0	
	MSUB  	sf=0	op31=0	o0=1	
	MADD  	sf=1	op31=0	o0=0	
	MSUB  	sf=1	op31=0	o0=1	
*/
void
dp3(ulong ir)
{
	ulong sf, op31, Rm, o0, Ra, Rn, Rd;

	geta3(ir);
	USED(sf, op31, Rm, o0, Ra, Rn, Rd);
}

/* logic shift-reg
params: shift<23,22> Rm<20,16> imm6<15,10> Rn<9,5> Rd<4,0> 
ops: sf<31> opc<30,29> N<21> 
	AND   	sf=0	opc=0	N=0	
	BIC   	sf=0	opc=0	N=1	
	ORR   	sf=0	opc=1	N=0	
	ORN   	sf=0	opc=1	N=1	
	EOR   	sf=0	opc=2	N=0	
	EON   	sf=0	opc=2	N=1	
	ANDS  	sf=0	opc=3	N=0	
	BICS  	sf=0	opc=3	N=1	
	AND   	sf=1	opc=0	N=0	
	BIC   	sf=1	opc=0	N=1	
	ORR   	sf=1	opc=1	N=0	
	ORN   	sf=1	opc=1	N=1	
	EOR   	sf=1	opc=2	N=0	
	EON   	sf=1	opc=2	N=1	
	ANDS  	sf=1	opc=3	N=0	
	BICS  	sf=1	opc=3	N=1	
*/
void
logsreg(ulong ir)
{
	ulong sf, opc, shift, N, Rm, imm6, Rn, Rd;

	getalsr(ir);
	USED(sf, opc, shift, N, Rm, imm6, Rn, Rd);
}

/* load/store reg
params: imm19<23,5> Rt<4,0> 
ops: opc<31,30> V<26> 
	LDR   	opc=0	V=0	
	LDR   	opc=1	V=0	
	LDRSW 	opc=2	V=0	
	PRFM  	opc=3	V=0	
*/
void
ldstreg(ulong ir)
{
	ulong opc, V, imm19, Rt;

	getlsr(ir);
	USED(opc, V, imm19, Rt);
}

/* load/store ex
params: Rs<20,16> Rt2<14,10> Rn<9,5> Rt<4,0> 
ops: size<31,30> o2<23> L<22> o1<21> o0<15> 
	STXRB 	size=0	o2=0	L=0	o1=0	o0=0	
	STLXRB	size=0	o2=0	L=0	o1=0	o0=1	
	LDXRB 	size=0	o2=0	L=1	o1=0	o0=0	
	LDAXRB	size=0	o2=0	L=1	o1=0	o0=1	
	STLRB 	size=0	o2=1	L=0	o1=0	o0=1	
	LDARB 	size=0	o2=1	L=1	o1=0	o0=1	
	STXRH 	size=1	o2=0	L=0	o1=0	o0=0	
	STLXRH	size=1	o2=0	L=0	o1=0	o0=1	
	LDXRH 	size=1	o2=0	L=1	o1=0	o0=0	
	LDAXRH	size=1	o2=0	L=1	o1=0	o0=1	
	STLRH 	size=1	o2=1	L=0	o1=0	o0=1	
	LDARH 	size=1	o2=1	L=1	o1=0	o0=1	
	STXR  	size=2	o2=0	L=0	o1=0	o0=0	
	STLXR 	size=2	o2=0	L=0	o1=0	o0=1	
	STXP  	size=2	o2=0	L=0	o1=1	o0=0	
	STLXP 	size=2	o2=0	L=0	o1=1	o0=1	
	LDXR  	size=2	o2=0	L=1	o1=0	o0=0	
	LDAXR 	size=2	o2=0	L=1	o1=0	o0=1	
	LDXP  	size=2	o2=0	L=1	o1=1	o0=0	
	LDAXP 	size=2	o2=0	L=1	o1=1	o0=1	
	STLR  	size=2	o2=1	L=0	o1=0	o0=1	
	STLR  	size=2	o2=1	L=1	o1=0	o0=1	
	STXR  	size=3	o2=0	L=0	o1=0	o0=0	
	STLXR 	size=3	o2=0	L=0	o1=0	o0=1	
	STXP  	size=3	o2=0	L=0	o1=1	o0=0	
	STLXP 	size=3	o2=0	L=0	o1=1	o0=1	
	LDXR  	size=3	o2=0	L=1	o1=0	o0=0	
	LDAXR 	size=3	o2=0	L=1	o1=0	o0=1	
	LDXP  	size=3	o2=0	L=1	o1=1	o0=0	
	LDAXP 	size=3	o2=0	L=1	o1=1	o0=1	
	STLR  	size=3	o2=1	L=0	o1=0	o0=1	
	STLR  	size=3	o2=1	L=1	o1=0	o0=1	
*/
void
ldstex(ulong ir)
{
	ulong size, o2, L, o1, Rs, o0, Rt2, Rn, Rt;

	getlsx(ir);
	USED(size, o2, L, o1, Rs, o0, Rt2, Rn, Rt);
}

/* load/store no-alloc pair (off)
params: imm7<21,15> Rt2<14,10> Rn<9,5> Rt<4,0> 
ops: opc<31,30> V<26> L<22> 
	STNP  	opc=0	V=0	L=0	
	LDNP  	opc=0	V=0	L=1	
	STNP  	opc=2	V=0	L=0	
	LDNP  	opc=2	V=0	L=1	
*/
void
ldstnoallocp(ulong ir)
{
	ulong opc, V, L, imm7, Rt2, Rn, Rt;

	getlsnp(ir);
	USED(opc, V, L, imm7, Rt2, Rn, Rt);
}

/* load/store reg (imm post-index)
params: imm9<20,12> Rn<9,5> Rt<4,0> 
ops: size<31,30> V<26> opc<23,22> 
	STRB  	size=0	V=0	opc=0	
	LDRB  	size=0	V=0	opc=1	
	LDRSB 	size=0	V=0	opc=2	
	LDRSB 	size=0	V=0	opc=3	
	STRH  	size=1	V=0	opc=0	
	LDRH  	size=1	V=0	opc=1	
	LDRSH 	size=1	V=0	opc=2	
	LDRSH 	size=1	V=0	opc=3	
	STR   	size=2	V=0	opc=0	
	LDR   	size=2	V=0	opc=1	
	LDRSW 	size=2	V=0	opc=2	
	STR   	size=3	V=0	opc=0	
	LDR   	size=3	V=0	opc=1	
*/
void
ldstregimmpost(ulong ir)
{
	ulong size, V, opc, imm9, Rn, Rt;

	getlspos(ir);
	USED(size, V, opc, imm9, Rn, Rt);
}

/* load/store reg (imm pre-index)
params: imm9<20,12> Rn<9,5> Rt<4,0> 
ops: size<31,30> V<26> opc<23,22> 
	STRB  	size=0	V=0	opc=0	
	LDRB  	size=0	V=0	opc=1	
	LDRSB 	size=0	V=0	opc=2	
	LDRSB 	size=0	V=0	opc=3	
	STRH  	size=1	V=0	opc=0	
	LDRH  	size=1	V=0	opc=1	
	LDRSH 	size=1	V=0	opc=2	
	LDRSH 	size=1	V=0	opc=3	
	STR   	size=2	V=0	opc=0	
	LDR   	size=2	V=0	opc=1	
	LDRSW 	size=2	V=0	opc=2	
	STR   	size=3	V=0	opc=0	
	LDR   	size=3	V=0	opc=1	
*/
void
ldstregimmpre(ulong ir)
{
	ulong size, V, opc, imm9, Rn, Rt;

	getlspre(ir);
	USED(size, V, opc, imm9, Rn, Rt);
}

/* load/store reg (off)
params: Rm<20,16> option<15,13> S<12> Rn<9,5> Rt<4,0> 
ops: size<31,30> V<26> opc<23,22> 
	STRB  	size=0	V=0	opc=0	
	LDRB  	size=0	V=0	opc=1	
	LDRSB 	size=0	V=0	opc=2	
	LDRSB 	size=0	V=0	opc=3	
	STRH  	size=1	V=0	opc=0	
	LDRH  	size=1	V=0	opc=1	
	LDRSH 	size=1	V=0	opc=2	
	LDRSH 	size=1	V=0	opc=3	
	STR   	size=2	V=0	opc=0	
	LDR   	size=2	V=0	opc=1	
	STR   	size=3	V=0	opc=0	
	LDR   	size=3	V=0	opc=1	
	PRFM  	size=3	V=0	opc=2	
*/
void
ldstregoff(ulong ir)
{
	ulong size, V, opc, Rm, option, S, Rn, Rt;

	getlso(ir);
	USED(size, V, opc, Rm, option, S, Rn, Rt);
}

/* load/store reg (unpriv)
params: imm9<20,12> Rn<9,5> Rt<4,0> 
ops: size<31,30> V<26> opc<23,22> 
	STTRB 	size=0	V=0	opc=0	
	LDTRB 	size=0	V=0	opc=1	
	LDTRSB	size=0	V=0	opc=2	
	LDTRSB	size=0	V=0	opc=3	
	STTRH 	size=1	V=0	opc=0	
	LDTRH 	size=1	V=0	opc=1	
	LDTRSH	size=1	V=0	opc=2	
	LDTRSH	size=1	V=0	opc=3	
	STTR  	size=2	V=0	opc=0	
	LDTR  	size=2	V=0	opc=1	
	LDTRSW	size=2	V=0	opc=2	
	STTR  	size=3	V=0	opc=0	
	LDTR  	size=3	V=0	opc=1	
*/
void
ldstregupriv(ulong ir)
{
	ulong size, V, opc, imm9, Rn, Rt;

	getlsu(ir);
	USED(size, V, opc, imm9, Rn, Rt);
}

/* load/store reg (unscaled imm)
params: imm9<20,12> Rn<9,5> Rt<4,0> 
ops: size<31,30> V<26> opc<23,22> 
	STURB 	size=0	V=0	opc=0	
	LDURB 	size=0	V=0	opc=1	
	LDURSB	size=0	V=0	opc=2	
	LDURSB	size=0	V=0	opc=3	
	STURH 	size=1	V=0	opc=0	
	LDURH 	size=1	V=0	opc=1	
	LDURSH	size=1	V=0	opc=2	
	LDURSH	size=1	V=0	opc=3	
	STUR  	size=2	V=0	opc=0	
	LDUR  	size=2	V=0	opc=1	
	LDURSW	size=2	V=0	opc=2	
	STUR  	size=3	V=0	opc=0	
	LDUR  	size=3	V=0	opc=1	
	PRFUM 	size=3	V=0	opc=2	
*/
void
ldstreguscaleimm(ulong ir)
{
	ulong size, V, opc, imm9, Rn, Rt;

	getlsuci(ir);
	USED(size, V, opc, imm9, Rn, Rt);
}

/* load/store reg (unsigned imm)
params: imm12<21,10> Rn<9,5> Rt<4,0> 
ops: size<31,30> V<26> opc<23,22> 
	STRB  	size=0	V=0	opc=0	
	LDRB  	size=0	V=0	opc=1	
	LDRSB 	size=0	V=0	opc=2	
	LDRSB 	size=0	V=0	opc=3	
	STRH  	size=1	V=0	opc=0	
	LDRH  	size=1	V=0	opc=1	
	LDRSH 	size=1	V=0	opc=2	
	LDRSH 	size=1	V=0	opc=3	
	STR   	size=2	V=0	opc=0	
	LDR   	size=2	V=0	opc=1	
	LDRSW 	size=2	V=0	opc=2	
	STR   	size=3	V=0	opc=0	
	LDR   	size=3	V=0	opc=1	
	PRFM  	size=3	V=0	opc=2	
*/
void
ldstregusignimm(ulong ir)
{
	ulong size, V, opc, imm12, Rn, Rt;

	getlsusi(ir);
	USED(size, V, opc, imm12, Rn, Rt);
}

/* load/store reg-pair (off)
params: imm7<21,15> Rt2<14,10> Rn<9,5> Rt<4,0> 
ops: opc<31,30> V<26> L<22> 
	STP   	opc=0	V=0	L=0	
	LDP   	opc=0	V=0	L=1	
	LDPSW 	opc=1	V=0	L=1	
	STP   	opc=2	V=0	L=0	
	LDP   	opc=2	V=0	L=1	
*/
void
ldstregpoff(ulong ir)
{
	ulong opc, V, L, imm7, Rt2, Rn, Rt;

	getlsrpo(ir);
	USED(opc, V, L, imm7, Rt2, Rn, Rt);
}

/* load/store reg-pair (post-index)
params: imm7<21,15> Rt2<14,10> Rn<9,5> Rt<4,0> 
ops: opc<31,30> V<26> L<22> 
	STP   	opc=0	V=0	L=0	
	LDP   	opc=0	V=0	L=1	
	LDPSW 	opc=1	V=0	L=1	
	STP   	opc=2	V=0	L=0	
	LDP   	opc=2	V=0	L=1	
*/
void
ldstregppost(ulong ir)
{
	ulong opc, V, L, imm7, Rt2, Rn, Rt;

	getlsppo(ir);
	USED(opc, V, L, imm7, Rt2, Rn, Rt);
}

/* load/store reg-pair (pre-index)
params: imm7<21,15> Rt2<14,10> Rn<9,5> Rt<4,0> 
ops: opc<31,30> V<26> L<22> 
	STP   	opc=0	V=0	L=0	
	LDP   	opc=0	V=0	L=1	
	LDPSW 	opc=1	V=0	L=1	
	STP   	opc=2	V=0	L=0	
	LDP   	opc=2	V=0	L=1	
*/
void
ldstregppre(ulong ir)
{
	ulong opc, V, L, imm7, Rt2, Rn, Rt;

	getlsppr(ir);
	USED(opc, V, L, imm7, Rt2, Rn, Rt);
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
