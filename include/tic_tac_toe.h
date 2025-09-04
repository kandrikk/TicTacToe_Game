#include <iostream>
#include <vector>
#include <ncurses.h>
#include <chrono>
#include <thread>
#include <utility>

#define SLEEP std::this_thread::sleep_for(std::chrono::seconds(1));

typedef struct {
    bool stop;
    bool robot;
    char simbol;
    int number_puts;
    int position;
    std::vector<char> field;
    std::vector<std::pair<int, int>> coordinates;
} tictac;

void game();
void draw_field();
void update_field(tictac tic);
void backlight(tictac tic);

bool move(tictac *tic);

void menu(tictac *tic);

void robot_move(tictac *tic);
int robot_choos(tictac tic);

void put_simbol(tictac *tic);
void move_up(tictac *tic);
void move_down(tictac *tic);
void move_left(tictac *tic);
void move_right(tictac *tic);

void check_win(tictac *tic);
bool is_winner(tictac tic, int id);

bool if_horizontal(tictac tic, int id);
bool if_vertical(tictac tic, int id);
bool if_diagonal(tictac tic);

int select_color(char simbol);
char switch_simbol(tictac tic);
bool check_position(tictac tic);

void initialize_tictac(tictac *tic);
void init_ncurses();