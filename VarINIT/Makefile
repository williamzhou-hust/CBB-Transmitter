CC:=gcc
header_path:=../headers/

commonStructure.o:$(header_path)globalVarINIT.h
	$(CC) -c globalVarINIT.c -o $@

.PHONY:clean
clean:
	-rm *.o
