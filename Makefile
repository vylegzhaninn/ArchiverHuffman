TARGET = huffman
CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c algofhuffman.c read.c write.c out.c heap.c

all: huffman

huffman: $(SRCS)
	$(CC) $(CFLAGS) -DBYTESIZE1 -o $@ $(SRCS)

clean:
	rm -f huffman
