CC = gcc
CFLAGS = -Wall -ansi -pedantic
EXECUTABLE = assembler
SAMPLES = *.ob *.ent *.ext

all: assembler

assembler: assembler.c
	$(CC) $(CFLAGS) -g *.c -o $(EXECUTABLE) -lm

clean:
	rm -rf $(EXECUTABLE) && cd tests/ && rm -rf $(SAMPLES)

a:
	rm -rf $(EXECUTABLE) && cd tests/ && rm -rf $(SAMPLES)
	$(CC) $(CFLAGS) -g *.c -o $(EXECUTABLE) -lm
