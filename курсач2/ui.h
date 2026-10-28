#pragma once
#ifndef UI_H
#define UI_H

int is_mouse_button_pressed_asm(int button);
void gotoxy(int x, int y);
void clear_screen(void);
void draw_board(void);

#endif