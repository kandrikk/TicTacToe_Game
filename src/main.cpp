#include "tic_tac_toe.h"

int main() {
    init_ncurses();

    tic_tac_toe();

    endwin();
    return 0;
}