#include "algofhuffman.h"

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

    struct tree_node_t* result = extractMin(heap);
    free(heap);
    return result;
}

void freeHuffmanTree(struct tree_node_t* root) {
    if (root == NULL) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}