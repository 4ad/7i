#include <u.h>
#include <libc.h>
#include <bio.h>
#include <mach.h>
#define Extern extern
#include "arm64.h"

vlong	doshift(ulong, vlong, ulong, ulong);
vlong	shift64(vlong, ulong, ulong);
long	shift32(long, ulong, ulong);

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

Inst itab[] =
{
	/* compare and branch */
	[Ccmpb+ 0]	{cmpb, "CBZ",   	Ibranch}, /* 0 */
	[Ccmpb+ 2]	{cmpb, "CBZ",   	Ibranch}, /* 2 */
	[Ccmpb+ 1]	{cmpb, "CBNZ",  	Ibranch}, /* 1 */
	[Ccmpb+ 3]	{cmpb, "CBNZ",  	Ibranch}, /* 3 */

	/* conditional branch */
	[Ccb+ 0]	{condb, "B.cond",	Ibranch}, /* 64 */

	/* system */
	[Csys+ 1]	{syscall, "SVC",   	Isyscall}, /* 129 */

	/* test and branch */
	[Ctb+ 0]	{tb, "TBZ",   	Ibranch}, /* 192 */
	[Ctb+ 1]	{tb, "TBNZ",  	Ibranch}, /* 193 */

	/* unconditional branch imm */
	[Cubi+ 0]	{uncondbimm, "B",     	Ibranch}, /* 256 */
	[Cubi+ 1]	{uncondbimm, "BL",    	Ibranch}, /* 257 */

	/* unconditional branch reg */
	[Cubr+31]	{uncondbreg, "BR",    	Ibranch}, /* 351 */
	[Cubr+63]	{uncondbreg, "BLR",   	Ibranch}, /* 383 */
	[Cubr+95]	{uncondbreg, "RET",   	Ibranch}, /* 351 */

	/* add/sub imm */
	[Cai+ 0]	{addsubimm, "ADD",   	Iarith}, /* 384 */
	[Cai+ 1]	{addsubimm, "ADDS",  	Iarith}, /* 385 */
	[Cai+ 2]	{addsubimm, "SUB",   	Iarith}, /* 386 */
	[Cai+ 3]	{addsubimm, "SUBS",  	Iarith}, /* 387 */
	[Cai+ 4]	{addsubimm, "ADD",   	Iarith}, /* 388 */
	[Cai+ 5]	{addsubimm, "ADDS",  	Iarith}, /* 389 */
	[Cai+ 6]	{addsubimm, "SUB",   	Iarith}, /* 390 */
	[Cai+ 7]	{addsubimm, "SUBS",  	Iarith}, /* 391 */

	/* bitfield */
	[Cab+ 0]	{bitfield, "SBFM",  	Iarith}, /* 448 */
	[Cab+ 2]	{bitfield, "BFM",   	Iarith}, /* 450 */
	[Cab+ 4]	{bitfield, "UBFM",  	Iarith}, /* 452 */
	[Cab+ 9]	{bitfield, "SBFM",  	Iarith}, /* 457 */
	[Cab+11]	{bitfield, "BFM",   	Iarith}, /* 459 */
	[Cab+13]	{bitfield, "UBFM",  	Iarith}, /* 461 */

	/* extract */
	[Cax+ 0]	{extract, "EXTR",  	Iarith}, /* 512 */
	[Cax+18]	{extract, "EXTR",  	Iarith}, /* 530 */

	/* logic imm */
	[Cali+ 0]	{logimm, "AND",   	  Ilog}, /* 576 */
	[Cali+ 1]	{logimm, "ORR",   	  Ilog}, /* 577 */
	[Cali+ 2]	{logimm, "EOR",   	  Ilog}, /* 578 */
	[Cali+ 3]	{logimm, "ANDS",  	  Ilog}, /* 579 */
	[Cali+ 4]	{logimm, "AND",   	  Ilog}, /* 580 */
	[Cali+ 5]	{logimm, "ORR",   	  Ilog}, /* 581 */
	[Cali+ 6]	{logimm, "EOR",   	  Ilog}, /* 582 */
	[Cali+ 7]	{logimm, "ANDS",  	  Ilog}, /* 583 */

	/* move wide imm */
	[Camwi+ 0]	{movwimm, "MOVN",  	Iarith}, /* 640 */
	[Camwi+ 2]	{movwimm, "MOVZ",  	Iarith}, /* 642 */
	[Camwi+ 3]	{movwimm, "MOVK",  	Iarith}, /* 643 */
	[Camwi+ 4]	{movwimm, "MOVN",  	Iarith}, /* 644 */
	[Camwi+ 6]	{movwimm, "MOVZ",  	Iarith}, /* 646 */
	[Camwi+ 7]	{movwimm, "MOVK",  	Iarith}, /* 647 */

	/* PC-rel addr */
	[Capcr+ 0]	{pcrel, "ADR",   	Iarith}, /* 704 */
	[Capcr+ 4]	{pcrel, "ADRP",  	Iarith}, /* 708 */

	/* add/sub extended reg */
	[Car+ 0]	{addsubreg, "ADD",   	Iarith}, /* 768 */
	[Car+ 4]	{addsubreg, "ADDS",  	Iarith}, /* 772 */
	[Car+ 8]	{addsubreg, "SUB",   	Iarith}, /* 776 */
	[Car+12]	{addsubreg, "SUBS",  	Iarith}, /* 780 */
	[Car+16]	{addsubreg, "ADD",   	Iarith}, /* 784 */
	[Car+20]	{addsubreg, "ADDS",  	Iarith}, /* 788 */
	[Car+24]	{addsubreg, "SUB",   	Iarith}, /* 792 */
	[Car+28]	{addsubreg, "SUBS",  	Iarith}, /* 796 */

	/* add/sub shift-reg */
	[Casr+ 0]	{addsubsreg, "ADD",   	Iarith}, /* 832 */
	[Casr+ 4]	{addsubsreg, "ADDS",  	Iarith}, /* 836 */
	[Casr+ 8]	{addsubsreg, "ADD",   	Iarith}, /* 840 */
	[Casr+12]	{addsubsreg, "ADDS",  	Iarith}, /* 844 */
	[Casr+16]	{addsubsreg, "ADD",   	Iarith}, /* 848 */
	[Casr+20]	{addsubsreg, "ADDS",  	Iarith}, /* 852 */
	[Casr+24]	{addsubsreg, "ADD",   	Iarith}, /* 856 */
	[Casr+28]	{addsubsreg, "ADDS",  	Iarith}, /* 860 */

	/* add/sub carry */
	[Cac+ 0]	{addsubc, "ADC",   	Iarith}, /* 896 */
	[Cac+ 1]	{addsubc, "ADCS",  	Iarith}, /* 897 */
	[Cac+ 2]	{addsubc, "SBC",   	Iarith}, /* 898 */
	[Cac+ 3]	{addsubc, "SBCS",  	Iarith}, /* 899 */
	[Cac+ 4]	{addsubc, "ADC",   	Iarith}, /* 900 */
	[Cac+ 5]	{addsubc, "ADCS",  	Iarith}, /* 901 */
	[Cac+ 6]	{addsubc, "SBC",   	Iarith}, /* 902 */
	[Cac+ 7]	{addsubc, "SBCS",  	Iarith}, /* 903 */

	/* cond compare imm */
	[Caci+ 1]	{condcmpimm, "CCMN",  	Iarith}, /* 961 */
	[Caci+ 3]	{condcmpimm, "CCMP",  	Iarith}, /* 963 */
	[Caci+ 5]	{condcmpimm, "CCMN",  	Iarith}, /* 965 */
	[Caci+ 7]	{condcmpimm, "CCMP",  	Iarith}, /* 967 */

	/* cond compare reg */
	[Cacr+ 1]	{condcmpreg, "CCMN",  	Iarith}, /* 1025 */
	[Cacr+ 3]	{condcmpreg, "CCMP",  	Iarith}, /* 1027 */
	[Cacr+ 5]	{condcmpreg, "CCMN",  	Iarith}, /* 1029 */
	[Cacr+ 7]	{condcmpreg, "CCMP",  	Iarith}, /* 1031 */

	/* cond select */
	[Cacs+ 0]	{condsel, "CSEL",  	Iarith}, /* 1088 */
	[Cacs+ 1]	{condsel, "CSINC", 	Iarith}, /* 1089 */
	[Cacs+ 8]	{condsel, "CSINV", 	Iarith}, /* 1096 */
	[Cacs+ 9]	{condsel, "CSNEG", 	Iarith}, /* 1097 */
	[Cacs+16]	{condsel, "CSEL",  	Iarith}, /* 1104 */
	[Cacs+17]	{condsel, "CSINC", 	Iarith}, /* 1105 */
	[Cacs+24]	{condsel, "CSINV", 	Iarith}, /* 1112 */
	[Cacs+25]	{condsel, "CSNEG", 	Iarith}, /* 1113 */

	/* data proc 1 src */
	[Ca1+ 0]	{dp1, "RBIT",  	Iarith}, /* 1152 */
	[Ca1+ 1]	{dp1, "REV16", 	Iarith}, /* 1153 */
	[Ca1+ 2]	{dp1, "REV",   	Iarith}, /* 1154 */
	[Ca1+ 4]	{dp1, "CLZ",   	Iarith}, /* 1156 */
	[Ca1+ 5]	{dp1, "CLZ",   	Iarith}, /* 1157 */
	[Ca1+16]	{dp1, "RBIT",  	Iarith}, /* 1168 */
	[Ca1+17]	{dp1, "REV16", 	Iarith}, /* 1169 */
	[Ca1+18]	{dp1, "REV32", 	Iarith}, /* 1170 */
	[Ca1+19]	{dp1, "REV",   	Iarith}, /* 1171 */
	[Ca1+20]	{dp1, "CLZ",   	Iarith}, /* 1172 */
	[Ca1+21]	{dp1, "CLZ",   	Iarith}, /* 1173 */

	/* data proc 2 src */
	[Ca2+ 2]	{dp2, "UDIV",  	Iarith}, /* 1218 */
	[Ca2+ 3]	{dp2, "SDIV",  	Iarith}, /* 1219 */
	[Ca2+ 8]	{dp2, "LSLV",  	Iarith}, /* 1224 */
	[Ca2+ 9]	{dp2, "LSRV",  	Iarith}, /* 1225 */
	[Ca2+10]	{dp2, "ASRV",  	Iarith}, /* 1226 */
	[Ca2+11]	{dp2, "RORV",  	Iarith}, /* 1227 */
	[Ca2+34]	{dp2, "UDIV",  	Iarith}, /* 1250 */
	[Ca2+35]	{dp2, "SDIV",  	Iarith}, /* 1251 */
	[Ca2+40]	{dp2, "LSLV",  	Iarith}, /* 1256 */
	[Ca2+41]	{dp2, "LSRV",  	Iarith}, /* 1257 */
	[Ca2+42]	{dp2, "ASRV",  	Iarith}, /* 1258 */
	[Ca2+43]	{dp2, "RORV",  	Iarith}, /* 1259 */

	/* data proc 3 src */
	[Ca3+ 0]	{dp3, "MADD",  	Iarith}, /* 1280 */
	[Ca3+ 1]	{dp3, "MSUB",  	Iarith}, /* 1281 */
	[Ca3+16]	{dp3, "MADD",  	Iarith}, /* 1296 */
	[Ca3+17]	{dp3, "MSUB",  	Iarith}, /* 1297 */

	/* logic shift-reg */
	[Calsr+ 0]	{logsreg, "AND",   	  Ilog}, /* 1344 */
	[Calsr+ 1]	{logsreg, "BIC",   	  Ilog}, /* 1345 */
	[Calsr+ 2]	{logsreg, "ORR",   	  Ilog}, /* 1346 */
	[Calsr+ 3]	{logsreg, "ORN",   	  Ilog}, /* 1347 */
	[Calsr+ 4]	{logsreg, "EOR",   	  Ilog}, /* 1348 */
	[Calsr+ 5]	{logsreg, "EON",   	  Ilog}, /* 1349 */
	[Calsr+ 6]	{logsreg, "ANDS",  	  Ilog}, /* 1350 */
	[Calsr+ 7]	{logsreg, "BICS",  	  Ilog}, /* 1351 */
	[Calsr+ 8]	{logsreg, "AND",   	  Ilog}, /* 1352 */
	[Calsr+ 9]	{logsreg, "BIC",   	  Ilog}, /* 1353 */
	[Calsr+10]	{logsreg, "ORR",   	  Ilog}, /* 1354 */
	[Calsr+11]	{logsreg, "ORN",   	  Ilog}, /* 1355 */
	[Calsr+12]	{logsreg, "EOR",   	  Ilog}, /* 1356 */
	[Calsr+13]	{logsreg, "EON",   	  Ilog}, /* 1357 */
	[Calsr+14]	{logsreg, "ANDS",  	  Ilog}, /* 1358 */
	[Calsr+15]	{logsreg, "BICS",  	  Ilog}, /* 1359 */

	/* load/store reg */
	[Clsr+ 0]	{ldstreg, "LDR",   	 Iload}, /* 1408 */
	[Clsr+ 2]	{ldstreg, "LDR",   	 Iload}, /* 1410 */
	[Clsr+ 4]	{ldstreg, "LDRSW", 	 Iload}, /* 1412 */
	[Clsr+ 6]	{ldstreg, "PRFM",  	 Iload}, /* 1414 */

	/* load/store ex */
	[Clsx+ 0]	{ldstex, "STXRB", 	Istore}, /* 1472 */
	[Clsx+ 1]	{ldstex, "STLXRB",	Istore}, /* 1473 */
	[Clsx+ 4]	{ldstex, "LDXRB", 	 Iload}, /* 1476 */
	[Clsx+ 5]	{ldstex, "LDAXRB",	 Iload}, /* 1477 */
	[Clsx+ 9]	{ldstex, "STLRB", 	Istore}, /* 1481 */
	[Clsx+13]	{ldstex, "LDARB", 	 Iload}, /* 1485 */
	[Clsx+16]	{ldstex, "STXRH", 	Istore}, /* 1488 */
	[Clsx+17]	{ldstex, "STLXRH",	Istore}, /* 1489 */
	[Clsx+20]	{ldstex, "LDXRH", 	 Iload}, /* 1492 */
	[Clsx+21]	{ldstex, "LDAXRH",	 Iload}, /* 1493 */
	[Clsx+25]	{ldstex, "STLRH", 	Istore}, /* 1497 */
	[Clsx+29]	{ldstex, "LDARH", 	 Iload}, /* 1501 */
	[Clsx+32]	{ldstex, "STXR",  	Istore}, /* 1504 */
	[Clsx+33]	{ldstex, "STLXR", 	Istore}, /* 1505 */
	[Clsx+34]	{ldstex, "STXP",  	Istore}, /* 1506 */
	[Clsx+35]	{ldstex, "STLXP", 	Istore}, /* 1507 */
	[Clsx+36]	{ldstex, "LDXR",  	 Iload}, /* 1508 */
	[Clsx+37]	{ldstex, "LDAXR", 	 Iload}, /* 1509 */
	[Clsx+38]	{ldstex, "LDXP",  	 Iload}, /* 1510 */
	[Clsx+39]	{ldstex, "LDAXP", 	 Iload}, /* 1511 */
	[Clsx+41]	{ldstex, "STLR",  	Istore}, /* 1513 */
	[Clsx+45]	{ldstex, "STLR",  	Istore}, /* 1517 */
	[Clsx+48]	{ldstex, "STXR",  	Istore}, /* 1520 */
	[Clsx+49]	{ldstex, "STLXR", 	Istore}, /* 1521 */
	[Clsx+50]	{ldstex, "STXP",  	Istore}, /* 1522 */
	[Clsx+51]	{ldstex, "STLXP", 	Istore}, /* 1523 */
	[Clsx+52]	{ldstex, "LDXR",  	 Iload}, /* 1524 */
	[Clsx+53]	{ldstex, "LDAXR", 	 Iload}, /* 1525 */
	[Clsx+54]	{ldstex, "LDXP",  	 Iload}, /* 1526 */
	[Clsx+55]	{ldstex, "LDAXP", 	 Iload}, /* 1527 */
	[Clsx+57]	{ldstex, "STLR",  	 Iload}, /* 1529 */
	[Clsx+61]	{ldstex, "STLR",  	 Iload}, /* 1533 */

	/* load/store no-alloc pair (off) */
	[Clsnp+ 0]	{ldstnoallocp, "STNP",  	Istore}, /* 1536 */
	[Clsnp+ 1]	{ldstnoallocp, "LDNP",  	 Iload}, /* 1537 */
	[Clsnp+ 8]	{ldstnoallocp, "STNP",  	Istore}, /* 1544 */
	[Clsnp+ 9]	{ldstnoallocp, "LDNP",  	 Iload}, /* 1545 */

	/* load/store reg (imm post-index) */
	[Clspos+ 0]	{ldstregimmpost, "STRB",  	Istore}, /* 1600 */
	[Clspos+ 1]	{ldstregimmpost, "LDRB",  	 Iload}, /* 1601 */
	[Clspos+ 2]	{ldstregimmpost, "LDRSB", 	 Iload}, /* 1602 */
	[Clspos+ 3]	{ldstregimmpost, "LDRSB", 	 Iload}, /* 1603 */
	[Clspos+ 8]	{ldstregimmpost, "STRH",  	Istore}, /* 1608 */
	[Clspos+ 9]	{ldstregimmpost, "LDRH",  	 Iload}, /* 1609 */
	[Clspos+10]	{ldstregimmpost, "LDRSH", 	 Iload}, /* 1610 */
	[Clspos+11]	{ldstregimmpost, "LDRSH", 	 Iload}, /* 1611 */
	[Clspos+16]	{ldstregimmpost, "STR",   	Istore}, /* 1616 */
	[Clspos+17]	{ldstregimmpost, "LDR",   	 Iload}, /* 1617 */
	[Clspos+18]	{ldstregimmpost, "LDRSW", 	 Iload}, /* 1618 */
	[Clspos+24]	{ldstregimmpost, "STR",   	Istore}, /* 1624 */
	[Clspos+25]	{ldstregimmpost, "LDR",   	 Iload}, /* 1625 */

	/* load/store reg (imm pre-index) */
	[Clspre+ 0]	{ldstregimmpre, "STRB",  	Istore}, /* 1664 */
	[Clspre+ 1]	{ldstregimmpre, "LDRB",  	 Iload}, /* 1665 */
	[Clspre+ 2]	{ldstregimmpre, "LDRSB", 	 Iload}, /* 1666 */
	[Clspre+ 3]	{ldstregimmpre, "LDRSB", 	 Iload}, /* 1667 */
	[Clspre+ 8]	{ldstregimmpre, "STRH",  	Istore}, /* 1672 */
	[Clspre+ 9]	{ldstregimmpre, "LDRH",  	 Iload}, /* 1673 */
	[Clspre+10]	{ldstregimmpre, "LDRSH", 	 Iload}, /* 1674 */
	[Clspre+11]	{ldstregimmpre, "LDRSH", 	 Iload}, /* 1675 */
	[Clspre+16]	{ldstregimmpre, "STR",   	Istore}, /* 1680 */
	[Clspre+17]	{ldstregimmpre, "LDR",   	 Iload}, /* 1681 */
	[Clspre+18]	{ldstregimmpre, "LDRSW", 	 Iload}, /* 1682 */
	[Clspre+24]	{ldstregimmpre, "STR",   	Istore}, /* 1688 */
	[Clspre+25]	{ldstregimmpre, "LDR",   	 Iload}, /* 1689 */

	/* load/store reg (off) */
	[Clso+ 0]	{ldstregoff, "STRB",  	Istore}, /* 1728 */
	[Clso+ 1]	{ldstregoff, "LDRB",  	 Iload}, /* 1729 */
	[Clso+ 2]	{ldstregoff, "LDRSB", 	 Iload}, /* 1730 */
	[Clso+ 3]	{ldstregoff, "LDRSB", 	 Iload}, /* 1731 */
	[Clso+ 8]	{ldstregoff, "STRH",  	Istore}, /* 1736 */
	[Clso+ 9]	{ldstregoff, "LDRH",  	 Iload}, /* 1737 */
	[Clso+10]	{ldstregoff, "LDRSH", 	 Iload}, /* 1738 */
	[Clso+11]	{ldstregoff, "LDRSH", 	 Iload}, /* 1739 */
	[Clso+16]	{ldstregoff, "STR",   	Istore}, /* 1744 */
	[Clso+17]	{ldstregoff, "LDR",   	 Iload}, /* 1745 */
	[Clso+24]	{ldstregoff, "STR",   	Istore}, /* 1752 */
	[Clso+25]	{ldstregoff, "LDR",   	 Iload}, /* 1753 */
	[Clso+26]	{ldstregoff, "PRFM",  	 Iload}, /* 1754 */

	/* load/store reg (unpriv) */
	[Clsu+ 0]	{ldstregupriv, "STTRB", 	Istore}, /* 1792 */
	[Clsu+ 1]	{ldstregupriv, "LDTRB", 	 Iload}, /* 1793 */
	[Clsu+ 2]	{ldstregupriv, "LDTRSB",	 Iload}, /* 1794 */
	[Clsu+ 3]	{ldstregupriv, "LDTRSB",	 Iload}, /* 1795 */
	[Clsu+ 8]	{ldstregupriv, "STTRH", 	Istore}, /* 1800 */
	[Clsu+ 9]	{ldstregupriv, "LDTRH", 	 Iload}, /* 1801 */
	[Clsu+10]	{ldstregupriv, "LDTRSH",	 Iload}, /* 1802 */
	[Clsu+11]	{ldstregupriv, "LDTRSH",	 Iload}, /* 1803 */
	[Clsu+16]	{ldstregupriv, "STTR",  	Istore}, /* 1808 */
	[Clsu+17]	{ldstregupriv, "LDTR",  	 Iload}, /* 1809 */
	[Clsu+18]	{ldstregupriv, "LDTRSW",	 Iload}, /* 1810 */
	[Clsu+24]	{ldstregupriv, "STTR",  	Istore}, /* 1816 */
	[Clsu+25]	{ldstregupriv, "LDTR",  	 Iload}, /* 1817 */

	/* load/store reg (unscaled imm) */
	[Clsuci+ 0]	{ldstreguscaleimm, "STURB", 	Istore}, /* 1856 */
	[Clsuci+ 1]	{ldstreguscaleimm, "LDURB", 	 Iload}, /* 1857 */
	[Clsuci+ 2]	{ldstreguscaleimm, "LDURSB",	 Iload}, /* 1858 */
	[Clsuci+ 3]	{ldstreguscaleimm, "LDURSB",	 Iload}, /* 1859 */
	[Clsuci+ 8]	{ldstreguscaleimm, "STURH", 	Istore}, /* 1864 */
	[Clsuci+ 9]	{ldstreguscaleimm, "LDURH", 	 Iload}, /* 1865 */
	[Clsuci+10]	{ldstreguscaleimm, "LDURSH",	 Iload}, /* 1866 */
	[Clsuci+11]	{ldstreguscaleimm, "LDURSH",	 Iload}, /* 1867 */
	[Clsuci+16]	{ldstreguscaleimm, "STUR",  	Istore}, /* 1872 */
	[Clsuci+17]	{ldstreguscaleimm, "LDUR",  	 Iload}, /* 1873 */
	[Clsuci+18]	{ldstreguscaleimm, "LDURSW",	 Iload}, /* 1874 */
	[Clsuci+24]	{ldstreguscaleimm, "STUR",  	Istore}, /* 1880 */
	[Clsuci+25]	{ldstreguscaleimm, "LDUR",  	 Iload}, /* 1881 */
	[Clsuci+26]	{ldstreguscaleimm, "PRFUM", 	 Iload}, /* 1882 */

	/* load/store reg (unsigned imm) */
	[Clsusi+ 0]	{ldstregusignimm, "STRB",  	Istore}, /* 1920 */
	[Clsusi+ 1]	{ldstregusignimm, "LDRB",  	 Iload}, /* 1921 */
	[Clsusi+ 2]	{ldstregusignimm, "LDRSB", 	 Iload}, /* 1922 */
	[Clsusi+ 3]	{ldstregusignimm, "LDRSB", 	 Iload}, /* 1923 */
	[Clsusi+ 8]	{ldstregusignimm, "STRH",  	Istore}, /* 1928 */
	[Clsusi+ 9]	{ldstregusignimm, "LDRH",  	 Iload}, /* 1929 */
	[Clsusi+10]	{ldstregusignimm, "LDRSH", 	 Iload}, /* 1930 */
	[Clsusi+11]	{ldstregusignimm, "LDRSH", 	 Iload}, /* 1931 */
	[Clsusi+16]	{ldstregusignimm, "STR",   	Istore}, /* 1936 */
	[Clsusi+17]	{ldstregusignimm, "LDR",   	 Iload}, /* 1937 */
	[Clsusi+18]	{ldstregusignimm, "LDRSW", 	 Iload}, /* 1938 */
	[Clsusi+24]	{ldstregusignimm, "STR",   	Istore}, /* 1944 */
	[Clsusi+25]	{ldstregusignimm, "LDR",   	 Iload}, /* 1945 */
	[Clsusi+26]	{ldstregusignimm, "PRFM",  	Istore}, /* 1946 */

	/* load/store reg-pair (off) */
	[Clsrpo+ 0]	{ldstregpoff, "STP",   	Istore}, /* 1984 */
	[Clsrpo+ 1]	{ldstregpoff, "LDP",   	 Iload}, /* 1985 */
	[Clsrpo+ 5]	{ldstregpoff, "LDPSW", 	 Iload}, /* 1989 */
	[Clsrpo+ 8]	{ldstregpoff, "STP",   	 Iload}, /* 1992 */
	[Clsrpo+ 9]	{ldstregpoff, "LDP",   	 Iload}, /* 1993 */

	/* load/store reg-pair (post-index) */
	[Clsppo+ 0]	{ldstregppost, "STP",   	Istore}, /* 2048 */
	[Clsppo+ 1]	{ldstregppost, "LDP",   	 Iload}, /* 2049 */
	[Clsppo+ 5]	{ldstregppost, "LDPSW", 	 Iload}, /* 2053 */
	[Clsppo+ 8]	{ldstregppost, "STP",   	Istore}, /* 2056 */
	[Clsppo+ 9]	{ldstregppost, "LDP",   	 Iload}, /* 2057 */

	/* load/store reg-pair (pre-index) */
	[Clsppr+ 0]	{ldstregppre, "STP",   	Istore}, /* 2112 */
	[Clsppr+ 1]	{ldstregppre, "LDP",   	 Iload}, /* 2113 */
	[Clsppr+ 5]	{ldstregppre, "LDPSW", 	 Iload}, /* 2117 */
	[Clsppr+ 8]	{ldstregppre, "STP",   	Istore}, /* 2120 */
	[Clsppr+ 9]	{ldstregppre, "LDP",   	 Iload}, /* 2121 */

	[Cundef]	{undef, "???",   	 Inop}, /* 2176 */

	{ 0 }
};

/* doshift, not shift, because we want to preserve names in the ARM64
manual, and shift is used in many instruction encodings as a field
name.*/
vlong
doshift(ulong sf, vlong v, ulong typ, ulong bits)
{
	if(bits == 0)
		return v;
	if(sf == 1) /* 64-bit */
		return shift64(v, typ, bits);
	/* shift32 returns long, so it will be sign extended. */
	return shift32((uvlong)v, typ, bits);
}

vlong
shift64(vlong v, ulong typ, ulong bits)
{
	switch(typ) {
	case 0:	/* logical left */
		return v << bits;
	case 1: /* logical right */
		return (uvlong)v >> bits;
	case 2:	/* arith right */
		return v >> bits;
	case 3:	/* rotate right */
		return (v << (64-bits)) | ((uvlong)v >> bits);
	}
	/* not reached */
	return 0xbad0ULL << 60;
}

long
shift32(long v, ulong typ, ulong bits)
{
	switch(typ) {
	case 0:	/* logical left */
		return v << bits;
	case 1: /* logical right */
		return (ulong)v >> bits;
	case 2:	/* arith right */
		return v >> bits;
	case 3:	/* rotate right */
		return (v << (32-bits)) | ((ulong)v >> bits);
	}
	/* not reached */
	return 0xbad1U << 28;
}

void
run(void)
{
	do {
		reg.ir = ifetch(reg.pc);
		ci = &itab[getxo(reg.ir)];
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

/* Font
getxo returns the "super-opcode" (xo) of an instruction. The xo is
the bitwise OR between the class opcode (see arm64.h:/classes) and
the number constructed from the class-select bits (the bullets in
the table below). The xo can index the itab table.

Instruction encoding table, per class. Bullets represent bits which
select a particular instruction from a particular instruction class.

•011|010•|....|....|....|....|....|.... cmpb  compare and branch
0101|010.|....|....|....|....|...•|.... cb    conditional branch
1101|0100|•••.|....|....|....|....|..•• sys   system
.011|011•|....|....|....|....|....|.... tb    test and branch
•001|01..|....|....|....|....|....|.... ubi   unconditional branch imm
1101|011•|••••|••••|....|....|....|.... ubr   unconditional branch reg
•••1|0001|....|....|....|....|....|.... ai    add/sub imm
•••1|0011|0•..|....|....|....|....|.... ab    bitfield
•••1|0011|1••.|....|....|....|....|.... ax    extract
•••1|0010|0...|....|....|....|....|.... ali   logic imm
•••1|0010|1...|....|....|....|....|.... amwi  move wide imm
•••1|0000|....|....|....|....|....|.... apcr  PC-rel addr
•••0|1011|••1.|....|....|....|....|.... ar    add/sub reg
•••0|1011|••0.|....|....|....|....|.... asr   add/sub shift-reg
•••1|1010|000.|....|....|....|....|.... ac    add/sub carry
•••1|1010|010.|....|....|1...|....|.... aci   cond compare imm
•••1|1010|010.|....|....|0...|....|.... acr   cond compare reg
•••1|1010|100.|....|....|••..|....|.... acs   cond select
•1•1|1010|110.|....|...•|••..|....|.... a1    data proc 1 src
•0.1|1010|110.|....|.•••|••..|....|.... a2    data proc 2 src
•..1|1011|•••.|....|•...|....|....|.... a3    data proc 3 src
•••0|1010|..•.|....|....|....|....|.... alsr  logic shift-reg
••01|1•00|....|....|....|....|....|.... lsr   load/store reg
••00|1000|•••.|....|•...|....|....|.... lsx   load/store ex
••10|1•00|0•..|....|....|....|....|.... lsnp  load/store no-alloc pair (off)
••11|1•00|••0.|....|....|01..|....|.... lspos load/store reg (imm post-index)
••11|1•00|••0.|....|....|11..|....|.... lspre load/store reg (imm pre-index)
••11|1•00|••1.|....|....|10..|....|.... lso   load/store reg (off)
••11|1•00|••0.|....|....|10..|....|.... lsu   load/store reg (unpriv)
••11|1•00|••1.|....|....|00..|....|.... lsuci load/store reg (unscaled imm)
••11|1•01|••..|....|....|....|....|.... lsusi load/store reg (unsigned imm)
••10|1•01|0•..|....|....|....|....|.... lsrpo load/store reg-pair (off)
••10|1•00|1•..|....|....|....|....|.... lsppo load/store reg-pair (post-index)
••10|1•01|1•..|....|....|....|....|.... lsppr load/store reg-pair (pre-index)

*/
ulong
getxo(ulong ir)
{
	// high-level dispatch.
	ulong b2826, b27, b25, b2725;
	// data processing (imm).
	ulong b2824, b2823;
	// branches, exceptions, syscalls.
	ulong b3026, b3025, b3125, b3124;
	// loads and stores.
	ulong b2927, b2924, b2524, b2523, b21, b1110;
	// data processing (reg).
	ulong b30, b2321, b11;	// +b2824, +b21

	b2826 = (ir>>26)&7;
	b27 = (ir>>27)&1;
	b25 = (ir>>25)&1;
	b2725 = (ir>>25)&7;
	b2824 = (ir>>24)&0x1F;
	b2823 = (ir>>23)&0x3F;
	b3026 = (ir>>26)&0x1F;
	b3025 = (ir>>25)&0x3F;
	b3125 = (ir>>25)&0x7F;
	b3124 = (ir>>24)&0xFF;
	b2927 = (ir>>27)&7;
	b2924 = (ir>>24)&0x3F;
	b2524 = (ir>>24)&3;
	b2523 = (ir>>23)&7;
	b21 = (ir>>21)&1;
	b1110 = (ir>>10)&3;
	b30 = (ir>>30)&1;
	b2321 = (ir>>21)&7;
	b11 = (ir>>11)&1;

	switch(b2826) {
	case 4:	// data processing (imm)
		switch(b2824) {
		case 0x10:	// PC-rel addr
			return Capcr | opapcr(ir);
		case 0x11:	// add/sub imm
			return Cai | opai(ir);
		}
		switch(b2823) {
		case 0x24:	// logic imm
			return Cali | opali(ir);
		case 0x25:	// move wide imm
			return Camwi | opamwi(ir);
		case 0x26:	// bitfield
			return Cab | opab(ir);
		case 0x27:	// extract
			return Cax | opax(ir);
		}
		return Cundef;
	case 5:	// branches, exceptions, syscalls
		if(b3026 == 5)	// unconditional branch imm
			return Cubi | opubi(ir);
		switch(b3025) {
		case 0x1A:	// compare and branch
			return Ccmpb | opcmpb(ir);
		case 0x1B:	// test and branch
			return Ctb | optb(ir);
		}
		switch(b3125) {
		case 0x2A:	// conditional branch
			return Ccb | opcb(ir);
		case 0x6B:	// unconditional branch reg
			return Cubr | opubr(ir);
		}
		if(b3124 == 0xD4)	// system
			return Csys | opsys(ir);
		return Cundef;
	}
	if(b27 == 1 && b25 == 0) {	// loads and stores
		if(b2924 == 8)	// load/store ex
			return Clsx | oplsx(ir);
		if (b2927 == 3 && b2524 == 0)	// load/store reg
			return Clsr | oplsr(ir);
		switch(b2927) {
		case 5:	// lsnp, lsrpo, lsppo, lsppr
			switch(b2523) {
			case 0:	// load/store no-alloc pair (off)
				return Clsnp | oplsnp(ir);
			case 1:	// load/store reg-pair (post-index)
				return Clsppo | oplsppo(ir);
			case 2:	// load/store reg-pair (off)
				return Clsrpo | oplsrpo(ir);
			case 3:	// load/store reg-pair (pre-index)
				return Clsppr | oplsppr(ir);
			}
			return Cundef;
		case 7:	// lspos, lspre, lso, lsu, lsuci, lsusi
			if(b2524 == 1)	// load/store reg (unsigned imm)
				return Clsusi | oplsusi(ir);
			else if(b2524 == 0) {
				if(b21 == 1) {
					if(b1110 == 2)	// load/store reg (off)
						return Clso | oplso(ir);
					return Cundef;
				}
				switch(b1110) {
				case 0:	// load/store reg (unscaled imm)
					return Clsuci | oplsuci(ir);
				case 1:	// load/store reg (imm post-index)
					return Clspos | oplspos(ir);
				case 2:	// load/store reg (unpriv)
					return Clsu | oplsu(ir);
				case 3:	// load/store reg (imm pre-index)
					return Clspre | oplspre(ir);
				}
			}
			return Cundef;
		}
		return Cundef;
	}
	if(b2725 == 5) {	// data processing (reg)
		switch(b2824) {
		case 0xA:	// logic shift-reg
			return Calsr | opalsr(ir);
		case 0xB:	// ar, asr
			if(b21 == 1)	// add/sub extended reg
				return Car | opar(ir);
			return Casr | opasr(ir);	// add/sub shift-reg
		case 0x1A:	// ac, aci, acr, acs, a1, a2
			switch(b2321) {
			case 0:	// add/sub carry
				return Cac | opac(ir);
			case 2:	// aci, acr
				if(b11 == 1)	// cond compare imm
					return Caci | opaci(ir);
				return Cacr | opacr(ir);	// cond compare reg
			case 4:	// cond select
				return Cacs | opacs(ir);
			case 6:	// a1, a2
				if(b30 == 1) // data proc 1 src
					return Ca1 | opa1(ir);
				return Ca2 | opa2(ir);	// data proc 2 src
			}
			return Cundef;
		case 0x1B:	// a3    data proc 3 src
			return Ca3 | opa3(ir);
		}
		return Cundef;
	}
	return Cundef;
}

void
ilock(int)
{
}

void
undef(ulong ir)
{
	Bprint(bioout, "undefined instruction IR #%.8lux (xo=%lud, pc=#%.8lux)\n", ir, getxo(ir), reg.pc);
	longjmp(errjmp, 0);
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
	USED(sf, op);
	if(trace)
		itrace("%s\timm19=%d, Rt=%d", ci->name, imm19, Rt);
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
	USED(o0);
	if(trace)
		itrace("%s\to1=%d, imm19=%d, cond=%d", ci->name, o1, imm19, cond);
}

/* system
params: imm16<20,5> 
ops: opc<23,21> LL<1,0> 
	SVC   	opc=0	LL=1	
*/
void
syscall(ulong ir)
{
	ulong opc, imm16, LL;

	getsys(ir);
	USED(opc, LL);
	if(trace)
		itrace("%s\timm16=%d", ci->name, imm16);
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
	USED(op);
	if(trace)
		itrace("%s\tb5=%d, b40=%d, imm14=%d, Rt=%d", ci->name, b5, b40, imm14, Rt);
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
	USED(op);
	if(trace)
		itrace("%s\timm26=%d", ci->name, imm26);
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
	USED(opc, op2);
	if(trace)
		itrace("%s\tRn=%d", ci->name, Rn);
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
	ulong imm;
	uvlong r;

	getai(ir);
	if(shift)
		imm = imm12<<12;
	else
		imm = imm12;
	if(sf)
		r = reg.r[Rn];
	else
		r = (ulong)reg.r[Rn];
	switch(op) {
	case 0:	/* add */
		r += imm;
		break;
	case 1: /* sub */
		r -= imm;
		break;
	}
	reg.r[Rd] = r;
	if(S)
		undef(ir);

	if(trace)
		itrace("%s\tshift=%d, imm12=%d, Rn=%d, Rd=%d", ci->name, shift, imm12, Rn, Rd);
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
	USED(sf, opc, N);
	if(trace)
		itrace("%s\timmr=%d, imms=%d, Rn=%d, Rd=%d", ci->name, immr, imms, Rn, Rd);
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
	USED(sf, op21, N, o0);
	if(trace)
		itrace("%s\tRm=%d, imms=%d, Rn=%d, Rd=%d", ci->name, Rm, imms, Rn, Rd);
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
	USED(sf, opc);
	if(trace)
		itrace("%s\tN=%d, immr=%d, imms=%d, Rn=%d, Rd=%d", ci->name, N, immr, imms, Rn, Rd);
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
	USED(sf, opc);
	if(trace)
		itrace("%s\thw=%d, imm16=%d, Rd=%d", ci->name, hw, imm16, Rd);
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
	USED(op, immlo);
	if(trace)
		itrace("%s\timmhi=%d, Rd=%d", ci->name, immhi, Rd);
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
	USED(sf, op, S, opt);
	if(trace)
		itrace("%s\tRm=%d, option=%d, imm3=%d, Rn=%d, Rd=%d", ci->name, Rm, option, imm3, Rn, Rd);
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
	USED(sf, op, S, shift);
	if(trace)
		itrace("%s\tRm=%d, imm6=%d, Rn=%d, Rd=%d", ci->name, Rm, imm6, Rn, Rd);
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
	USED(sf, op, S);
	if(trace)
		itrace("%s\tRm=%d, Rn=%d, Rd=%d", ci->name, Rm, Rn, Rd);
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
	USED(sf, op, S);
	if(trace)
		itrace("%s\timm5=%d, cond=%d, Rn=%d, nzcv=%d", ci->name, imm5, cond, Rn, nzcv);
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
	USED(sf, op, S);
	if(trace)
		itrace("%s\tRm=%d, cond=%d, Rn=%d, nzcv=%d", ci->name, Rm, cond, Rn, nzcv);
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
	USED(sf, op, S, op2);
	if(trace)
		itrace("%s\tRm=%d, cond=%d, Rn=%d, Rd=%d", ci->name, Rm, cond, Rn, Rd);
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
	USED(sf, S, opcode);
	if(trace)
		itrace("%s\tRn=%d, Rd=%d", ci->name, Rn, Rd);
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
	USED(sf, opcode);
	if(trace)
		itrace("%s\tRm=%d, Rn=%d, Rd=%d", ci->name, Rm, Rn, Rd);
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
	USED(sf, op31, o0);
	if(trace)
		itrace("%s\tRm=%d, Ra=%d, Rn=%d, Rd=%d", ci->name, Rm, Ra, Rn, Rd);
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
	uvlong Xn, m, r;
	ulong Wn, m32;

	getalsr(ir);
	m = doshift(sf, reg.r[Rm], shift, imm6);
	m32 = (ulong)m;
	if(Rn == 31)
		Xn = 0;
	else
		Xn = reg.r[Rn];
	Wn = (ulong)Xn;
	SET(r);	/* silence the compiler */
	switch(opc<<1|N) {
	case 0:	/* AND */
		if(sf == 1)
			r = Xn & m;
		else
			r = Wn & m32;
		break;
	case 2:	/* ORR */
		if(sf == 1)
			r = Xn | m;
		else
			r = Wn | m32;
		break;
	default:
		undef(ir);
	}
	reg.r[Rd] = r;
	if(trace)
		itrace("%s\tshift=%d, Rm=%d, imm6=%d, Rn=%d, Rd=%d", ci->name, shift, Rm, imm6, Rn, Rd);
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
	long addr;

	getlsr(ir);
	USED(V);
	addr = reg.pc;
	if(imm19 >> 16)
		addr += ~((1<<19)-1) | imm19<<2;
	else
		addr += imm19<<2;
	switch(opc) {
	case 0:	/* 32-bit LDR */
		reg.r[Rt] = getmem_w(addr);
		break;
	case 1:	/* 64-bit LDR */
		reg.r[Rt] = getmem_v(addr);
		break;
	default:
		undef(ir);
	}
	if(trace)
		itrace("%s\timm19=%d, Rt=%d", ci->name, imm19, Rt);
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
	USED(size, o2, L, o1, o0);
	if(trace)
		itrace("%s\tRs=%d, Rt2=%d, Rn=%d, Rt=%d", ci->name, Rs, Rt2, Rn, Rt);
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
	USED(opc, V, L);
	if(trace)
		itrace("%s\timm7=%d, Rt2=%d, Rn=%d, Rt=%d", ci->name, imm7, Rt2, Rn, Rt);
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
	USED(size, V, opc);
	if(trace)
		itrace("%s\timm9=%d, Rn=%d, Rt=%d", ci->name, imm9, Rn, Rt);
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
	USED(size, V, opc);
	if(trace)
		itrace("%s\timm9=%d, Rn=%d, Rt=%d", ci->name, imm9, Rn, Rt);
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
	USED(size, V, opc);
	if(trace)
		itrace("%s\tRm=%d, option=%d, S=%d, Rn=%d, Rt=%d", ci->name, Rm, option, S, Rn, Rt);
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
	USED(size, V, opc);
	if(trace)
		itrace("%s\timm9=%d, Rn=%d, Rt=%d", ci->name, imm9, Rn, Rt);
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
	USED(size, V, opc);
	if(trace)
		itrace("%s\timm9=%d, Rn=%d, Rt=%d", ci->name, imm9, Rn, Rt);
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
	USED(size, V, opc);
	if(trace)
		itrace("%s\timm12=%d, Rn=%d, Rt=%d", ci->name, imm12, Rn, Rt);
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
	USED(opc, V, L);
	if(trace)
		itrace("%s\timm7=%d, Rt2=%d, Rn=%d, Rt=%d", ci->name, imm7, Rt2, Rn, Rt);
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
	USED(opc, V, L);
	if(trace)
		itrace("%s\timm7=%d, Rt2=%d, Rn=%d, Rt=%d", ci->name, imm7, Rt2, Rn, Rt);
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
	USED(opc, V, L);
	if(trace)
		itrace("%s\timm7=%d, Rt2=%d, Rn=%d, Rt=%d", ci->name, imm7, Rt2, Rn, Rt);
}