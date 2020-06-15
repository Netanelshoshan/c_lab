CC = gcc
CFLAGS = -Wall -ansi -pedantic
EXECUTABLE = adjacency
%.o	:	%.c 
		$(CC) -c -o $@ $< $(CFLAGS)
$(EXECUTABLE):	adjacency.o
		$(CC) -o $(EXECUTABLE) adjacency.o && rm -rf *.o
clean:
		rm -f $(EXECUTABLE) *.o


