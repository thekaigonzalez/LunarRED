SRC=lobj.c lbuf.c ltok.c llex.c lerr.c lgen.c lcg.c lstd.c
WARN=-Wno-implicit-int -Wno-pointer-to-int-cast -Wno-int-conversion
MODE=-D__NEXFUSE__

all:
	gcc -o ltest test.c $(SRC) $(WARN) $(MODE) -g
	gcc -O2 -o lunar lunar.c $(SRC) $(WARN) $(MODE) -g
