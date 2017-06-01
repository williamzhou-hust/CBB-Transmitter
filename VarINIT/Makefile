CC:=gcc
header_path:=../headers/
CFLAGS:=-c -D AVX2

globalVarINIT.o:$(header_path)globalVarINIT.h
	$(CC) $(CFLAGS) globalVarINIT.c -o $@

.PHONY:clean
clean:
	-rm *.o
