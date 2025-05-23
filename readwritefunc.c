#include "readwritefunc.h"

static uint8_t HuffmanCodes[MAX_SIZE][32];
static uint8_t HuffmanCodesSize[MAX_SIZE];

static uint8_t buffer = 0;
static int idxbuffer = 0;

void buildHuffmanCodes(struct tree_node_t* root, uint8_t *bits, int depth) {
    if (!root) return;

    if (!root->left && !root->right) {
        uint8_t idx = root->sym;
        memcpy(HuffmanCodes[idx], bits, 32);
        HuffmanCodesSize[idx] = depth;
        return;
    }

    buildHuffmanCodes(root->left, bits, depth + 1);

    uint8_t byte_idx = depth / 8;
    uint8_t bit_mask = 128 >> (depth % 8);
    uint8_t old_byte = bits[byte_idx];
    bits[byte_idx] |= bit_mask;

    buildHuffmanCodes(root->right, bits, depth + 1);
    bits[byte_idx] = old_byte;
}


void WriteBits(uint8_t sym, FILE *output) {
    uint8_t *code = HuffmanCodes[sym];
    int bitlen = HuffmanCodesSize[sym];

    for (int i = 0; i < bitlen; ++i) {
        int byte_idx = i / 8;
        int bit_idx = 7 - (i % 8);
        uint8_t bit = (code[byte_idx] >> bit_idx) & 1;

        buffer = (buffer << 1) | bit;
        idxbuffer++;

        if (idxbuffer == 8) {
            fwrite(&buffer, 1, 1, output);
            buffer = 0;
            idxbuffer = 0;
        }
    }
}

void EncodingSym(FILE *input, FILE *output) {
    uint8_t ch;
    while (fread(&ch, 1, 1, input) == 1) {
        WriteBits(ch, output);
    }

    if (idxbuffer > 0) {
        buffer <<= (8 - idxbuffer);
        fwrite(&buffer, 1, 1, output);
        buffer = 0;
        idxbuffer = 0;
    }
}

void writeHuffmanTreeToFile(struct tree_node_t* root, FILE* output) {
    if (!root) return;

    if (!root->left && !root->right) {
        uint8_t marker = 1;
        fwrite(&marker, 1, 1, output);
        fwrite(&root->sym, 1, 1, output);
    } else {
        uint8_t marker = 0;
        fwrite(&marker, 1, 1, output);
        writeHuffmanTreeToFile(root->left, output);
        writeHuffmanTreeToFile(root->right, output);
    }
}

struct tree_node_t* readHuffmanTreeFromFile(FILE* input) {
    uint8_t marker;
    if (fread(&marker, sizeof(uint8_t), 1, input) != 1){
        perror("Ошибка чтения маркера!\n");
        exit(1);
    }
    struct tree_node_t* node = malloc(sizeof(struct tree_node_t));
    if (node == NULL) {
        perror("Ошибка выделения памяти для узла дерева!\n");
        exit(1);
    }

    if (marker == 1) {
        if (fread(&node->sym, sizeof(char), 1, input) != 1){
            perror("Ошибка чтения символа!\n");
            exit(1);
        }
        node->left = NULL;
        node->right = NULL;
    } else {
        node->left = readHuffmanTreeFromFile(input);
        node->right = readHuffmanTreeFromFile(input);
        node->sym = 0;
    }

    return node;
}


void DecodingSym(FILE *input, FILE *output, struct tree_node_t *root, int total_chars) {
    struct tree_node_t *current = root;
    uint64_t chars_decoded = 0;
    uint8_t byte;
    int bit_index = 0;

    while (chars_decoded < total_chars) {
        if (bit_index == 0) {
            if (fread(&byte, 1, 1, input) != 1) {
                perror("В исходном в файле не полное кол-во символов!\n");
                exit(1);
            }
        }

        uint8_t bit = (byte >> (7 - bit_index)) & 1;

        if (bit == 0) {
            current = current->left;
        } else {
            current = current->right;
        }

        if (!current->left && !current->right) {
            fwrite(&current->sym, 1, 1, output);
            chars_decoded++;
            current = root;
        }

        bit_index = (bit_index + 1) % 8;
    }
}
