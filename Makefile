lib_BCCencode:=./BCCencode/
lib_Process:=./Process/
lib_process_data:=./process_data/
lib_typeDef:=./typeDef/
lib_globalVarINIT:=./VarINIT/
lib_intrinsics:=./intrinsics_interface/
libraries:=$(lib_BCCencode) $(lib_Process) $(lib_process_data)\
	$(lib_typeDef) $(lib_globalVarINIT) $(lib_intrinsics)

CC:=gcc
MV:=-mv -u
CFLAGS:=-c -D OPTIMIZATION

.PHONY:all $(libraries)
all:$(libraries) Transmitter
$(libraries):
	$(MAKE) --directory=$@

vpath %.o libraries
Objects=mainbfBCC.o test.o $(lib_BCCencode)ccoding_byte.o $(lib_Process)bccInterleaverForSig.o\
		$(lib_Process)csdForHeLTF.o $(lib_Process)csdForPreamble.o $(lib_Process)generateBasicSig.o\
		$(lib_Process)generateHeLTF_csd.o $(lib_Process)generateLTF_csd.o\
		$(lib_Process)generatePreamble_csd.o $(lib_Process)generateSig_csd.o $(lib_Process)generateSTF_csd.o\
		$(lib_Process)MapForSig.o $(lib_Process)setSigInfo.o\
		$(lib_process_data)mcs_table_for_20M.o $(lib_process_data)process_data.o\
		$(lib_process_data)BCC_encoder.o $(lib_process_data)Data_CSD.o $(lib_process_data)GenDataAndScramble.o\
		$(lib_process_data)GenInit.o $(lib_process_data)modulate.o $(lib_process_data)PilotAdd_SubcarMap.o\
		$(lib_typeDef)commonStructure.o $(lib_globalVarINIT)globalVarINIT.o $(lib_intrinsics)intrinsics_interface.o
Transmitter:$(Objects)
	$(CC) -g $(Objects) -lm -o $@
	$(MV)  $(lib_BCCencode)*.o $(lib_Process)*.o $(lib_process_data)*.o $(lib_typeDef)*.o\
		$(lib_globalVarINIT)*.o $(lib_intrinsics)*.o *.o ./objs
	./Transmitter
	$(MV) csd_*.txt *_csd.txt ./dataForRun

mainbfBCC.o:allHeaders.h
	$(CC) $(CFLAGS) -g mainbfBCC.c -o $@
test.o:test.c
	$(CC) -c $< -o $@

.PHONY:clean
clean:
	-rm *.o
	-rm $(lib_BCCencode)*.o
	-rm $(lib_Process)*.o
	-rm $(lib_process_data)*.o
	-rm $(lib_typeDef)*.o
	-rm $(lib_globalVarINIT)*.o
	-rm $(lib_intrinsics)*.o
	-rm ./objs/*.o
	-rm Transmitter
	-rm csd_*.txt
	-rm *_csd.txt
