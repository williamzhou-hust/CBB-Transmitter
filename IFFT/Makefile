header_path:=../headers/
src_path:=../IFFT/
h1:=$(header_path)commonStructure.h
h2:=$(header_path)globalVarINIT.h
h3:=$(header_path)integerTypeDef.h
h4:=$(header_path)process.h
h5:=$(header_path)process_data.h

CC:=gcc
vpath %.h header_path
vpath %.c src_path

.PHONY:all
all:ifft.o ifftShiftandIFFTPreamble.o

ifft.o:$(h1) $(h2) $(h4)
	$(CC) -c ifft.c -o $@
ifftShiftandIFFTPreamble.o:$(h1) $(h2) $(h4)
	$(CC) -c ifftShiftandIFFTPreamble.c -o $@

.PHONY:clean
clean:
	-rm *.o
