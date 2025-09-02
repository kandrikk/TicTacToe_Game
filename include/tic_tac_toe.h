#include <iostream>
#include <vector>
#include <ncurses.h>
#include <chrono>
#include <thread>
#include <utility>

#define SLEEP std::this_thread::sleep_for(std::chrono::seconds(2));

typedef struct {
    char move;
    int data_move;
    int position;
    std::vector<char> field;
    std::vector<std::pair<int, int>> coordinates;
} tictac;

void tic_tac_toe();
void draw_field(tictac board);
void initialize_tictac(tictac *tic);
bool control(tictac *tic);
void move(tictac *tic);
bool is_winner(tictac tic);
bool check_win(tictac tic, int id, char mv);

bool if_horizontal(tictac tic, int id, char mv);
bool if_vertical(tictac tic, int id, char mv);
bool if_diagonal(tictac tic, char mv);
void update_field(tictac tic);

void init_ncurses();