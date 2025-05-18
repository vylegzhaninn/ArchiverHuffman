#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "algofhuffman.h"
#include "writefunc.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Ошибка: недостаточно аргументов!\n");
        printf("Используйте: ./huffman [--help | -en | -de] file1 ...\n");
        return 1;
    }    

    int mode = 12345678;
    if (strcmp(argv[1], "-en") == 0) mode = 1;
    if (strcmp(argv[1], "-de") == 0) mode = 0;

    if (strcmp(argv[1], "--help") == 0){
        printf("Usage ./huffman options file1, file2, ...\nOptions:\n");
        printf("  -en    -> encode file.\n");
        printf("  -de    -> decode file.\n"); 
        printf("  --help -> documentation.\n");
        return 1;
    }

    if (mode != 1 && mode != 0){
        printf(" Режим работы передан не верно!\n Используйте для кодирования - en\n Используйте для декодирования - de\n");
        printf("for help -> ./huffman --help");
        return 1;
    }

    long int *sizeinput = malloc(sizeof(long int) * (argc - 2));
    long int *sizeoutput = malloc(sizeof(long int) * (argc - 2));
    double* compressed = malloc(sizeof(double) * (argc - 2));

    for (int i = 2; i < argc; i++){

        if (mode){

            char *output_filename;
            output_filename = malloc(sizeof(char) * 30);
            sprintf(output_filename, "encodefile(%d).txt", i - 1);
            struct tree_node_t* huffman_tree;
            printf("Открытие файлов\n");
            FILE* input = fopen(argv[i], "rb");
            FILE* output = fopen(output_filename, "wb");

            if (input == NULL || output == NULL){
                printf("Ошибка открытия файлов\n");
                printf("for help -> ./huffman --help");
                return 1;
            }

            double frequencies[MAX_COUNT] = {0};
            int total_chars = 0;
            uint8_t ch;
            printf("Считывание символов\n");
            while (fread(&ch, 1, 1, input) == 1) {  
                frequencies[ch]++;
                total_chars++;
            }

            if (total_chars == 0){
                printf("Пустой файл\n");
                return 1;
            }

            for (int i = 0; i < MAX_COUNT; i++)
                frequencies[i] /= total_chars;

            printf("Построение дерева хафмена\n");
            huffman_tree = BuildHuffmanTree(MAX_COUNT, frequencies);

            uint8_t bits[32] = {0};
            printf("Построение кодов хафмена\n");
            buildHuffmanCodes(huffman_tree, bits, 0);
            
            printf("Запись кодов хафмена в исходный файл\n");
            writeHuffmanTreeToFile(huffman_tree, output);
            fwrite(&total_chars, sizeof(int), 1, output);
            rewind(input);
            printf("Кодирование символов\n");
            EncodingSym(input, output);

            sizeinput[i - 2] = ftell(input);
            sizeoutput[i - 2] = ftell(output);
            compressed[i - 2] = (double) sizeoutput[i - 2] / sizeinput[i - 2];
   
            fclose(input);
            fclose(output); 

            freeHuffmanTree(huffman_tree);
            free(output_filename);
        }else{
            char *output_filename = malloc(sizeof(char) * 30);
            sprintf(output_filename, "decodefile(%d).txt", i - 1);
            struct tree_node_t* huffman_tree;

            printf("Открытие файлов\n");
            FILE* input = fopen(argv[i], "rb");
            FILE* output = fopen(output_filename, "wb");
            
            if (input == NULL || output == NULL){
                printf("Ошибка открытия файлов\n");
                return 1;
            }

            printf("Считывание дерева хафмена\n");
            huffman_tree = readHuffmanTreeFromFile(input);
            int total_chars = 0;
            printf("Декодирование символов\n");
            fread(&total_chars, sizeof(int), 1, input);
            DecodingSym(input, output, huffman_tree, total_chars);

            printf("Файл (%s) успешно декодирован.\n\n", argv[i]);
            fclose(input);
            fclose(output);

            freeHuffmanTree(huffman_tree);
            free(output_filename);
        }
    }

    if (mode){
        printf("\n");
        if (argc > 3) printf("Файлы успешно закодированы.\n\n");
        else printf("Файл успешно закодирован.\n\n");
        for (int i = 0; i < argc - 2; i++){
            printf("%s\n", argv[i + 2]);
            printf("Размер входного файла: %ld\n", sizeinput[i]);
            printf("Размер выходного файла: %ld\n", sizeoutput[i]);
            printf("Коэффициент сжатия: %.2f\n", compressed[i]);  
            printf("\n");
        }
    }

    return 0;
}
