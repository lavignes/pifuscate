CC = gcc
LD = ld
CFLAGS = -g -O3 -Wall
LDFLAGS = 
CSRC = pifuscate.c
COBJ = $(CSRC:.c=.o)

default: pifuscate

pifuscate: pi.o $(COBJ)
	$(CC) $(COBJ) pi.o $(LDFLAGS) -o pifuscate

.o:
	$(CC) $(CFLAGS) -c $<
	
pi.o:
	$(LD) -r -b binary -o pi.o pi.bin
	
clean:
	rm -f ./*.o ./pifuscate
