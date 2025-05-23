TARGET = huffman
CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c algofhuffman.c readwritefunc.c

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET)
