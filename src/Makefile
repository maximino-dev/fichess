CC=g++
CFLAGS=-Wall
SRC=engine.cc main.cc
OBJ=$(SRC:.c=.o)

all: engine

engine: $(OBJ)
	$(CC) $^ -o engine

main.o: engine.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f engine *.o