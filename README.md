
---

# Архиватор Хаффмана

Проект предназначен для сжатия файлов с однобайтовой кодировкой с использованием алгоритма Хаффмана.

## Сборка

Сборка проекта осуществляется с помощью команды `make` (в каталоге проекта). В результате будет создан исполняемый файл `huffman`.

## Режимы работы

Программа поддерживает два режима:

1. **Кодирование**:

   ```bash
   ./huffman -en -f выходной_файл [файл1 файл2 ...] 
   ```

   Сжимает указанные файлы. Результаты сохраняются в файлы вида `encodefile(...).txt`.

2. **Декодирование**:

   ```bash
   ./huffman -de -f выходной_файл
   ```

   Восстанавливает исходные данные из сжатых файлов. Результаты сохраняются в файлы вида `decodefile(...).txt`.

В программу можно передовать несколько файлов на вход для кодирования. Так же в программу можно передать команду --help, которая выдаст краткое руководство по пользованию.

## Использование

```bash
./huffman [-en | -de] -f выходной_файл [файл1 файл2 ...] 
```

## Содержимое проекта

* **Makefile** — содержит команды для сборки проекта.
* **algofhuffman.c / algofhuffman.h** — реализация алгоритма Хаффмана.
* **heap.c / heap.h** — содержит реализацию минимальной кучи.
* **write.c / write.h** — содержит функции для кодирования символов в файл.
* **read.c / read.h** — реализация функций для чтения.
* **out.c / out.h** —  вспомогательные функции для вывода.

## Формат сжатого файла

В начале записано количество сжатых файлов, после чего для каждого файла отдельно записано в 1 байте кол-во символов в названии файла и само название.
После идет в 4 байтах кол-во символо и дерево хафмена записаное по такому принципу:

* Если вершина — лист, сначала записывается байт со значением `1`, затем байт с символом.
* Если вершина — промежуточная, записывается байт со значением `0`.

После дерева идут закодированные символы в виде "склеенных" битовых последовательностей. Последний байт может быть дополнен нулями.

---
