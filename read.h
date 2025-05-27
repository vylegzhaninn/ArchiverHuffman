#ifndef READ_H
#define READ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "algofhuffman.h"

struct tree_node_t* readHuffmanTreeFromFile(FILE* input);
void DecodingSym(FILE *input, FILE *output, struct tree_node_t *root, uint64_t total_chars);

#endif