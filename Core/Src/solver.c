#include "stdlib.h"
#include "stdio.h"

#include "solver.h"
#include "API.h"
#include "Queue.h"
#include "Cell.h"
#include "Mouse.h"

//16x16 maze of cell pointers
Cell* maze[16][16];

//Queue of cells for floodfill
Queue cell_queue;

//Mouse
Mouse mouse;

//Goal arrays :( tragically defined in solver.c
int x_goals [NUM_GOALS_X] = {7,8};
int y_goals [NUM_GOALS_Y] = {7,8};

//Solve
Action solver()
{
    floodFill();

    //TODO REMOVE ONLY FOR SIM
    set_walls(maze, &mouse);
    update_sim_graphics();
    check_if_goal_reached();

    return move_to_smaller_cell();;
}

bool wall_north(Cell* cell) { return !!(cell->walls & 0b1000); }
bool wall_east(Cell* cell) { return !!(cell->walls & 0b0100); }
bool wall_south(Cell* cell) { return !!(cell->walls & 0b0010); }
bool wall_west(Cell* cell) { return !!(cell->walls & 0b0001); }

char heading_to_char(Heading wall_direction)
{
    if(wall_direction == NORTH)
        return 'n';
    if(wall_direction == EAST)
        return 'e';
    if(wall_direction == SOUTH)
        return 's';
    if(wall_direction == WEST)
        return 'w';
}

void check_if_goal_reached()
{
    if(maze[mouse.x_coord][mouse.y_coord]->dist == 0 && mouse.x_coord != 0 && mouse.y_coord != 0)
    {
        x_goals[0] = 0; x_goals[1] = 0;
        y_goals[0] = 0; y_goals[1] = 0;
    }
    else if (maze[mouse.x_coord][mouse.y_coord]->dist == 0)
    {
        x_goals[0] = 7; x_goals[1] = 8;
        y_goals[0] = 7; y_goals[1] = 8;
    }
}

void update_sim_graphics()
{
    for(int x = 0; x < MAZE_W; x++)
    {
        for(int y = 0; y < MAZE_H; y++)
        {
            Cell* current_cell = maze[x][y];
            int x = current_cell->x_coord;
            int y = current_cell->y_coord;

            /*
            Remove this when actually implementing
            char str[20];
            sprintf(str, "%d", current_cell->dist);
            API_setText(x, y,str);
            */

            /*
            if(wall_north(current_cell)) API_setWall(x, y, heading_to_char(NORTH));
            if(wall_east(current_cell)) API_setWall(x, y, heading_to_char(EAST));
            if(wall_south(current_cell)) API_setWall(x, y, heading_to_char(SOUTH));
            if(wall_west(current_cell)) API_setWall(x, y, heading_to_char(WEST));
            */
        }
    }
}


// Populates maze with most-accurate manhattan distances
void floodFill()
{
    reset_environment();

    //Breadth-First Floodfill
    while (cell_queue.head)
    {
        Cell* current_cell = pop(&cell_queue);
        Cell** neighboring_cells = find_neighbor_cells(current_cell); // Four neighboring cells
        for(int c = 0; c < 4; c++)
        {
            if(neighboring_cells[c] && neighboring_cells[c]->dist == BLANK)
            {
                neighboring_cells[c]->dist = current_cell->dist + 1;
                push(&cell_queue, neighboring_cells[c]);

            }
        }
        free(neighboring_cells);
    }
}

Heading rotate(Heading curr_heading, int amount)
{
    switch (curr_heading)
    {
        case NORTH:
            if (amount == 1) return EAST;
            return WEST;
        case EAST:
            if (amount == 1) return SOUTH;
            return NORTH;
        case SOUTH:
            if (amount == 1) return WEST;
            return EAST;
        case WEST:
            if (amount == 1) return NORTH;
            return SOUTH;

    }
}

Action move_to_smaller_cell()
{
    int x = mouse.x_coord;
    int y = mouse.y_coord;

    Cell* current_cell = maze[x][y];
    Cell** neighboring_cells = find_neighbor_cells(current_cell);

    for(int c = 0; c < 4; c++)
    {
        Cell* neighboring_cell = neighboring_cells[c];
        if(neighboring_cell && neighboring_cell->dist < current_cell->dist)
        {
            Heading facing = mouse.facing;
            switch(cell_direction_from_mouse(neighboring_cell->x_coord, neighboring_cell->y_coord))
            {
                case NORTH:
                    if(facing == NORTH)
                    {
                        mouse.y_coord = y + 1;
                        return FORWARD;
                    }
                    else if (facing == EAST)
                    {
                        mouse.facing = rotate(facing, 0);
                        return LEFT;
                    }
                    if(API_wallLeft() && API_wallRight()) {
                    	mouse.facing = rotate(facing, 1);
                    	mouse.facing = rotate(mouse.facing, 1);
                    	return FLIP;
                    }
                    mouse.facing = rotate(facing, 1);
                    return RIGHT;
                    break;

                case EAST:
                    if(facing == EAST)
                    {
                        mouse.x_coord = x + 1;
                        return FORWARD;
                    }
                    else if (facing == SOUTH)
                    {
                        mouse.facing = rotate(facing, 0);
                        return LEFT;
                    }
                    if(API_wallLeft() && API_wallRight()) {
                    	mouse.facing = rotate(facing, 1);
                    	mouse.facing = rotate(mouse.facing, 1);
                    	return FLIP;
                    }
                    mouse.facing = rotate(facing, 1);
                    return RIGHT;
                    break;

                case SOUTH:
                    if(facing == SOUTH)
                    {
                        mouse.y_coord = y - 1;
                        return FORWARD;
                    }
                    else if (facing == WEST)
                    {
                        mouse.facing = rotate(facing, 0);
                        return LEFT;
                    }
                    if(API_wallLeft() && API_wallRight()) {
                    	mouse.facing = rotate(facing, 1);
                    	mouse.facing = rotate(mouse.facing, 1);
                    	return FLIP;
                    }
                    mouse.facing = rotate(facing, 1);
                    return RIGHT;
                    break;

                case WEST:
                    if(facing == WEST)
                    {
                        mouse.x_coord = x - 1;
                        return FORWARD;
                    }
                    else if (facing == NORTH)
                    {
                        mouse.facing = rotate(facing, 0);
                        return LEFT;
                    }
                    if(API_wallLeft() && API_wallRight()) {
                    	mouse.facing = rotate(facing, 1);
                    	mouse.facing = rotate(mouse.facing, 1);
                    	return FLIP;
                    }
                    mouse.facing = rotate(facing, 1);
                    return RIGHT;
                    break;
            }
        }
    }
    free(neighboring_cells);

    return NOTHING;
}

Heading cell_direction_from_mouse(int cell_x, int cell_y)
{
    if (cell_x - mouse.x_coord == 1)
        return EAST;
    if (cell_x - mouse.x_coord == -1)
        return WEST;
    if (cell_y - mouse.y_coord == 1)
        return NORTH;
    if (cell_y - mouse.y_coord == -1)
        return SOUTH;
}

Cell** find_neighbor_cells(Cell* current_cell)
{
    Cell** neighboring_cells = malloc(4 * sizeof(Cell*));
    for (int c = 0; c < 4; c++) {
        neighboring_cells[c] = 0;
    }

    int x = current_cell->x_coord;
    int y = current_cell->y_coord;
    unsigned int walls = current_cell->walls;

    if (x + 1 < MAZE_W && !wall_east(current_cell))
        neighboring_cells[0] = maze[x + 1][y];
    if (x - 1 >= 0 && !wall_west(current_cell))
        neighboring_cells[1] = maze[x - 1][y];
    if (y + 1 < MAZE_H && !wall_north(current_cell))
        neighboring_cells[2] = maze[x][y + 1];
    if (y - 1 >= 0 && !wall_south(current_cell))
        neighboring_cells[3] = maze[x][y - 1];

    return neighboring_cells;
}

bool cell_exists(int x, int y)
{
    return (x < MAZE_W && x >= 0 && y < MAZE_H && y >= 0);
}


//Setup the environment
void setup_environment()
{
    //Setup cell queue
    cell_queue.head = 0;
    cell_queue.tail = 0;
    cell_queue.length = 0;

    //Setup maze
    for(int x = 0; x < MAZE_W; x++)
    {
        for(int y = 0; y < MAZE_H; y++)
        {
            Cell* new_cell = malloc(sizeof(Cell));
            new_cell->x_coord = x;
            new_cell->y_coord = y;
            new_cell->dist = BLANK;
            new_cell->walls = 0b0000;

            maze[x][y] = new_cell;
        }
    }

    //Setup mouse
    mouse.x_coord = 0;
    mouse.y_coord = 0;
    mouse.facing = NORTH;
}

void reset_environment()
{
    //Setup maze
    for(int x = 0; x < MAZE_W; x++)
    {
        for(int y = 0; y < MAZE_H; y++)
        {
            maze[x][y]->dist = BLANK;
        }
    }

    //Setup goals in maze and queue
    for(int x = 0; x < NUM_GOALS_X; x++)
    {
        for(int y = 0; y < NUM_GOALS_Y; y++)
        {
            Cell* goal_cell = maze[x_goals[x]][y_goals[y]];
            goal_cell->dist = 0;
            push(&cell_queue, goal_cell);
        }
    }
}

void set_walls()
{
    int x_pos = mouse.x_coord;
    int y_pos = mouse.y_coord;
    Heading facing = mouse.facing;

    //Wall in front of mouse
    if (API_wallFront())
    {
        switch(facing)
        {
            case NORTH:
                maze[x_pos][y_pos]->walls = maze[x_pos][y_pos]->walls | 0b1000;
                if(cell_exists(x_pos, y_pos + 1)) maze[x_pos][y_pos + 1]->walls = maze[x_pos][y_pos + 1]->walls | 0b0010;
                break;
            case EAST:
                maze[x_pos][y_pos]->walls = maze[x_pos][y_pos]->walls | 0b0100;
                if(cell_exists(x_pos + 1, y_pos)) maze[x_pos + 1][y_pos]->walls = maze[x_pos + 1][y_pos]->walls | 0b0001;
                break;
            case SOUTH:
                maze[x_pos][y_pos]->walls = maze[x_pos][y_pos]->walls | 0b0010;
                if(cell_exists(x_pos, y_pos - 1)) maze[x_pos][y_pos - 1]->walls = maze[x_pos][y_pos - 1]->walls | 0b1000;
                break;
            case WEST:
                maze[x_pos][y_pos]->walls = maze[x_pos][y_pos]->walls | 0b0001;
                if(cell_exists(x_pos - 1, y_pos)) maze[x_pos - 1][y_pos]->walls = maze[x_pos - 1][y_pos]->walls | 0b0100;
                break;
        }
    }

    //Wall to the left of the mouse
    if (API_wallLeft())
    {
        switch(facing)
        {
            case EAST:
                maze[x_pos][y_pos]->walls = maze[x_pos][y_pos]->walls | 0b1000;
                if(cell_exists(x_pos, y_pos + 1)) maze[x_pos][y_pos + 1]->walls = maze[x_pos][y_pos + 1]->walls | 0b0010;
                break;
            case SOUTH:
                maze[x_pos][y_pos]->walls = maze[x_pos][y_pos]->walls | 0b0100;
                if(cell_exists(x_pos + 1, y_pos)) maze[x_pos + 1][y_pos]->walls = maze[x_pos + 1][y_pos]->walls | 0b0001;
                break;
            case WEST:
                maze[x_pos][y_pos]->walls = maze[x_pos][y_pos]->walls | 0b0010;
                if(cell_exists(x_pos, y_pos - 1)) maze[x_pos][y_pos - 1]->walls = maze[x_pos][y_pos - 1]->walls | 0b1000;
                break;
            case NORTH:
                maze[x_pos][y_pos]->walls = maze[x_pos][y_pos]->walls | 0b0001;
                if(cell_exists(x_pos - 1, y_pos)) maze[x_pos - 1][y_pos]->walls = maze[x_pos - 1][y_pos]->walls | 0b0100;
                break;
        }
    }

    //Wall to the right of the mouse
    if (API_wallRight())
    {
        switch(facing)
        {
            case WEST:
                maze[x_pos][y_pos]->walls = maze[x_pos][y_pos]->walls | 0b1000;
                if(cell_exists(x_pos, y_pos + 1)) maze[x_pos][y_pos + 1]->walls = maze[x_pos][y_pos + 1]->walls | 0b0010;
                break;
            case NORTH:
                maze[x_pos][y_pos]->walls = maze[x_pos][y_pos]->walls | 0b0100;
                if(cell_exists(x_pos + 1, y_pos)) maze[x_pos + 1][y_pos]->walls = maze[x_pos + 1][y_pos]->walls | 0b0001;
                break;
            case EAST:
                maze[x_pos][y_pos]->walls = maze[x_pos][y_pos]->walls | 0b0010;
                if(cell_exists(x_pos, y_pos - 1)) maze[x_pos][y_pos - 1]->walls = maze[x_pos][y_pos - 1]->walls | 0b1000;
                break;
            case SOUTH:
                maze[x_pos][y_pos]->walls = maze[x_pos][y_pos]->walls | 0b0001;
                if(cell_exists(x_pos - 1, y_pos)) maze[x_pos - 1][y_pos]->walls = maze[x_pos - 1][y_pos]->walls | 0b0100;
                break;
        }
    }
}
