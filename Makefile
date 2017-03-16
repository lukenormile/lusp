CC=cc
CFLAGS=-I -std=c99 -g -Werror -Wall
LDFLAGS=-ledit -lm
SOURCES=parsing.c mpc.c operations.c
DEPS=$(SOURCES:.c=.h)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=parsing

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

parsing: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm $(OBJECTS)
