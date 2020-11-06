#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <ao/ao.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>  

#define SIDELEN 4
#define GAME_WIN_SCORES 8192

struct {
    int x;
    int y;
} typedef point;

struct {
    int vx;
    int vy;
} typedef vector;

struct {
    point from;
    point to;
    int offset;
} typedef transition;

struct {
    transition trans;
    struct linked_list * next;
} typedef linked_list;

struct {
    point pnt;
    struct linked_list_points * next;
} typedef linked_list_points;

void * play_pluck_th(void *);

void free_moves();
linked_list * get_all_points(int [SIDELEN][SIDELEN], char);

#endif
