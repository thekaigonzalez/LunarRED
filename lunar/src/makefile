SRC=lobj.c lbuf.c ltok.c llex.c lerr.c lgen.c lcg.c lstd.c lmem.c -lwArgLib
WARN=-Wno-implicit-int -Wno-pointer-to-int-cast -Wno-int-conversion
MODE=-D__NEXFUSE__ -I../../ArgumentWizard/wArgLib

install-deps:
	make -C ../../ArgumentWizard/wArgLib lib
	mv ../../ArgumentWizard/wArgLib/libwArgLib.so /usr/lib/libwArgLib.so

all:
	gcc -o ltest test.c $(SRC) $(WARN) $(MODE) -g
	gcc -O2 -o lunar lunar.c $(SRC) $(WARN) $(MODE) -g
	gcc -O2 -o bytedump bytedump.c $(SRC) $(WARN) $(MODE) -g
