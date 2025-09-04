#include "tic_tac_toe.h"

void game() {
    tictac tic;
    initialize_tictac(&tic);
    menu(&tic);
    draw_field();

    while (!tic.stop) {

        if (move(&tic)) {
            put_simbol(&tic);
            check_win(&tic);
            tic.simbol = switch_simbol(tic);
            robot_move(&tic);
        }
    }
}

void menu(tictac *tic) {
    bool series = 1;

    mvprintw(2, 2, "Play to freinds - 1:");
    mvprintw(3, 2, "Play to bot - 2:");

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
        }
    }
}

void draw_field() {
    int y = 2;
    int x = 2;
    
    clear();
    mvprintw(y++, x, "   |   |   ");
    mvprintw(y++, x, "---|---|---");
    mvprintw(y++, x, "   |   |   ");
    mvprintw(y++, x, "---|---|---");
    mvprintw(y++, x, "   |   |   ");
    refresh();

    attron(COLOR_PAIR(2));
    mvprintw(1, 2, "Move: x");
    attroff(COLOR_PAIR(2));
}

void update_field(tictac tic) {
    int color = select_color(tic.simbol);
    char back_simbol = switch_simbol(tic);
    int color2 = select_color(back_simbol);

    int y = tic.coordinates[tic.position - 1].first;
    int x = tic.coordinates[tic.position - 1].second;

    attron(COLOR_PAIR(color));
    mvprintw(y, x, "%c", tic.simbol);
    attroff(COLOR_PAIR(color));

    attron(COLOR_PAIR(color2));
    mvprintw(1, 2, "Move: %c", back_simbol);
    attroff(COLOR_PAIR(color2));
}

void backlight(tictac tic) {
    char back = tic.field[tic.position - 1];

    int y = tic.coordinates[tic.position - 1].first;
    int x = tic.coordinates[tic.position - 1].second;

    attron(COLOR_PAIR(4));
    mvprintw(y, x, " ");
    attroff(COLOR_PAIR(4));
    refresh();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    int color = select_color(back);
    attron(COLOR_PAIR(color));
    mvprintw(y, x, "%c", back);
    attroff(COLOR_PAIR(color));
    refresh();
}

void check_win(tictac *tic) {
    update_field(*tic);

    for (int i = 0; i < 3; ++i) {
        if ((is_winner(*tic, i))) {
            tic->stop = true;
            refresh();
            SLEEP;
        }
    }
}

bool is_winner(tictac tic, int id) {
    bool res = false;

    int color = select_color(tic.simbol);

    if (if_horizontal(tic, id) || if_vertical(tic, id) || if_diagonal(tic)) {
        attron(COLOR_PAIR(color));
        mvprintw(1, 2, "Winner: %c", tic.simbol - 32);
        attroff(COLOR_PAIR(color));
        res =  true;
    }

    if (tic.number_puts == 9 && !res) {
        attron(COLOR_PAIR(1));
        mvprintw(1, 2, "Draw    ");
        attroff(COLOR_PAIR(1));
        res = true;
    }

    return res;
}

void put_simbol(tictac *tic) {
    tic->field[tic->position - 1] = tic->simbol;

    tic->number_puts++;

    update_field(*tic);
}

void robot_move(tictac *tic) {
    int back_pos = tic->position;
    if (!tic->robot || tic->stop) return;

    int pos = robot_choos(*tic);
    tic->position = pos;

    put_simbol(tic);
    check_win(tic);
    update_field(*tic);

    tic->simbol = switch_simbol(*tic);
    tic->position = back_pos;
}

int robot_choos(tictac tic) {
    int i;

    do {
        i = rand() % 9;
    } while (tic.field[i] != ' ');

    return i + 1;
}

bool move(tictac *tic) {
    int res = false;
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
        if (check_position(*tic)) res = true;
        break;

    case 'Q':
    case 'q':
        tic->stop = true;
        break;
    }

    backlight(*tic);
    
    return res;
}

bool check_position(tictac tic) {
    if (tic.field[tic.position - 1] == 'x' 
    || tic.field[tic.position - 1] == 'o') return false;

    return true;
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

bool if_horizontal(tictac tic, int id) {
    if (tic.field[id * 3] == tic.simbol &&
         tic.field[id * 3 + 1] == tic.simbol &&
          tic.field[id * 3 + 2] == tic.simbol) return true;

    return false;
}

bool if_vertical(tictac tic, int id) {
    if (tic.field[id] == tic.simbol &&
         tic.field[id + 3] == tic.simbol &&
          tic.field[id + 6] == tic.simbol) return true;

    return false;
}

bool if_diagonal(tictac tic) {
    if ((tic.field[0] == tic.simbol && tic.field[4] == tic.simbol && tic.field[8] == tic.simbol) || 
        (tic.field[6] == tic.simbol && tic.field[4] == tic.simbol && tic.field[2] == tic.simbol)) return true;
    
    return false;
}

int select_color(char simbol) {
    int color;
    switch (simbol) {
    case 'o':
        color = 1;
        break;

    case 'x':
        color = 2;
        break;

    default:
        color = 3;
        break;
    }

    return color;
}

char switch_simbol(tictac tic) {
    char res = tic.simbol;
    if (!tic.stop) {
        res = (res == 'x') ? 'o' : 'x';
    }

    return res;
}

