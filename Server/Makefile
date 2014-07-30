CC=gcc
CFLAGS=-c -g -Wall
LDFLAGS=
SOURCES=HttpServer.c util.c
OBJS=$(SOURCES:.c=.o)
EXECUTABLE=hs

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(EXECUTABLE) $(OBJS)

