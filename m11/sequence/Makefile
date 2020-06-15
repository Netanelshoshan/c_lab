CC = gcc
CFLAGS = -Wall -ansi -pedantic
EXECUTABLE = sequence
%.o:	%.c 
	$(CC) -c -o $@ $< $(CFLAGS)
$(EXECUTABLE):	sequence.o
	$(CC) -o $(EXECUTABLE) sequence.o && rm -rf *.o  
clean:
	rm -f $(EXECUTABLE) *.o


