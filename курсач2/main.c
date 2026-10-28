#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

// Подключаем модули
#include "common.h"
#include "ui.h"
#include "engine.h"
#include "records.h"

int main(void) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    int menu_choice;
    while (1) {
        clear_screen();
        printf("=== ГЛАВНОЕ МЕНЮ ===\n");
        if (game_active) printf("0. Возобновить игру\n");
        printf("1. Начать новую игру\n");
        printf("2. Результаты игр\n");
        printf("3. Выход\n");
        printf("Ваш выбор: ");

        if (scanf("%d", &menu_choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (menu_choice == 3) break;
        if (menu_choice == 2) { show_highscores(); continue; }
        if (menu_choice == 0 && game_active) { play_game(); continue; }

        if (menu_choice == 1) {
            int diff;
            do {
                clear_screen();
                printf("Выберите сложность (1 - Легко, 2 - Средне, 3 - Сложно): ");
                scanf("%d", &diff);
            } while (diff < 1 || diff > 3);

            while (getchar() != '\n');
            fflush(stdin);
            FlushConsoleInputBuffer(hStdin);

            init_game(diff);
            play_game();
        }
    }

    return 0;
}