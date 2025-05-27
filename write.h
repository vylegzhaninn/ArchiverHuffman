#ifndef WRITE_H
#define WRITE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "algofhuffman.h"

void buildHuffmanCodes(struct tree_node_t* root, uint8_t *bits, int depth);
void WriteBits(uint8_t sym, FILE *output);
void EncodingSym(FILE *input, FILE *output);
void writeHuffmanTreeToFile(struct tree_node_t* root, FILE* output);

#endif
