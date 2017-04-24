CC:=gcc
CFLAGS:=-g -c -mavx2
intrinsics_interface.o:intrinsics_interface_v2.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY:clean
clean:
	-rm *.o
