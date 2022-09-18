#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include "tetris.h"

#define ROW (22)
#define COL (11)
#define TRUE (1)
#define FALSE (0)
#define LEFT (75)
#define RIGHT (77)
#define DOWN (80)
#define UP (72)
#define SPACE (32)
#define TIME (500) 

int g_buffer[ROW][COL];
int g_board[ROW][COL];
int x = 4;
int y = 0;
int g_form;
int g_rotation;
int g_score;

clock_t start;
clock_t end;

int g_block[7][4][4][4] = {
	{
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{0,1,1,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,1,0,0}
		}
	},
	{
		{
			{0,0,0,0},
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,1,0,0}
		}
	},
	{
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{1,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{1,0,0,0}
		}
	},
	{
		{
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0}
		}
	},
	{
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{1,0,0,0},
			{1,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,0,0,0}
		}
	},
	{
		{
			{0,0,0,0},
			{0,0,1,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{1,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0},
			{1,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		}
	},
	{
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		}
	}
};

typedef enum EBlocks {
	EMPTY,
	BLOCK,
	MOVING_BLOCK,
	WALL
};

void CursorView()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.bVisible = 0;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}


void gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void init_game()
{
	g_rotation = 0;
	g_score = 0;

	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			g_board[i][j] = EMPTY;
		}
	}

	for (int i = 0; i < ROW; i++) {
		g_board[i][0] = WALL;
		g_board[i][COL - 1] = WALL;
	}

	for (int j = 0; j < COL; j++) {
		g_board[0][j] = WALL;
		g_board[ROW - 1][j] = WALL;
	}
}

void draw_board()
{
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (g_buffer[i][j] != g_board[i][j]) {
				gotoxy(j * 2, i);

				switch (g_board[i][j])
				{
				case EMPTY:
					printf("  ");
					break;
				case WALL:
					printf("��");
					break;
				case BLOCK:
					printf("��");
					break;
				case MOVING_BLOCK:
					printf("��");
					break;
				}
				g_buffer[i][j] = g_board[i][j];
			}
		}
	}
	gotoxy(35, 3);
	printf("SCORE : %d", g_score);
}

void draw_block()
{

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (g_block[g_form][g_rotation][i][j] == BLOCK) {
				g_board[y + i][x + j] = MOVING_BLOCK;
			}
		}
	}

	for (int j = 0; j < COL; j++) {
		g_board[0][j] = WALL;

	}

}

void delete_pre_block()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (g_block[g_form][g_rotation][i][j] == BLOCK) {
				g_board[y + i][x + j] = EMPTY;
			}
		}
	}
}

int can_move(int x, int y)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (g_block[g_form][g_rotation][i][j] == BLOCK) {
				int flag = g_board[y + i][j + x];
				if (flag == BLOCK || flag == WALL) {
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

void drop_block()
{
	end = clock();
	if ((float)end - start > 1000) {
		if (can_move(x, y + 1)) {

			delete_pre_block();

			y++;
			start = clock();
		}
	}

}

void cumulate_block()
{
	if (can_move(x, y + 1) == FALSE) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (g_block[g_form][g_rotation][i][j] == BLOCK) {
					g_board[y + i][j + x] = BLOCK;
				}
			}
		}
		x = 4;
		y = 0;
		g_form = rand() % 7;
	}
}

void input_command()
{
	if (_kbhit()) {
		int key = _getch();
		switch (key)
		{
		case LEFT:
			if (can_move(x - 1, y)) {
				delete_pre_block();
				x--;
			}
			break;
		case RIGHT:
			if (can_move(x + 1, y)) {
				delete_pre_block();
				x++;
			}
			break;
		case DOWN:
			if (can_move(x, y + 1)) {
				delete_pre_block();
				y++;
			}
			break;
		case UP:
			delete_pre_block();
			g_rotation++;
			g_rotation %= 4;
			if (can_move(x, y) == FALSE) {
				g_rotation += 3;
				g_rotation %= 4;
				draw_block();
			}
			break;
		case SPACE:
			delete_pre_block();
			while (can_move(x, y) == TRUE) {
				if (can_move(x, y + 1)) {
					y++;
				}
				else {
					break;
				}
			}
			cumulate_block();
			break;
		}
	}
}

void remove_line_and_get_score()
{
	for (int i = ROW - 2; i > 0; i--) {
		int count = 0;
		for (int j = 1; j < COL - 1; j++) {
			if (g_board[i][j] == BLOCK) {
				count++;
			}
		}
		if (count >= COL - 2) {
			g_score += 10;
			for (int k = i; k > 1; k--) {
				for (int x = 1; x < COL; x++) {
					g_board[k][x] = g_board[k - 1][x];

				}
			}
		}
	}
}

int is_finished()
{
	for (int i = 0; i < COL; i++) {
		if (g_board[0][i] == BLOCK) {
			return TRUE;
		}
	}
	return FALSE;
}

void print_menu()
{
	gotoxy(55, 5);
	printf("----------- CONTROL KEY -----------");
	gotoxy(55, 7);
	printf("�� : MOVE RIGHT");
	gotoxy(55, 9);
	printf("�� : MOVE LEFT");
	gotoxy(55, 11);
	printf("�� : MOVE DOWN");
	gotoxy(55, 13);
	printf("�� : ROTATE BLOCK");
	gotoxy(55, 15);
	printf("SPACE_BAR : GOTO BOTTOM");
}
