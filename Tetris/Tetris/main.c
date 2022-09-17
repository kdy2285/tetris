#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

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
int x = 8; 
int y = 0;
int g_form;
int g_rotation = 0;
int tmpX;
int tmpY;

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
				switch (g_board[i][j])
				{
				case EMPTY:
					gotoxy(j * 2, i);
					printf("  ");
					break;
				case WALL:
					gotoxy(j * 2, i);
					printf("в╦");
					break;
				case BLOCK:
					gotoxy(j * 2, i);
					printf("бр");
					break;
				}
				g_buffer[i][j] = g_board[i][j];
			
		}
	}
}

void draw_block()
{
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (g_block[g_form][g_rotation][i][j] == BLOCK) {
				gotoxy(x + j * 2, y + i);
				printf("бс");
			}
		}
	}
}

int can_move(int x, int y)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (g_block[g_form][g_rotation][i][j] == BLOCK) {
				int flag = g_board[y + i][j + x / 2];
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
			y++;
			start = clock();
		}
	}	
}

void save_block()
{
	if (can_move(x, y + 1) == FALSE) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (g_block[g_form][g_rotation][i][j] == BLOCK) {
					g_board[y + i][j + x / 2] = BLOCK;
				}
			}
		}
		x = 8;
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
				if (can_move(x - 2, y)) {
					x -= 2;
				}
				break;
			case RIGHT:
				if (can_move(x + 2, y)) {
					x += 2;
				}
				break;
			case DOWN:
				if (can_move(x, y + 1)) {
					y++;
				}
				break;
			case UP:
				g_rotation++;
				g_rotation %= 4;
				if (can_move(x, y) == FALSE) {
					g_rotation += 3;
					g_rotation %= 4;
				}
				break;
		}
	}
}

void get_score() 
{
	for (int i = ROW - 2; i > 0; i--) {
		int count = 0;
		for (int j = 1; j < COL - 1; j++) {
			if (g_board[i][j] == BLOCK) {
				count++;
			}	
		}
		if (count >= COL - 2) {
			for (int k = i; k > 1; k--) {
				for (int x = 1; x < COL; x++) {
					
					g_board[k][x] = g_board[k - 1][x];
					
				}
			}
			
		}
	}
}



int main()
{
	CursorView();
	init_game();
	g_form = rand() % 7;

	while (1) {
		draw_board();
		draw_block();
		drop_block();
		save_block();
		input_command();
		get_score();
	}
	
	
	_getch();
	return 0;
}