#ifndef WRITEFUNC
#define WRITEFUNC

void buildHuffmanCodes(struct tree_node_t* root, uint8_t *bits, int depth);
void WriteBits(uint8_t sym, FILE *output);
void EncodingSym(FILE *input, FILE *output);
void writeHuffmanTreeToFile(struct tree_node_t* root, FILE* output);
struct tree_node_t* readHuffmanTreeFromFile(FILE* input);
void DecodingSym(FILE *input, FILE *output, struct tree_node_t *root, uint64_t total_chars);

#endif