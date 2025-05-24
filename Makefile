CC       = gcc
CFLAGS   = -Wall -Wextra -g
SRCS     = main.c algofhuffman.c readwritefunc.c

all: huffman

huffman: $(SRCS)
	$(CC) $(CFLAGS) -DBYTESIZE1 -o $@ $(SRCS)

clean:
	rm -f huffman1 huffman2
