#include "tic_tac_toe.h"

void tic_tac_toe() {
    tictac tic;
    int y;
    initialize_tictac(&tic);
    menu(&tic);
    draw_field(tic);

    while (true) {
        mvprintw(15, 15, "%d", tic.position);
        mvprintw(16, 15, "%d", tic.data_move);

        int color = (tic.move == 'x') ? 2 : 1;
        attron(COLOR_PAIR(color));
        mvprintw(1, 2, "Move: %c", tic.move);
        attroff(COLOR_PAIR(color));

        backlight(tic);
        
        y = control(&tic);
        if (y == 1) {
            update_field(tic);
            if (is_winner(tic)) break;
            if (tic.robot) move_robot(&tic);
            if (is_winner(tic)) break;
        } else if (y == -1) {
            break;
        }
    }
}

void menu(tictac *tic) {
    mvprintw(2, 2, "Play to freinds - 1: \n  Play to bot - 2: ");
    bool series = 1;
    while (series) {
    char ch = getch();
    
        switch (ch) {
        case '1':
            series = false;
            break;

        case '2':
            tic->robot = true;
            series = false;
            break;

        default:
            break;
        }
    }
}

void draw_field(tictac tic) {
    int y = 2;
    int x = 2;
    tic.position = 5;
    
    clear();
    mvprintw(y++, x, "   |   |  ");
    mvprintw(y++, x, "---|---|---");
    mvprintw(y++, x, "   |   |  ");
    mvprintw(y++, x, "---|---|---");
    mvprintw(y++, x, "   |   |  ");
    refresh();
}

void update_field(tictac tic) {
    int simbol = (tic.move == 'x') ? 'o' : 'x';
    int color = (simbol == 'x') ? 2 : 1;

    int y = tic.coordinates[tic.position - 1].first;
    int x = tic.coordinates[tic.position - 1].second;

    attron(COLOR_PAIR(color));
    mvprintw(y, x, "%c", simbol);
    mvprintw(1, 2, "Move: %c", tic.move);
    attroff(COLOR_PAIR(color));
}

void backlight(tictac tic) {
    char back = tic.field[tic.position - 1];

    int y = tic.coordinates[tic.position - 1].first;
    int x = tic.coordinates[tic.position - 1].second;

    int color;
    if (back == 'x') {color = 2;} 
    else if (back == 'o') {color = 1;}
    else {color = 3;}

    attron(COLOR_PAIR(4));
    mvprintw(y, x, " ");
    attroff(COLOR_PAIR(4));
    refresh();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    attron(COLOR_PAIR(color));
    mvprintw(y, x, "%c", back);
    attroff(COLOR_PAIR(color));
    refresh();
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

    int simbol = (tic.move == 'x') ? 'o' : 'x';
    int color = (simbol == 'x') ? 2 : 1;

    if (if_horizontal(tic, id, mv) || if_vertical(tic, id, mv) || if_diagonal(tic, mv)) {
        attron(COLOR_PAIR(color));
        mvprintw(10, 10, "Winner: %c", mv - 32);
        attroff(COLOR_PAIR(color));
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

int control(tictac *tic) {
    int res = 0;
    int key = getch();

    switch (key) {
    case KEY_UP:
        move_up(tic);
        break;

    case KEY_DOWN:
        move_down(tic);
        break;

    case KEY_LEFT:
        move_left(tic);
        break;

    case KEY_RIGHT:
        move_right(tic);
        break;

    case ' ':
        move(tic);
        res = 1;
        break;

    case 'Q':
    case 'q':
        res = -1;
        break;
    }
    
    return res;
}

void move(tictac *tic) {
    if (tic->field[tic->position - 1] == 'x' 
        || tic->field[tic->position - 1] == 'o') return;
    tic->field[tic->position - 1] = tic->move;

    tic->move = (tic->move == 'x') ? 'o' : 'x';

    tic->data_move++;
}

void move_robot(tictac *tic) {
    while (true) {
    std::srand(time(nullptr));
    int i = rand() % 9 + 1;

        if (tic->field[i - 1] == ' ') {
            tic->position = i;
            move(tic);
            ++i;
            break;
        }
    }
    update_field(*tic);
}

void move_up(tictac *tic) {
    if (tic->position > 3) tic->position -= 3;
}

void move_down(tictac *tic) {
    if (tic->position < 7) tic->position += 3;
}

void move_left(tictac *tic) {
    int n = tic->position;
    if ((n != 1) && (n != 4) && (n != 7)) tic->position -= 1;
}

void move_right(tictac *tic) {
    int n = tic->position;
    if ((n != 3) && (n != 6) && (n != 9)) tic->position += 1;
}