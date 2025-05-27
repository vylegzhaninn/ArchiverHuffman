#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "algofhuffman.h"
#include "read.h"
#include "write.h"
#include "out.h"
#include <unistd.h>


int main(int argc, char* argv[]) {
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
        printf("┃ Использование: ./huffman [-en | -de] -f выходной_файл [файл1 файл2 ...] ┃\n");
        printf("┃ Опции:                                                                  ┃\n");
        printf("┃  -en          Кодировать файлы в единый архив.                          ┃\n");
        printf("┃  -de          Декодировать архив в исходные файлы.                      ┃\n");
        printf("┃  -f outfile   имя архива для кодирования или декодирования.             ┃\n");
        printf("┃  --help       Показать справочное сообщение.                            ┃\n");
        printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
        return 1;
    }

    int mode = -1;
    char* output_filename = NULL;
    int file_start_idx = 2;

    if (strcmp(argv[1], "-en") == 0) {
        mode = 1;
    } else if (strcmp(argv[1], "-de") == 0) {
        mode = 0;
    } else {
        fprintf(stderr, "Неверный режим. Используйте -en или -de.\n");
        return 1;
    }

    if (argc < 3 || strcmp(argv[2], "-f") != 0) {
        fprintf(stderr, "Отсутствует -f выходной_файл.\n");
        return 1;
    }
    if (argc < 4) {
        fprintf(stderr, "Отсутствует имя выходного файла после -f.\n");
        return 1;
    }
    output_filename = argv[3];
    file_start_idx = 4;

    if (mode == 1 && file_start_idx >= argc) {
        fprintf(stderr, "Не указаны входные файлы для кодирования.\n");
        return 1;
    }
    if (mode == 0 && file_start_idx < argc) {
        fprintf(stderr, "В режиме декодирования не требуются входные файлы.\n");
        return 1;
    }

    FILE* output = fopen(output_filename, mode == 1 ? "wb" : "rb");
    if (output == NULL) {
        perror("Ошибка открытия файла архива");
        return 1;
    }

    if (mode == 1) {
        long int* sizeinput = malloc(sizeof(long int) * (argc - file_start_idx));
        long int* sizeoutput = malloc(sizeof(long int) * (argc - file_start_idx));
        double* compressed = malloc(sizeof(double) * (argc - file_start_idx));
        if (sizeinput == NULL || sizeoutput == NULL || compressed == NULL) {
            perror("Ошибка выделения памяти");
            fclose(output);
            return 1;
        }

        uint32_t num_files = argc - file_start_idx;
        fwrite(&num_files, sizeof(uint32_t), 1, output);

        long int total_output_size = ftell(output);

        for (int i = file_start_idx; i < argc; i++) {
            printf("\rОбработка файла: %s                    ", argv[i]);
            fflush(stdout);
            usleep(500000); // для наглядности

            FILE* input = fopen(argv[i], "rb");
            if (input == NULL) {
                perror("\nОшибка открытия входного файла");
                free(sizeinput);
                free(sizeoutput);
                free(compressed);
                fclose(output);
                return 1;
            }

            printf("\rЗапись вспомогательной информации в файл     ");
            fflush(stdout);
            usleep(500000);

            const char* filename = strrchr(argv[i], '/');
            filename = filename ? filename + 1 : argv[i];
            uint16_t filename_len = strlen(filename);
            fwrite(&filename_len, sizeof(uint16_t), 1, output);
            fwrite(filename, sizeof(char), filename_len, output);

            double frequencies[MAX_COUNT] = {0};
            uint64_t total_chars = 0;
            uint8_t ch;
            while (fread(&ch, 1, 1, input) == 1) {
                frequencies[ch]++;
                total_chars++;
            }

            if (total_chars == 0) {
                printf("\nПредупреждение: Пустой файл %s \nУберите его и попробуйте снова\n", argv[i]);
                fclose(input);
                exit(1);
            }

            int unique_chars = 0;
            uint8_t single_char = 0;
            for (int j = 0; j < MAX_COUNT; j++) {
                if (frequencies[j] > 0) {
                    unique_chars++;
                    single_char = (uint8_t)j;
                }
            }

            if (total_chars == 1 || unique_chars == 1) {
                struct tree_node_t* huffman_tree = NewTreeFromSym(single_char, 1.0);
                writeHuffmanTreeToFile(huffman_tree, output);
                fwrite(&total_chars, sizeof(uint64_t), 1, output);
                uint8_t bit = 0;
                fwrite(&bit, 1, 1, output);
                sizeinput[i - file_start_idx] = ftell(input);
                sizeoutput[i - file_start_idx] = ftell(output) - total_output_size;
                compressed[i - file_start_idx] = (double)sizeoutput[i - file_start_idx] / sizeinput[i - file_start_idx];
                total_output_size = ftell(output);
                fclose(input);
                freeHuffmanTree(huffman_tree);
                continue;
            }

            for (int j = 0; j < MAX_COUNT; j++) {
                frequencies[j] /= total_chars;
            }

            printf("\rЗапись кодов Хаффмена                        ");
            fflush(stdout);
            usleep(500000);

            struct tree_node_t* huffman_tree = BuildHuffmanTree(MAX_COUNT, frequencies);
            uint8_t bits[32] = {0};
            buildHuffmanCodes(huffman_tree, bits, 0);

            writeHuffmanTreeToFile(huffman_tree, output);
            fwrite(&total_chars, sizeof(uint64_t), 1, output);

            printf("\rДекодирование символов                       ");
            fflush(stdout);
            usleep(500000);

            rewind(input);
            EncodingSym(input, output);

            sizeinput[i - file_start_idx] = ftell(input);
            sizeoutput[i - file_start_idx] = ftell(output) - total_output_size;
            compressed[i - file_start_idx] = (double)sizeoutput[i - file_start_idx] / sizeinput[i - file_start_idx];
            total_output_size = ftell(output);

            fclose(input);
            freeHuffmanTree(huffman_tree);
        }
        printf("\r");
        int width = 45;
        char buffer[256];

        print_border(width, 1);
        snprintf(buffer, sizeof(buffer), "Файлы успешно закодированы в %s", output_filename);
        print_line(buffer, width);
        print_border(width, 0);

        print_border(width, 1);
        for (int i = 0; i < argc - file_start_idx; i++) {
            snprintf(buffer, sizeof(buffer), "Файл: %s", argv[i + file_start_idx]);
            print_line(buffer, width);
            snprintf(buffer, sizeof(buffer), "Размер входного файла: %ld байт", sizeinput[i]);
            print_line(buffer, width);
            snprintf(buffer, sizeof(buffer), "Вклад в размер архива: %ld байт", sizeoutput[i]);
            print_line(buffer, width);
            snprintf(buffer, sizeof(buffer), "Коэффициент сжатия: %.2f", compressed[i]);
            print_line(buffer, width);
            if (i != argc - file_start_idx - 1) {
                print_line("", width);
            }
        }
        print_border(width, 0);

        free(sizeinput);
        free(sizeoutput);
        free(compressed);
    } else {
        uint32_t num_files;
        if (fread(&num_files, sizeof(uint32_t), 1, output) != 1) {
            perror("Ошибка чтения количества файлов");
            fclose(output);
            return 1;
        }

        int width = 45;
        char buffer[256];

        print_border(width, 1);
        snprintf(buffer, sizeof(buffer), "Декодирование архива %s", output_filename);
        print_line(buffer, width);
        print_border(width, 0);
        print_border(width, 1);

        for (uint32_t i = 0; i < num_files; i++) {
            printf("\r   Считывание вспомогательных данных     ");
            fflush(stdout);
            usleep(500000);
            uint16_t filename_len;
            if (fread(&filename_len, sizeof(uint16_t), 1, output) != 1) {
                perror("\nОшибка чтения длины имени файла");
                fclose(output);
                return 1;
            }

            char* filename = malloc(filename_len + 1);
            if (filename == NULL) {
                perror("\nОшибка выделения памяти для имени файла");
                fclose(output);
                return 1;
            }
            if (fread(filename, sizeof(char), filename_len, output) != filename_len) {
                perror("\nОшибка чтения имени файла");
                free(filename);
                fclose(output);
                return 1;
            }
            filename[filename_len] = '\0';

            FILE* decoded_output = fopen(filename, "wb");
            if (decoded_output == NULL) {
                perror("\nОшибка открытия файла для декодирования");
                free(filename);
                fclose(output);
                return 1;
            }

            printf("\r   Чтение дерева Хаффмена           ");
            fflush(stdout);
            usleep(500000);

            struct tree_node_t* huffman_tree = readHuffmanTreeFromFile(output);
            uint64_t total_chars;
            if (fread(&total_chars, sizeof(uint64_t), 1, output) != 1) {
                perror("\nОшибка чтения общего количества символов");
                free(filename);
                freeHuffmanTree(huffman_tree);
                fclose(decoded_output);
                fclose(output);
                return 1;
            }

            printf("\r   Декодирование символов             ");
            fflush(stdout);
            usleep(500000);

            if (total_chars == 1 || (huffman_tree->left == NULL && huffman_tree->right == NULL)) {
                fwrite(&huffman_tree->sym, 1, 1, decoded_output);
                for (uint64_t j = 1; j < total_chars; j++) {
                    fwrite(&huffman_tree->sym, 1, 1, decoded_output);
                }
                uint8_t dummy;
                fread(&dummy, 1, 1, output);
            } else {
                DecodingSym(output, decoded_output, huffman_tree, total_chars);
            }

            printf("\r");

            snprintf(buffer, sizeof(buffer), "Файл %s успешно декодирован", filename);
            print_line(buffer, width);
            if (i != num_files - 1) {
                print_line("", width);
            }

            freeHuffmanTree(huffman_tree);
            free(filename);
            fclose(decoded_output);
        }
        print_border(width, 0);
    }

    fclose(output);
    return 0;
}
