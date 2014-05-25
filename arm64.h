/*
 * arm64.h
 */
#include "/arm64/include/ureg.h"
#define	USERADDR	0xC0000000
#define	UREGADDR	(USERADDR+BY2PG-4-0xA0)
#define	REGOFF(x)	offsetof(Ureg, x)

typedef struct Registers Registers;
typedef struct Segment Segment;
typedef struct Memory Memory;
typedef struct Inset Inset;
typedef struct Inst Inst;
typedef struct Icache Icache;
typedef struct Breakpoint Breakpoint;

enum
{
	Instruction	= 1,
	Read		= 2,
	Write		= 4,
	Access		= 2|4,
	Equal		= 4|8,
};

struct Breakpoint
{
	int		type;		/* Instruction/Read/Access/Write/Equal */
	ulong		addr;		/* Place at address */
	int		count;		/* To execute count times or value */
	int		done;		/* How many times passed through */
	Breakpoint	*next;		/* Link to next one */
};

enum
{
	Iload,
	Istore,
	Iarith,
	Ilog,
	Ibranch,
	Ireg,
	Isyscall,
	Ifloat,
	Inop,
	Icontrol,
};

struct Icache
{
	int	on;			/* Turned on */
	int	linesize;		/* Line size in bytes */
	int	stall;			/* Cache stalls */
	int	*lines;			/* Tag array */
	int*	(*hash)(ulong);		/* Hash function */
	char	*hashtext;		/* What the function looks like */
};

struct Inset
{
	Inst	*tab;		/* indexed by extended opcode */
	int	nel;
};

struct Inst
{
	void 	(*func)(ulong);
	char	*name;
	int	type;
	int	count;
	int	taken;
};

struct Registers
{
	ulong	pc;
	ulong	ir;
	Inst	*ip;
	long	r[32];
	ulong	ctr;
	ulong	cr;
	ulong	xer;
	ulong	lr;
	ulong	fpscr;
	ulong	dec;
	ulong	tbl;
	ulong	tbu;
	double	fd[32];
};

enum
{
	MemRead,
	MemReadstring,
	MemWrite,
};

enum
{
	Stack,
	Text,
	Data,
	Bss,
	Nseg,
};

struct Segment
{
	short	type;
	ulong	base;
	ulong	end;
	ulong	fileoff;
	ulong	fileend;
	int	rss;
	int	refs;
	uchar	**table;
};

struct Memory
{
	Segment	seg[Nseg];
};

void		fatal(int, char*, ...);
void		fpreginit(void);
void		run(void);
void		undef(ulong);
void		unimp(ulong);
void		dumpreg(void);
void		dumpfreg(void);
void		dumpdreg(void);
void*		emalloc(ulong);
void*		erealloc(void*, ulong, ulong);
void*		vaddr(ulong);
void		itrace(char *, ...);
void		segsum(void);
void		sc(ulong);
char*		memio(char*, ulong, int, int);
ulong		getmem_w(ulong);
ulong		ifetch(ulong);
ushort		getmem_h(ulong);
void		putmem_w(ulong, ulong);
uchar		getmem_b(ulong);
void		putmem_b(ulong, uchar);
uvlong	getmem_v(ulong);
ulong		getmem_4(ulong);
ulong		getmem_2(ulong);
void	putmem_v(ulong, uvlong);
void		putmem_h(ulong, short);
void		isum(void);
void		initicache(void);
void		updateicache(ulong addr);
long		lnrand(long);
void		randseed(long, long);
void		cmd(void);
void		brkchk(ulong, int);
void		delbpt(char*);
void		breakpoint(char*, char*);
char*		nextc(char*);
ulong		expr(char*);
void		initstk(int, char**);
void		initmap(void);
void		inithdr(int);
void		reset(void);
void		dobplist(void);
void		procinit(int);
void		printsource(long);
void		printparams(Symbol *, ulong);
void		printlocals(Symbol *, ulong);
void		stktrace(int);
void		iprofile(void);

/* Globals */
Extern 		Registers reg;
Extern 		Memory memory;
Extern		int text;
Extern		int trace;
Extern 		int sysdbg;
Extern 		int calltree;
Extern		Icache icache;
Extern		int count;
Extern		jmp_buf errjmp;
Extern		Breakpoint *bplist;
Extern		int atbpt;
Extern		int membpt;
Extern		int cmdcount;
Extern		int nopcount;
Extern		ulong dot;
extern		char *file;
Extern		Biobuf *bioout;
Extern		Biobuf *bin;
Extern		Inst *ci;
Extern		ulong *iprof;
Extern		ulong	iprofsize;
Extern		ulong loadlock;
extern		int datasize;		
extern		int printcol;
Extern		Map *symmap;
extern		ulong bits[];

/* Plan9 Kernel constants */
#define	BY2PG		4096
#define BY2WD		4
#define UTZERO		0x1000
#define TSTKSIZ		32
#define TSTACKTOP	0x20000000
#define STACKTOP	(TSTACKTOP-TSTKSIZ*BY2PG)
#define STACKSIZE	(4*1024*1024)

#define PROFGRAN	4

#define getop(i) ((i>>26)&0x3F)
#define getxo(i) ((i>>1)&0x3FF)

/* instruction classes: iota << 6. */
enum {
	Ccmpb  =    0,	/* compare and branch */
	Ccb    =   64,	/* conditional branch */
	Csys   =  128,	/* system */
	Ctb    =  192,	/* test and branch */
	Cubi   =  256,	/* unconditional branch imm */
	Cubr   =  320,	/* unconditional branch reg */
	Cai    =  384,	/* add/sub imm */
	Cab    =  448,	/* bitfield */
	Cax    =  512,	/* extract */
	Cali   =  576,	/* logic imm */
	Camwi  =  640,	/* move wide imm */
	Capcr  =  704,	/* PC-rel addr */
	Car    =  768,	/* add/sub extended reg */
	Casr   =  832,	/* add/sub shift-reg */
	Cac    =  896,	/* add/sub carry */
	Caci   =  960,	/* cond compare imm */
	Cacr   = 1024,	/* cond compare reg */
	Cacs   = 1088,	/* cond select */
	Ca1    = 1152,	/* data proc 1 src */
	Ca2    = 1216,	/* data proc 2 src */
	Ca3    = 1280,	/* data proc 3 src */
	Calsr  = 1344,	/* logic shift-reg */
	Clsr   = 1408,	/* load/store reg */
	Clsx   = 1472,	/* load/store ex */
	Clsnp  = 1536,	/* load/store no-alloc pair (off) */
	Clspos = 1600,	/* load/store reg (imm post-index) */
	Clspre = 1664,	/* load/store reg (imm pre-index) */
	Clso   = 1728,	/* load/store reg (off) */
	Clsu   = 1792,	/* load/store reg (unpriv) */
	Clsuci = 1856,	/* load/store reg (unscaled imm) */
	Clsusi = 1920,	/* load/store reg (unsigned imm) */
	Clsrpo = 1984,	/* load/store reg-pair (off) */
	Clsppo = 2048,	/* load/store reg-pair (post-index) */
	Clsppr = 2112,	/* load/store reg-pair (pre-index) */
	Cundef = 2176	/* undefined instruction */
};
