#ifndef TETRIS_H
#define TETRIS_H

void CursorView();
void gotoxy(int x, int y);
void init_game();
void draw_board();
void draw_block();
void make_random_block();
void delete_pre_block();
int can_move(int x, int y);
void drop_block();
void cumulate_block();
void input_command();
void remove_line_and_get_score();
int is_finished();
void print_menu();

#endif
