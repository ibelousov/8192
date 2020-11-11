#include "../include/main.h"

int OFFSETS[36][7] = {
    {1 , 1, 1, 1, 1, 1, 1},
    {2 , 1, 1, 1, 1, 1, 1},
    {3 , 1, 1, 2, 1, 1, 1},
    {4 , 1, 2, 2, 1, 1, 1},
    {5 , 1, 2, 3, 1, 1, 2},
    {6 , 1, 2, 3, 1, 1, 2},

    {7 , 2, 3, 4, 1, 2, 2},
    {8 , 2, 3, 4, 1, 2, 2},
    {9 , 2, 3, 5, 1, 2, 3},
    {10, 2, 4, 5, 1, 2, 3},
    {11, 2, 4, 6, 1, 2, 3},
    {12, 2, 4, 6, 1, 2, 3},

    {13, 3, 5, 7, 2, 3, 4},
    {14, 3, 5, 7, 2, 3, 4},
    {15, 3, 5, 8, 2, 3, 4},
    {16, 3, 6, 8, 2, 3, 4},
    {17, 3, 6, 9, 2, 3, 5},
    {18, 3, 6, 9, 2, 3, 5},

    {19, 4, 7, 10,2, 4, 5},
    {20, 4, 7, 10,2, 4, 5},
    {21, 4, 7, 11,2, 4, 6},
    {22, 4, 8, 11,2, 4, 6},
    {23, 4, 8, 12,2, 4, 6},
    {24, 4, 8, 12,2, 4, 6},

    {25, 5, 9, 13,3, 5, 7},
    {26, 5, 9, 13,3, 5, 7},
    {27, 5, 9, 14,3, 5, 7},
    {28, 5, 10,14,3, 5, 7},
    {29, 5, 10,15,3, 5, 8},
    {30, 5, 10,15,3, 5, 8},
    
    {31, 6, 11, 16, 3, 6,8},
    {32, 6, 11, 16, 3, 6,8},
    {33, 6, 11, 17, 3, 6,9},
    {34, 6, 12, 17, 3, 6,9},
    {35, 6, 12, 18, 3, 6,9},
    {36, 6, 12, 18, 3, 6,9},       
};

int FIELD[SIDELEN][SIDELEN] = {
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
};

int scores = 0;

void setXY(int x, int y, int value)
{
    FIELD[y][x] = value;
}

int right_pad(int value)
{
    return (value < 10 || value < 100) ? 2 : 1;
}

int left_pad(int value)
{
    if((value < 10 || value < 100))
        return 2;

    return (value > 100 && value < 1000) ? 2 : 1;
}

int is_point_in_list(int x, int y, linked_list * lst)
{
    while(lst) {
        if(lst->trans.from.x == x && lst->trans.from.y == y) {
            return 1;
        }
        lst = (linked_list *) lst->next;
    }
    return 0;
}

void print_ceil(int x, int y, int value, vector offset)
{
    attron(COLOR_PAIR(value == 0 ? 16 : log2(value*2)));
    mvprintw((LINES/2 - (3*y)-1 ) + offset.vy, COLS/2 - 6 * x + offset.vx, "%s", "      ");
    
    if(value == 0)
        mvprintw(LINES/2 - 3*y + offset.vy, COLS/2 - 6 * x + offset.vx, "%*s .%*s", left_pad(value), " ", right_pad(value), " ");
    else if(value < 10)
        mvprintw(LINES/2 - 3*y + offset.vy, COLS/2 - 6 * x + offset.vx, "%*s%02d%*s", left_pad(value), " ", value, right_pad(value), " ");
    else
        mvprintw(LINES/2 - 3*y + offset.vy, COLS/2 - 6 * x + offset.vx, "%*s%d%*s", left_pad(value), " ", value, right_pad(value), " ");
    mvprintw(LINES/2 - (3*y)+1 + offset.vy, COLS/2 - 6 * x + offset.vx, "%s", "      ");
    
    attroff(COLOR_PAIR(value == 0 ? 16 : value));
    refresh();
}


void print_field_without_moving_points(linked_list * points)
{
    int x, y;
    for(y = 0; y < SIDELEN; y++) {
        for(x = 0; x < SIDELEN; x++) {
            if(!is_point_in_list(x, y, points)) {
                print_ceil(x-1, y-1, FIELD[y][x], (vector){0,0});
            } else {
                print_ceil(x-1, y-1, 0, (vector){0,0});
            }
        }
    }
}

void print_field()
{
    int x, y;
    for(y = 0; y < SIDELEN; y++) {
        for(x = 0; x < SIDELEN; x++) {
            print_ceil(x-1, y-1, FIELD[y][x], (vector){0,0});
        }
    }
    refresh();
}

void print_empty_field()
{
    int x, y;
    for(y = 0; y < SIDELEN; y++) {
        for(x = 0; x < SIDELEN; x++) {
            print_ceil(x-1, y-1, 0, (vector){0,0});
        }
    }
}

void print_field_without_points(linked_list * movements)
{
    int x, y;
    for(y = 0; y < SIDELEN; y++) {
        for(x = 0; x < SIDELEN; x++) {
            linked_list * temp = movements;
            while(temp) {
                if(temp->trans.from.x == x && temp->trans.from.y == y) {
                    break;
                }
                temp = (linked_list *) temp->next;
            }
            if(temp) {
                print_ceil(x-1, y-1,  0, (vector){0,0});
            } else {
                print_ceil(x-1, y-1,  FIELD[y][x], (vector){0,0});
            }
        }
    }
}

int get_move_difference(transition trans)
{
    return (int)sqrt(pow(trans.from.y - trans.to.y, 2) + pow(trans.from.x - trans.to.x, 2));
}

void fill_with_points()
{
    point array[SIDELEN * SIDELEN];
    int numbers[] = {2,2,2,2,4,4,8};

    for(int i = 0; i < SIDELEN; i++) 
        for(int j = 0; j < SIDELEN; j++) 
            array[SIDELEN * i + j] = (point){j,i};
        
    for (size_t i = 0; i < SIDELEN; i++) 
    {
        size_t j = i + rand() / (RAND_MAX / (SIDELEN - i) + 1);
        point t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
    for(int i = 0; i < SIDELEN * SIDELEN; i++) {
        if(FIELD[array[i].y][array[i].x] == 0) {
            FIELD[array[i].y][array[i].x] = numbers[rand() % 7];
            return;
        }
    }
}

void fill_with_points_times(int times)
{
    while(times-- > 0)
        fill_with_points();
}

void make_movement(linked_list * movements, char key)
{
    struct timespec tw = {0,3000000};
    struct timespec tr;
    for(int i = 0; i < 35; i++) {
        linked_list * temp_movements = movements;
        vector offset = {0,0};
        
        bkgdset(COLOR_PAIR(15));
        print_field_without_points(movements);

        while(temp_movements) {
            int ceil_difference = get_move_difference(temp_movements->trans);
            int cell_value = FIELD[temp_movements->trans.from.y][temp_movements->trans.from.x];
            switch(key)
            {
                case 'l':
                    offset = (vector){OFFSETS[i][ceil_difference], 0};
                    break;
                case 'r':
                    offset = (vector){-OFFSETS[i][ceil_difference], 0};
                    break;
                case 'd':
                    offset = (vector){0, -OFFSETS[i][ceil_difference+3]};
                    break;
                case 'u':
                    offset = (vector){0, OFFSETS[i][ceil_difference+3]};
                    break;
            }
            print_ceil(temp_movements->trans.from.x-1, temp_movements->trans.from.y-1, cell_value, offset);
    
            temp_movements = (linked_list *) temp_movements->next;
        }
        refresh();
        nanosleep (&tw, &tr);
    }

    while(movements) {
        if(FIELD[movements->trans.to.y][movements->trans.to.x] > 0 && FIELD[movements->trans.from.y][movements->trans.from.x] > 0)
            scores += FIELD[movements->trans.to.y][movements->trans.to.x] + FIELD[movements->trans.from.y][movements->trans.from.x];
        FIELD[movements->trans.to.y][movements->trans.to.x] += FIELD[movements->trans.from.y][movements->trans.from.x];
        FIELD[movements->trans.from.y][movements->trans.from.x] = 0;
        
        movements = (linked_list *) movements->next;
    }
    bkgdset(COLOR_PAIR(15));
    print_field();
    refresh();    
    nanosleep (&tw, &tr);
}

int check_game_over()
{   
    if(get_all_points(FIELD, 'd') == NULL && 
        get_all_points(FIELD, 'r') == NULL && 
        get_all_points(FIELD, 'l') == NULL && 
        get_all_points(FIELD, 'u') == NULL )
        return 1;
    return 0;
}

void clear_field()
{
    for(int i = 0; i < SIDELEN; i++)
        for(int j = 0; j < SIDELEN; j++)
            FIELD[i][j] = 0;
}

int is_game_win()
{
    for(int i = 0; i < SIDELEN; i++) 
        for(int j = 0; j < SIDELEN; j++)
            if(FIELD[i][j] == GAME_WIN_SCORES)
                return 1;
    return 0;
}

void print_scores()
{
    attron(COLOR_PAIR(15));
    mvprintw(LINES / 2 - 8, COLS/2 + 4, "%08d", scores);
    attroff(COLOR_PAIR(15));
}

void print_game_win()
{
    attron(COLOR_PAIR(17));

    mvprintw(LINES / 2 - 4, COLS/2 - 6, "%s", "             ");
    mvprintw(LINES / 2 - 3, COLS/2 - 6, "%s", "  EPIC WIN!  ");
    mvprintw(LINES / 2 - 2, COLS/2 - 6, "%s", "   Q - Quit  ");
    mvprintw(LINES / 2 - 1, COLS/2 - 6, "%s",     "   N - New   ");
    mvprintw(LINES / 2, COLS/2 - 6, "%s", "             ");
    
    attroff(COLOR_PAIR(17));
}

void print_game_over()
{
    attron(COLOR_PAIR(14));

    mvprintw(LINES / 2 - 4, COLS/2 - 7, "%s", "              ");
    mvprintw(LINES / 2 - 3, COLS/2 - 7, "%s", "  GAME OVER!  ");
    mvprintw(LINES / 2 - 2, COLS/2 - 7, "%s", "   Q - Quit   ");
    mvprintw(LINES / 2 - 1, COLS/2 - 7, "%s", "   N - NEW    ");
    mvprintw(LINES / 2, COLS/2 - 7, "%s", "              ");
    
    attroff(COLOR_PAIR(14));
}

void print_are_you_sure()
{
    attron(COLOR_PAIR(14));

    mvprintw(LINES / 2 - 4, COLS/2 - 7, "%s", "              ");
    mvprintw(LINES / 2 - 3, COLS/2 - 7, "%s", "   YOU SURE?  ");
    mvprintw(LINES / 2 - 2, COLS/2 - 7, "%s", "   Y/y - Yes  ");
    mvprintw(LINES / 2 - 1, COLS/2 - 7, "%s", "              ");
    mvprintw(LINES / 2, COLS/2 - 7, "%s", "              ");
    
    attroff(COLOR_PAIR(14));
}

void init_color_pairs()
{
    init_pair(1, COLOR_WHITE, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(4, COLOR_WHITE, COLOR_RED);
    init_pair(5, COLOR_GREEN, COLOR_YELLOW);
    init_pair(6, COLOR_BLUE, COLOR_YELLOW);
    init_pair(7, COLOR_MAGENTA, COLOR_YELLOW);
    init_pair(8, COLOR_RED, COLOR_YELLOW);
    init_pair(9, COLOR_MAGENTA, COLOR_GREEN);
    init_pair(10, COLOR_YELLOW, COLOR_GREEN);
    init_pair(11, COLOR_MAGENTA, COLOR_GREEN);
    init_pair(12, COLOR_RED, COLOR_GREEN);
    init_pair(13, COLOR_MAGENTA, COLOR_WHITE);
    init_pair(14, COLOR_WHITE, COLOR_RED);
    init_pair(15, COLOR_WHITE, COLOR_CYAN);
    init_pair(16, COLOR_CYAN, COLOR_WHITE);
}

void print_background(int color)
{
    char * line = malloc(COLS + 1);
    for(int i = 0; i < COLS; i++) {
        line[i] = ' ';
    }
    for(int i = 0; i < LINES; i++)
        mvprintw(i, 0, "%s", line);
}

void start()
{
	start_color();

    init_color_pairs();

    curs_set(0);
    raw();
    keypad(stdscr, TRUE);
    noecho();
    refresh();
}

int main()
{	
    srand(time(NULL));
    initscr();
	start();

    linked_list * point_list;

    bkgd(COLOR_PAIR(15));

    fill_with_points_times(2);

    while(1) {
        free_moves();
        print_field();
        print_scores();
        int ch = getch();

        if(ch == 27 || ch == 'q' || ch == 'Q') {

    
            print_are_you_sure();
            
            ch = getch();

            if(ch == 'y' || ch == 'Y') {
                endwin(); 
                return 0;   
            }
        
            continue;
        }

        switch(ch)
        {
            case KEY_LEFT: 
                point_list = get_all_points(FIELD, 'r');
                ch = 'r';
                break;
            case KEY_RIGHT:
                point_list = get_all_points(FIELD, 'l');
                ch = 'l';
                break;
            case KEY_DOWN:
                ch = 'u';
                point_list = get_all_points(FIELD, 'u');
                break;
            case KEY_UP:
                ch = 'd';
                point_list = get_all_points(FIELD, 'd');
                break;
            default: 
                print_background(15);
                print_field();
                print_scores();
                refresh();
                continue;
        }

        make_movement(point_list, ch);
        print_scores();

        if(is_game_win()) {
            print_game_win();   
            while((ch = getch())) {
                if(ch == 'n' || ch == 'N') {
                    print_empty_field();
                    clear_field();
                    fill_with_points_times(2);
                    break;
                } else if(ch == 'q' || ch == 'Q') {
                    endwin(); 
                    return 0;
                }
            }
        }

        if(point_list) {
            fill_with_points();

            if(check_game_over(&point_list)) {
                print_field();
                print_game_over();
                while((ch = getch())) {
                    
                    if(ch == 'n' || ch == 'N') {
                        print_empty_field();
                        clear_field();
                        fill_with_points_times(2);
                        break;
                    } else if(ch == 'q' || ch == 'Q') {
                        endwin(); 
                        return 0;
                    }
                }

            }
            free_moves();
            print_field();
        }
        
    }

    endwin(); 
	return 0;
}
