#include "ui.h"
#include "common.h"
#include <windows.h>
#include <stdio.h>

int is_mouse_button_pressed_asm(int button) {
    int result = 0;
    __asm {
        mov eax, button
        push eax
        call GetAsyncKeyState
        mov result, eax
    }
    return result;
}

void gotoxy(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clear_screen(void) {
    system("cls");
}

void draw_board(void) {
    gotoxy(0, 0);
    printf("    ");
    for (int j = 0; j < cols; j++) printf("%2d", j);
    printf("\n");

    for (int i = 0; i < rows; i++) {
        printf("%2d ", i);
        for (int j = 0; j < cols; j++) {
            if (game_over == -1 && board[i][j] == -1) printf("* ");
            else if (flagged[i][j]) printf("F ");
            else if (!revealed[i][j]) printf(". ");
            else if (board[i][j] == 0) printf("  ");
            else printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\nОткрыто: %d/%d | [P] - Пауза/Меню\n", opened_cells, (rows * cols - mines));
}