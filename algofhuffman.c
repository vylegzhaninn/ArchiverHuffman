#include "algofhuffman.h"

void swap(struct tree_node_t** a, struct tree_node_t** b) {
    struct tree_node_t* temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(MinHeap* heap, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap->size && heap->data[left]->f < heap->data[smallest]->f)
        smallest = left;
    if (right < heap->size && heap->data[right]->f < heap->data[smallest]->f)
        smallest = right;

    if (smallest != i) {
        swap(&heap->data[i], &heap->data[smallest]);
        heapify(heap, smallest);
    }
}

void insert(MinHeap* heap, struct tree_node_t* node) {
    if (heap->size == MAX_SIZE) {
        perror("Куча переполнена!\n");
        exit(1);
    }

    int i = heap->size;
    heap->data[i] = node;
    heap->size++;

    while (i != 0 && heap->data[(i - 1) / 2]->f > heap->data[i]->f) {
        swap(&heap->data[i], &heap->data[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

struct tree_node_t* extractMin(MinHeap* heap) {
    if (heap->size <= 0)
        return NULL;

    if (heap->size == 1) {
        heap->size--;
        return heap->data[0];
    }

    struct tree_node_t* root = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapify(heap, 0);

    return root;
}

MinHeap* MakeEmptyHeap() {
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (heap == NULL) {
        perror("Ошибка выделения памяти для кучи!\n");
        exit(1);
    }
    heap->size = 0;
    for (int i = 0; i < MAX_SIZE; i++)
        heap->data[i] = NULL;
    return heap;
}

struct tree_node_t* NewTreeFromSym(char sym, double f) {
    struct tree_node_t* res = malloc(sizeof(struct tree_node_t));
    if (res == NULL) {
        perror("Ошибка выделения памяти для узла дерева!\n");
        exit(1);
    }
    res->f = f;
    res->sym = sym;
    res->left = NULL;
    res->right = NULL;
    return res;
}

struct tree_node_t* NewTreeFromTrees(struct tree_node_t* tree1, struct tree_node_t* tree2) {
    struct tree_node_t* res = malloc(sizeof(struct tree_node_t));
    if (res == NULL) {
        perror("Ошибка выделения памяти для узла дерева!\n");
        exit(1);
    }
    res->f = tree1->f + tree2->f;
    res->left = tree1;
    res->right = tree2;
    return res;
}

struct tree_node_t* BuildHuffmanTree(int n, double* f) {
    MinHeap* heap = MakeEmptyHeap();

    for (int i = 0; i < n; i++) {
        if (f[i]) insert(heap, NewTreeFromSym(i, f[i]));
    }

    while (heap->size > 1) {
        struct tree_node_t* tree1 = extractMin(heap);
        struct tree_node_t* tree2 = extractMin(heap);
        insert(heap, NewTreeFromTrees(tree1, tree2));
    }

    return extractMin(heap);
}

void freeHuffmanTree(struct tree_node_t* root) {
    if (root == NULL) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}
