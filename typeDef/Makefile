CC:=gcc
header_path:=../headers/

commonStructure.o:$(header_path)commonStructure.h
	$(CC) -c commonStructure.c -o $@

.PHONY:clean
clean:
	-rm *.o
