#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#define MAX_SIZE 1000
#define MAX_COUNT 256

struct tree_node_t {
    double f;
    char sym;
    struct tree_node_t* left;
    struct tree_node_t* right;
};

struct tree_node_t* NewTreeFromSym(char sym, double f);
struct tree_node_t* NewTreeFromTrees(struct tree_node_t* tree1, struct tree_node_t* tree2);
struct tree_node_t* BuildHuffmanTree(int n, double* f);
void freeHuffmanTree(struct tree_node_t* root);

#endif
