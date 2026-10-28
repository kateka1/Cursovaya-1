#pragma once
#ifndef COMMON_H
#define COMMON_H

#define MAX 30

extern int rows, cols, mines;
extern int board[MAX][MAX];
extern int revealed[MAX][MAX];
extern int flagged[MAX][MAX];
extern int opened_cells;
extern int game_over;
extern int game_active;

#endif