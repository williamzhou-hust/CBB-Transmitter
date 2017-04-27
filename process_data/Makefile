header_path:=../headers/
src_path:=./process_datafunction/
h1:=$(header_path)commonStructure.h
h2:=$(header_path)globalVarINIT.h
h3:=$(header_path)integerTypeDef.h
h4:=$(header_path)process.h
h5:=$(header_path)process_data.h

CFLAGS:=-c -D AVX2 -D OPTIMIZATION 
CC:=gcc
vpath %.h header_path
vpath %.c .
vpath %.c process_datafunction

.PHONY:all
all:mcs_table_for_20M.o process_data.o BCC_encoder.o Data_CSD.o GenDataAndScramble.o GenInit.o modulate.o PilotAdd_SubcarMap.o

mcs_table_for_20M.o:$(h2)
	$(CC) $(CFLAGS) mcs_table_for_20M.c -o $@
process_data.o:$(h1) $(h2) $(h5)
	$(CC) $(CFLAGS) process_data.c -o $@
BCC_encoder.o:$(src_path)BCC_encoder.c $(h5) $(h2) $(h4)
	$(CC) $(CFLAGS) $(src_path)BCC_encoder.c -o $@
GenDataAndScramble.o:$(src_path)GenDataAndScramble.c $(h2)
	$(CC) $(CFLAGS) $(src_path)GenDataAndScramble.c -o $@
GenInit.o:$(src_path)GenInit.c $(h2) $(h5)
	$(CC) $(CFLAGS) $(src_path)GenInit.c -o $@
Data_CSD.o:$(src_path)Data_CSD.c $(h1) $(h2) $(h5)
	$(CC) $(CFLAGS) $(src_path)Data_CSD.c -o $@
modulate.o:$(src_path)modulate.c $(h1) $(h2) $(h5)
	$(CC) $(CFLAGS) $(src_path)modulate.c -o $@
PilotAdd_SubcarMap.o:$(src_path)PilotAdd_SubcarMap.c $(h1) $(h2)
	$(CC) $(CFLAGS) $(src_path)PilotAdd_SubcarMap.c -o $@

.PHONY:clean
clean:
	-rm *.o
