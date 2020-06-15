CC = gcc
CFLAGS = -Wall -ansi -pedantic
EXECUTABLE = match
%.o	:	%.c 
		$(CC) -c -o $@ $< $(CFLAGS)
$(EXECUTABLE):	match.o
		$(CC) -o $(EXECUTABLE) match.o && rm -rf *.o
clean:
		rm -f $(EXECUTABLE) *.o


