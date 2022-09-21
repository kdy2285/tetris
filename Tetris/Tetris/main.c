#include "tetris.h"

int main()
{
	CursorView();
	init_game();
	print_menu();
	make_random_block();

	while (1) {
		draw_board();
		draw_block();
		drop_block();
		cumulate_block();
		input_command();
		remove_line_and_get_score();
		if (is_finished()) {
			gotoxy(30, 20);
			printf("게임 종료\n");
			break;
		}
	}
	
	_getch();
	return 0;
}