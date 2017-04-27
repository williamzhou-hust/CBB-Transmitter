header_path:=../headers/
src_path:=../Process/
h1:=$(header_path)commonStructure.h
h2:=$(header_path)globalVarINIT.h
h3:=$(header_path)integerTypeDef.h
h4:=$(header_path)process.h
h5:=$(header_path)process_data.h

CC:=gcc
CFLAGS:=-c -D AVX2
vpath %.h header_path
vpath %.c src_path

.PHONY:all
all:bccInterleaverForSig.o csdForHeLTF.o csdForPreamble.o generateBasicSig.o generateHeLTF_csd.o\
	generateLTF_csd.o generatePreamble_csd.o generateSig_csd.o generateSTF_csd.o MapForSig.o setSigInfo.o

bccInterleaverForSig.o:$(h4)
	$(CC) $(CFLAGS) bccInterleaverForSig.c -o $@
csdForHeLTF.o:$(h1) $(h2) $(h4)
	$(CC) $(CFLAGS) csdForHeLTF.c -o $@
csdForPreamble.o:$(h1) $(h2) $(h4)
	$(CC) $(CFLAGS) csdForPreamble.c -o $@
generateBasicSig.o:$(h1) $(h2) $(h4)
	$(CC) $(CFLAGS) generateBasicSig.c -o $@
generateHeLTF_csd.o:$(h1) $(h2) $(h3) $(h4)
	$(CC) $(CFLAGS) generateHeLTF_csd.c -o $@
generateLTF_csd.o:$(h1) $(h2) $(h4)
	$(CC) $(CFLAGS) generateLTF_csd.c -o $@
generatePreamble_csd.o:$(h1) $(h2) $(h4)
	$(CC) $(CFLAGS) generatePreamble_csd.c -o $@
generateSig_csd.o:$(h1) $(h2) $(h4)
	$(CC) $(CFLAGS) generateSig_csd.c -o $@
generateSTF_csd.o:$(h1) $(h2) $(h4)
	$(CC) $(CFLAGS) generateSTF_csd.c -o $@
MapForSig.o:$(h1) $(h2) $(h4)
	$(CC) $(CFLAGS) MapForSig.c -o $@
setSigInfo.o:$(h4)
	$(CC) $(CFLAGS) setSigInfo.c -o $@

.PHONY:clean
clean:
	-rm *.o


