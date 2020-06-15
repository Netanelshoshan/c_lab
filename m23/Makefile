CC = gcc
CFLAGS = -Wall -ansi -pedantic
EXECUTABLE = numbers

%.o	:%.c
	$(CC) $(CFLAGS) -c -o $@ $< 
$(EXECUTABLE):	numbers.o
	$(CC) -o $(EXECUTABLE) numbers.o && rm -rf *.o
clean:
	rm -f $(EXECUTABLE) *.o
