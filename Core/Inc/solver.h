#ifndef SOLVER_H
#define SOLVER_H

#include "Cell.h"

typedef enum Heading {NORTH, EAST, SOUTH, WEST} Heading;
typedef enum Action {LEFT, FORWARD, RIGHT, NOTHING} Action;
typedef enum bool {false = 0, true = 1} bool;

#define MAZE_W 16
#define MAZE_H 16

#define BLANK -1

#define NUM_GOALS_X 2
#define NUM_GOALS_Y 2


Action solver();
void floodFill();
void check_if_goal_reached();

void setup_environment();
void reset_environment();

bool wall_north(Cell* cell);
bool wall_east(Cell* cell);
bool wall_south(Cell* cell);
bool wall_west(Cell* cell);

Cell** find_neighbor_cells(Cell* current_cell);
bool cell_exists(int x, int y);
void set_walls();

Action move_to_smaller_cell();
Heading cell_direction_from_mouse(int cell_x, int cell_y);

Heading rotate(Heading curr_heading, int amount);

//TODO REMOVE THIS WHEN ACTUAL IMPLEMENTING
void update_sim_graphics();
char heading_to_char(Heading wall_direction);

#endif
