#define _CRT_SECURE_NO_WARNINGS
#include "records.h"
#include <stdio.h>
#include <conio.h>

void show_highscores(void) {
    system("cls"); 
    printf("=== ТАБЛИЦА РЕКОРДОВ ===\n\n");
    FILE* f = fopen("highscores.txt", "r");
    if (f == NULL) {
        printf("Рекордов пока нет. Будь первым!\n");
    }
    else {
        char line[100];
        while (fgets(line, sizeof(line), f)) {
            printf("%s", line);
        }
        fclose(f);
    }
    printf("\nНажмите любую клавишу, чтобы вернуться в меню...");
    _getch();
}

void save_highscore(const char* name, int score) {
    FILE* f = fopen("highscores.txt", "a");
    if (f != NULL) {
        fprintf(f, "Игрок: %s | Очки: %d\n", name, score);
        fclose(f);
    }
}