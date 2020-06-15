CC = gcc
CFLAGS = -Wall -ansi -pedantic
EXECUTABLE = mycomp

%.o	:%.c
	$(CC) $(CFLAGS) -c -o $@ $< -lm
$(EXECUTABLE):	complex.o mycomp.o
	$(CC) -o $(EXECUTABLE) complex.o mycomp.o -lm && rm -rf *.o
clean:
	rm -f $(EXECUTABLE) *.o


