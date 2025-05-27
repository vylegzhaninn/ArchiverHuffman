#ifndef HEAP_H
#define HEAP_H
#include <stdlib.h>
#include <stdio.h>
#include "algofhuffman.h"
#define MAX_SIZE 1000
#define MAX_COUNT 256

typedef struct {
    struct tree_node_t* data[MAX_SIZE];
    int size;
} MinHeap;

void swap(struct tree_node_t** a, struct tree_node_t** b);
void heapify(MinHeap* heap, int i);
void insert(MinHeap* heap, struct tree_node_t* node);
struct tree_node_t* extractMin(MinHeap* heap);
MinHeap* MakeEmptyHeap();
#endif