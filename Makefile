CC = gcc
CFLAGS = -O3 -Wall
LDFLAGS = 
CSRC = pifuscate.c
COBJ = $(CSRC:.c=.o)

default: pifuscate

pifuscate: $(COBJ)
	$(CC) $(COBJ) $(LDFLAGS) -o pifuscate

.o:
	$(CC) $(CFLAGS) -c $<
	
clean:
	rm -f ./*.o ./pifuscate
