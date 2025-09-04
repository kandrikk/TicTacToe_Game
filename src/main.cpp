#include "tic_tac_toe.h"

int main() {
    init_ncurses();

    game();

    endwin();
    return 0;
}