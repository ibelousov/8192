#include <assert.h>

#include "../include/main.h"


static int test_empty_moves() 
{
  int FIELD[SIDELEN][SIDELEN] = {
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
  };

  linked_list * moves = get_all_points(FIELD, 'l');
  assert(moves == NULL);

  moves = get_all_points(FIELD, 'r');
  assert(moves == NULL);

  return 0;
}

static int test_filled_moves() 
{
  int FIELD[SIDELEN][SIDELEN] = {
    {2,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
  };

  linked_list * moves = get_all_points(FIELD, 'r');
  
  assert(moves->next == NULL);

  free_moves();

  return 0;
}

int main(void) 
{
  test_empty_moves();

  test_filled_moves();

  return 0;
}