#include "../include/main.h"

linked_list * moves;

int TEMP_FIELD[SIDELEN][SIDELEN];

void add_transition_to_moves(point from, point to)
{
    linked_list * temp = (linked_list *) malloc(sizeof(linked_list));
    temp->trans.from = from;
    temp->trans.to = to;
    temp->trans.offset = 0;
    temp->next = NULL;

    linked_list * temp_head = moves;
    if(temp_head == NULL) {
        moves = temp;
    } else {
        while(temp_head->next) {
            temp_head = (linked_list *) temp_head->next;
        } 
        temp_head->next = (struct linked_list *) temp;
    }
}

int point_and_vector_sum(point * from, vector direction)
{
    if(from->x+direction.vx >= 0 && from->x+direction.vx < SIDELEN && from->y+direction.vy >= 0 && from->y+direction.vy < SIDELEN) {
        from->x = from->x+direction.vx;
        from->y = from->y+direction.vy;
        return 1;
    }
    return 0;
}

int get_val_by_point(point pnt)
{
    return TEMP_FIELD[pnt.y][pnt.x];
}

int set_val_by_point(point dest, int value)
{
    return TEMP_FIELD[dest.y][dest.x] = value;
}

void get_line_points(vector vec, point next)
{
    point before = next;
    
    while(point_and_vector_sum(&next, vec)) {
        if(get_val_by_point(next) == 0)
            continue;

        if(get_val_by_point(before) == 0) {
            add_transition_to_moves(next, before);
            set_val_by_point(before, get_val_by_point(next));
            set_val_by_point(next, 0);
        } else if(get_val_by_point(before) == get_val_by_point(next)) {
            add_transition_to_moves(next, before);
            set_val_by_point(before, get_val_by_point(next) * 2);   
            set_val_by_point(next, 0);         
            point_and_vector_sum(&before, vec);
        } else if(get_val_by_point(before) != get_val_by_point(next)) {
            point_and_vector_sum(&before, vec);
            if(before.x != next.x || before.y != next.y) {
                add_transition_to_moves(next, before);
                set_val_by_point(before, get_val_by_point(next));
                set_val_by_point(next, 0);
            }
        }
    }
    
    putchar('\n');
}

void get_points(char movement)
{
    for(int i = 0; i < SIDELEN; i++) {
        switch(movement)
        {
            case 'l': get_line_points((vector){1,0}, (point){0,i}); break;
            case 'r': get_line_points((vector){-1,0},(point){3,i}); break;
            case 'u': get_line_points((vector){0,1}, (point){i,0}); break;
            case 'd': get_line_points((vector){0,-1},(point){i,3}); break;
        }
    }
}

void free_moves()
{
    if(moves) {
        linked_list * tmp;
        while(moves != NULL)
        {
            tmp = moves;
            moves = (linked_list *) moves->next;
            free(tmp);
        }
    }
}

linked_list * get_all_points(int ARR[SIDELEN][SIDELEN], char movement)
{
    free_moves();
    moves = NULL;

    for(int i = 0; i < SIDELEN; i++)
        for(int j = 0; j < SIDELEN; j++)
            TEMP_FIELD[j][i] = ARR[j][i];

    get_points(movement);

    return moves;
}