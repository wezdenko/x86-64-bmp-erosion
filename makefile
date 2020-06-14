CC=gcc
ASMBIN=nasm

CFILE=erosion
ASMFILE=MiddleErosion
ASMFILE2=RightEdgeErosion
ASMFILE3=LeftEdgeErosion
ASMFILE4=FourBytesErosion

all : asm cc link clean
asm :
	$(ASMBIN) -o $(ASMFILE).o -f elf64 $(ASMFILE).asm
	$(ASMBIN) -o $(ASMFILE2).o -f elf64 $(ASMFILE2).asm
	$(ASMBIN) -o $(ASMFILE3).o -f elf64 $(ASMFILE3).asm
	$(ASMBIN) -o $(ASMFILE4).o -f elf64 $(ASMFILE4).asm
cc :
	$(CC) -c -g -O0 $(CFILE).c
link :
	$(CC) -o $(CFILE) $(CFILE).o $(ASMFILE).o $(ASMFILE2).o $(ASMFILE3).o $(ASMFILE4).o
clean :
	rm *.o
