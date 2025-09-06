#include "tic_tac_toe.h"

void game() {
    tictac tic;
    initialize_tictac(&tic);
    menu(&tic);
    draw_field();

    while (!tic.stop) {
        char back = tic.field[tic.position - 1];
        int y = tic.coordinates[tic.position - 1].first;
        int x = tic.coordinates[tic.position - 1].second;
        backlight(y, x, ' ', back);

        if (move(&tic)) {
            put_simbol(&tic);
            update_field(tic);
            evaluate_game_state(&tic);
            tic.simbol = switch_simbol(tic);
            robot_move(&tic);
        } 
    }
}

void menu(tictac *tic) {
    bool series = 1;
    int res = 1;
    int y = 2;
    int x = 19;
    char back = ' ';

    mvprintw(2, 2, "Play with freinds");
    mvprintw(3, 2, "Play with bot");

    while (series) {

    int key = getch();
    switch (key) {
        case KEY_UP:
            y = 2;
            x = 19;
            break;

        case KEY_DOWN:
            y = 3;
            x = 15;
            break;

        case ' ':
            res = (y == 2) ? 1 : 2;
            series = false;
            break;

        case 'q':
            tic->stop = true;
            series = false;
            break;
        }

        backlight(y, x, ' ', back);
    }

    if (res == 2) tic->robot = true;
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

void backlight(int y, int x, char new_simbol, char back) {
    attron(COLOR_PAIR(4));
    mvprintw(y, x, "%c", new_simbol);
    attroff(COLOR_PAIR(4));
    refresh();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    int color = select_color(back);
    attron(COLOR_PAIR(color));
    mvprintw(y, x, "%c", back);
    attroff(COLOR_PAIR(color));
    refresh();
}

void evaluate_game_state(tictac *tic) {
    int mode = 0;

    if (check_win(tic->field, tic->simbol)) {
        mode = 1;
    } else if (tic->number_puts == 9) mode = -1;

    if (mode != 0) {
        display_game_result(mode, tic->simbol);
        tic->stop = true;
        refresh();
        SLEEP;
    }
}

bool check_win(std::vector<char> field, char simbol) {
    bool res = false;

    for (int i = 0; i < 3; ++i) {
        if (field[i * 3] == simbol &&                                               // horizontal
            field[i * 3 + 1] == simbol &&
            field[i * 3 + 2] == simbol) res = true;

        if (field[i] == simbol &&                                                   // vertical
            field[i + 3] == simbol &&
            field[i + 6] == simbol) res = true;
    }

    if ((field[0] == simbol && field[4] == simbol && field[8] == simbol) ||     // diagonal
        (field[6] == simbol && field[4] == simbol && field[2] == simbol)) res = true;

    return res;
}

void display_game_result(int mode, char simbol) {
    int color = select_color(simbol);

    if (mode == 1) {
        attron(COLOR_PAIR(color));
        mvprintw(1, 2, "Winner: %c", simbol - 32);
        attroff(COLOR_PAIR(color));
    }

    if (mode == -1) {
        attron(COLOR_PAIR(1));
        mvprintw(1, 2, "Draw    ");
        attroff(COLOR_PAIR(1));
    }
}

void put_simbol(tictac *tic) {
    tic->field[tic->position - 1] = tic->simbol;

    tic->number_puts++;
}

void robot_move(tictac *tic) {
    if (!tic->robot || tic->stop) return;
    int back_pos = tic->position;

    int pos = robot_choos(*tic);
    tic->position = pos;

    put_simbol(tic);
    update_field(*tic);
    evaluate_game_state(tic);

    tic->simbol = switch_simbol(*tic);
    tic->position = back_pos;
}

int robot_choos(tictac tic) {
    int res;
    int o_win = find_winning_move(tic, 'o');
    int x_win = find_winning_move(tic, 'x');



    if (o_win > 0) {
        res = o_win;
    } else if (x_win > 0) {
        res = x_win;
    } else {
        do {
            res = rand() % 9 + 1;
        } while (tic.field[res - 1] != ' ');
    }

    return res;
}

int find_winning_move(tictac tic, char simbol) {
    int res = -1;
    auto cp_field = tic.field;

    for (int i = 0; i < 9; ++i) {
        if (cp_field[i] == ' ') cp_field[i] = simbol;
        if (check_win(cp_field, simbol)) {
            res = i + 1;
        }
        cp_field = tic.field;
    }

    return res;
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
        if (free_position(*tic)) res = true;
        break;

    case 'Q':
    case 'q':
        tic->stop = true;
        break;
    }

    return res;
}

bool free_position(tictac tic) {
    bool res = true;

    if (tic.field[tic.position - 1] == 'x' 
    || tic.field[tic.position - 1] == 'o') res = false;

    return res;
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

