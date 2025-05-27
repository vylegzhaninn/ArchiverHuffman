#include "read.h"

struct tree_node_t* readHuffmanTreeFromFile(FILE* input) {
    uint8_t marker;
    if (fread(&marker, sizeof(uint8_t), 1, input) != 1) {
        perror("Ошибка чтения маркера дерева Хаффмана");
        exit(1);
    }
    struct tree_node_t* node = malloc(sizeof(struct tree_node_t));
    if (node == NULL) {
        perror("Ошибка выделения памяти для узла дерева");
        exit(1);
    }

    if (marker == 1) {
        if (fread(&node->sym, sizeof(char), 1, input) != 1) {
            perror("Ошибка чтения символа");
            free(node);
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

void DecodingSym(FILE *input, FILE *output, struct tree_node_t *root, uint64_t total_chars) {
    struct tree_node_t *current = root;
    uint64_t chars_decoded = 0;
    uint8_t byte;
    int bit_index = 0;

    while (chars_decoded < total_chars) {
        if (bit_index == 0) {
            if (fread(&byte, 1, 1, input) != 1) {
                if (feof(input)) {
                    fprintf(stderr, "Достигнут конец файла до декодирования всех символов (%d из %ld)\n",
                            (int)chars_decoded, total_chars);
                } else {
                    perror("Ошибка чтения данных из архива");
                }
                freeHuffmanTree(root);
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