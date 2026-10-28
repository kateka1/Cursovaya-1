#define _CRT_SECURE_NO_WARNINGS
#include "engine.h"
#include "common.h"
#include "ui.h"
#include "records.h"
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>

// Выделяем память
int rows, cols, mines;
int board[MAX][MAX];
int revealed[MAX][MAX];
int flagged[MAX][MAX];
int opened_cells;
int game_over;
int game_active = 0;

void init_game(int difficulty) {
    switch (difficulty) {
    case 1: rows = 9;  cols = 9;  mines = 10; break;
    case 2: rows = 16; cols = 16; mines = 40; break;
    case 3: rows = 22; cols = 22; mines = 99; break;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = 0; revealed[i][j] = 0; flagged[i][j] = 0;
        }
    }
    opened_cells = 0;
    game_over = 0;
    game_active = 1;

    srand((unsigned)time(NULL));
    int placed = 0;
    while (placed < mines) {
        int r = rand() % rows;
        int c = rand() % cols;
        if (board[r][c] != -1) {
            board[r][c] = -1;
            placed++;
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j] == -1) continue;
            int count = 0;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    int ni = i + di; int nj = j + dj;
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && board[ni][nj] == -1) count++;
                }
            }
            board[i][j] = count;
        }
    }
}

void reveal(int r, int c) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return;
    if (revealed[r][c] || flagged[r][c]) return;
    revealed[r][c] = 1;
    opened_cells++;
    if (board[r][c] == 0) {
        for (int di = -1; di <= 1; di++)
            for (int dj = -1; dj <= 1; dj++)
                reveal(r + di, c + dj);
    }
}

void play_game(void) {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    clear_screen();
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    mode &= ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(hStdin, mode | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    FlushConsoleInputBuffer(hStdin);
    draw_board();

    while (!game_over) {
        if (GetAsyncKeyState('P') & 0x8000) {
            return;
        }

        DWORD nEvents;
        GetNumberOfConsoleInputEvents(hStdin, &nEvents);
        if (nEvents == 0) continue;

        INPUT_RECORD ir;
        DWORD nread;
        ReadConsoleInput(hStdin, &ir, 1, &nread);

        if (ir.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD mer = ir.Event.MouseEvent;
            if (mer.dwEventFlags == 0) {
                int col = (mer.dwMousePosition.X - 4) / 2;
                int row = mer.dwMousePosition.Y - 1;

                if (row >= 0 && row < rows && col >= 0 && col < cols) {
                    if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                        if (!flagged[row][col]) {
                            if (board[row][col] == -1) game_over = -1;
                            else {
                                reveal(row, col);
                                if (opened_cells == rows * cols - mines) game_over = 1;
                            }
                        }
                    }
                    else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
                        if (!revealed[row][col]) flagged[row][col] = !flagged[row][col];
                    }
                    draw_board();
                }
            }
        }
    }

    game_active = 0;
    if (game_over == 1) {
        printf("\nПОБЕДА! Имя: ");
        char name[50]; scanf("%s", name);
        save_highscore(name, opened_cells);
    }
    else {
        draw_board();
        printf("\nБУМ! Вы проиграли. Нажмите любую клавишу...");
        _getch();
    }
}