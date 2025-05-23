#ifndef HUFFMAN_HEAP_H
#define HUFFMAN_HEAP_H
#include <stdlib.h>
#include <stdio.h>
#define MAX_SIZE 1000
#define MAX_COUNT 256

struct tree_node_t {
    double f;
    char sym;
    struct tree_node_t* left;
    struct tree_node_t* right;
};

typedef struct {
    struct tree_node_t* data[MAX_SIZE];
    int size;
} MinHeap;

void swap(struct tree_node_t** a, struct tree_node_t** b);
void heapify(MinHeap* heap, int i);
void insert(MinHeap* heap, struct tree_node_t* node);
struct tree_node_t* extractMin(MinHeap* heap);
MinHeap* MakeEmptyHeap();
struct tree_node_t* NewTreeFromSym(char sym, double f);
struct tree_node_t* NewTreeFromTrees(struct tree_node_t* tree1, struct tree_node_t* tree2);
struct tree_node_t* BuildHuffmanTree(int n, double* f);
void freeHuffmanTree(struct tree_node_t* root);

#endif
