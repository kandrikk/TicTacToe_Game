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
void backlight(int y, int x, char new_simbol, char back);

bool move(tictac *tic);

void menu(tictac *tic);

void robot_move(tictac *tic);
int robot_choos(tictac tic);
int find_winning_move(tictac tic, char simbol);

void put_simbol(tictac *tic);
void move_up(tictac *tic);
void move_down(tictac *tic);
void move_left(tictac *tic);
void move_right(tictac *tic);

void evaluate_game_state(tictac *tic);
void display_game_result(int mode, char simbol);
bool check_win(std::vector<char> field, char simbol);

int select_color(char simbol);
char switch_simbol(tictac tic);
bool free_position(tictac tic);

void initialize_tictac(tictac *tic);
void init_ncurses();