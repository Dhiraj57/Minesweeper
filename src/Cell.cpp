#include <vector>

#include "Headers/Cell.hpp"
#include "Headers/GetCell.hpp"
#include "Headers/Global.hpp"

Cell::Cell(int i_x, int i_y)
{
    mines_around = 0;
    x = i_x;
    y = i_y;
    reset();
}

bool Cell::get_is_flagged()
{
    return is_flagged;
}

bool Cell::get_is_mine()
{
    return is_mine;
}

bool Cell::get_is_open()
{
    return is_open;
}

bool Cell::open(std::vector<Cell>& i_cells)
{
    if(is_open == 0 && is_flagged == 0)
    {
        is_open = 1;

        // We check weather the neighboring cell is empty and we open them all.
        if(is_mine == 0 && mines_around == 0 )
        {
            for(int a = -1; a < 2; a++)
            {
                for(int b = -1; b < 2; b++)
                {
                    // We are checking for cells inside grid.
                    if((a == 0 && b == 0) || (a + x < 0 || b + y < 0 || COLUMNS == a + x || ROWS == b + y))
                    {
                        continue;
                    }

                    get_cell(a + x, b + y, i_cells)->open(i_cells);
                }
            }
        }

        // If the cell we open is mine, we start the effect.
        effect_timer -= is_mine;

        // Return whether or not the cell has a mine
        return is_mine;
    }

    return 0;
}

bool Cell::update_effect_timer()
{
    if(effect_timer > 0)
    {
        effect_timer--;

        if(effect_timer == 0)
        {
            return 1;
        }
    }

    return 0;
}

int Cell::get_effect_timer()
{
    return effect_timer;
}

int Cell::get_mines_around()
{
    return mines_around;
}

int Cell::get_mouse_state()
{
    return mouse_state;
}

void Cell::count_mines_around(std::vector<Cell>& i_cells)
{
    mines_around = 0;

    // If this cell is not mine.
    if(is_mine == 0)
    {
        for(int a = -1; a < 2; a++)
        {
            for(int b = -1; b < 2; b++)
            {
                // We are checking for cells inside grid. (0,0) is this cell.
                if((a == 0 && b == 0) || (a + x < 0 || b + y < 0 || COLUMNS == a + x || ROWS == b + y))
                {
                    continue;
                }

                if(get_cell(a + x, b + y, i_cells)->get_is_mine())
                {
                    mines_around++;
                }
            }
        }
    }
}

void Cell::flag()
{
    if(is_open == 0)
    {
        is_flagged = 1 - is_flagged;
    }
}

void Cell::reset()
{
    is_flagged = 0;
    is_mine = 0;
    is_open = 0;

    effect_timer = EFFECT_DURATION;
    mouse_state = 0;
}

void Cell::set_effect_timer(int i_effect_timer)
{
    effect_timer = i_effect_timer;
}

void Cell::set_mine()
{
    is_mine = 1;
}

void Cell::set_mouse_state(int i_mouse_state)
{
    mouse_state = i_mouse_state;
}