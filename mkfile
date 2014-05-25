</$objtype/mkfile

TARG=qi
OFILES= qi.$O\
	run.$O\
	mem.$O\
	syscall.$O\
	stats.$O\
	icache.$O\
	symbols.$O\
	cmd.$O\
	bpt.$O\
	float.$O\
	branch.$O\
	iu.$O\

HFILES=power.h\

BIN=/$objtype/bin

UPDATE=\
	mkfile\
	$HFILES\
	${OFILES:%.$O=%.c}\

</sys/src/cmd/mkone

acid:	
	$CC -a run.c > acid.def
