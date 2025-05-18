#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Для sleep (если нужно замедление)

// Функция для очистки строки в терминале
void clearLine() {
    printf("\r%*s\r", 80, ""); // Перемещаем курсор в начало строки и затираем ее пробелами
    fflush(stdout); // Важно для немедленного обновления
}

// Пример добавления и удаления строк
void testLineManipulation() {
    printf("Первая строка\n");
    printf("Вторая строка");

    sleep(2); // Задержка для демонстрации

    clearLine();
    printf("Новая первая строка\n");

    sleep(2);

    clearLine();
    printf("Еще одна строка\n");

    printf("Последняя строка\n");
}

// Пример полосы загрузки
void testProgressBar() {
    int total = 100;
    int width = 50; // Ширина полосы
    for (int i = 0; i <= total; ++i) {
        clearLine();
        printf("Прогресс: [");
        int pos = width * i / total;
        for (int j = 0; j < width; ++j) {
            if (j < pos) printf("=");
            else if (j == pos) printf(">");
            else printf(" ");
        }
        printf("] %d%%\r", i);
        fflush(stdout);
        usleep(20000); // Задержка для визуализации (20 миллисекунд)
    }
    printf("\nЗавершено!\n");
}

int main() {
    printf("Демонстрация работы с терминалом:\n\n");

    printf("--- Манипуляция строками ---\n");
    testLineManipulation();

    printf("\n--- Полоса загрузки ---\n");
    testProgressBar();

    return 0;
}