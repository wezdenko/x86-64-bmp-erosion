CC=gcc
ASMBIN=nasm

CFILE=erosion
ASMFILE=MiddleErosion
ASMFILE2=RightEdgeErosion
ASMFILE3=LeftEdgeErosion

all : asm cc link clean
asm :
	$(ASMBIN) -o $(ASMFILE).o -f elf64 $(ASMFILE).asm
cc :
	$(CC) -c -g -O0 $(CFILE).c
link :
	$(CC) -o $(CFILE) $(CFILE).o $(ASMFILE).o
clean :
	rm *.o
