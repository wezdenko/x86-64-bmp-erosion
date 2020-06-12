CC=gcc
ASMBIN=nasm

CFILE=erosion
ASMFILE=MiddleErosion
ASMFILE2=RightEdgeErosion
ASMFILE3=LeftEdgeErosion

all : cc link clean
cc :
	$(CC) -c -g -O0 $(CFILE).c
link :
	$(CC) -o $(CFILE) $(CFILE).o
clean :
	rm *.o
