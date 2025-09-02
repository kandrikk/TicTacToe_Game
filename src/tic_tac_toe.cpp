#include "tic_tac_toe.h"

void tic_tac_toe() {
    tictac tic;
    initialize_tictac(&tic);
    draw_field(tic);


    while (true) {
        if (!control(&tic)) break;
        update_field(tic);
        if (is_winner(tic)) break;
    }
}

void backlight(tictac tic) {
    int y = tic.coordinates[tic.position - '1'].first;
    int x = tic.coordinates[tic.position - '1'].second;

    attron(COLOR_PAIR(0));
    mvprintw(y, x, " ");
    attroff(COLOR_PAIR(0));
}

void update_field(tictac tic) {
    int color = (tic.move == 'x') ? 2 : 1;
    int sim = tic.field[tic.position - '1'];

    int y = tic.coordinates[tic.position - '1'].first;
    int x = tic.coordinates[tic.position - '1'].second;

    attron(COLOR_PAIR(color));
    mvprintw(y, x, "%c", sim);
    attroff(COLOR_PAIR(color));
}

bool is_winner(tictac tic) {
    update_field(tic);
    
    char mv = (tic.move == 'x') ? 'o' : 'x';

    for (int i = 0; i < 3; ++i) {
        if ((check_win(tic, i, mv))) return true;
    }

    return false;
}

bool check_win(tictac tic, int id, char mv) {
    bool res = false;

    if (if_horizontal(tic, id, mv) || if_vertical(tic, id, mv) || if_diagonal(tic, mv)) {
        attron(COLOR_PAIR(2));
        mvprintw(10, 10, "Winner: %c", mv - 32);
        attroff(COLOR_PAIR(2));
        res =  true;
    }

    if (tic.data_move == 9 && !res) {
        attron(COLOR_PAIR(1));
        mvprintw(10, 10, "Draw");
        attroff(COLOR_PAIR(1));
        res = true;
    }

    if (res) {
        refresh();
        SLEEP;
    }

    return res;
}

bool if_horizontal(tictac tic, int id, char mv) {
    if (tic.field[id * 3] == mv && tic.field[id * 3 + 1] == mv && tic.field[id * 3 + 2] == mv) return true;

    return false;
}

bool if_vertical(tictac tic, int id, char mv) {
    if (tic.field[id] == mv && tic.field[id + 3] == mv && tic.field[id + 6] == mv) return true;

    return false;
}

bool if_diagonal(tictac tic, char mv) {
    if ((tic.field[0] == mv && tic.field[4] == mv && tic.field[8] == mv) || 
        (tic.field[6] == mv && tic.field[4] == mv && tic.field[2] == mv)) return true;
    
    return false;
}

void draw_field(tictac tic) {
    int y = 2;
    int x = 2;
    
    clear();
    mvprintw(y++, x, " %c | %c | %c ", tic.field[0], tic.field[1], tic.field[2]);
    mvprintw(y++, x, "---|---|---");
    mvprintw(y++, x, " %c | %c | %c ", tic.field[3], tic.field[4], tic.field[5]);
    mvprintw(y++, x, "---|---|---");
    mvprintw(y++, x, " %c | %c | %c ", tic.field[6], tic.field[7], tic.field[8]);
    refresh();
}

void move(tictac *tic) {
    if (tic->data_move >= 9) return;

    if (tic->field[tic->position - '1'] != tic->position) return;

    tic->field[tic->position - '1'] = tic->move;

    tic->move = (tic->move == 'x') ? 'o' : 'x';

    tic->data_move++;
}

bool control(tictac *tic) {
    bool res = true;
    int ch = getch();

    if ((ch > '9' || ch < '1') && ch != 'q' && ch != 'Q') {
        return res;
    }

    tic->position = ch;

    switch (tic->position) {
    case KEY_UP:
        
        break;

    case KEY_DOWN:

        break;

    case KEY_LEFT:

        break;

    case KEY_RIGHT:

        break;

    case ' ':

        break;

    case 'Q':
    case 'q':
        res = false;
        break;

    default:
        if (tic->position >= '1' && tic->position <= '9') move(tic);

        break;
    }

    return res;
}