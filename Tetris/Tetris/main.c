#include "tetris.h"

extern int g_form;

int main()
{
	CursorView();
	init_game();
	print_menu();
	g_form = rand() % 7;

	while (1) {
		draw_board();
		draw_block();
		drop_block();
		cumulate_block();
		input_command();
		remove_line_and_get_score();
		if (is_finished()) {
			gotoxy(30, 20);
			printf("���� ����\n");
			break;
		}
	}
	
	_getch();
	return 0;
}