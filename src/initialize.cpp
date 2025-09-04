#include "tic_tac_toe.h"

void init_ncurses() {
    initscr();
    curs_set(0);

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_WHITE);
    
    keypad(stdscr, TRUE);
    noecho();
}

void initialize_tictac(tictac *tic) {
    int w = 0;
    while (w != 10) {
        tic->field.push_back(' ');
        w++;
    }

    tic->position = 5;

    tic->robot = false;

    tic->coordinates.push_back(std::make_pair(2, 3));
    tic->coordinates.push_back(std::make_pair(2, 7));
    tic->coordinates.push_back(std::make_pair(2, 11));
    tic->coordinates.push_back(std::make_pair(4, 3));
    tic->coordinates.push_back(std::make_pair(4, 7));
    tic->coordinates.push_back(std::make_pair(4, 11));
    tic->coordinates.push_back(std::make_pair(6, 3));
    tic->coordinates.push_back(std::make_pair(6, 7));
    tic->coordinates.push_back(std::make_pair(6, 11));

    tic->data_move = 0;
    tic->move = 'x';
}