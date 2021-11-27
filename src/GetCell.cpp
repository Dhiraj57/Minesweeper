#include <vector>

#include "Headers/Cell.hpp"
#include "Headers/GetCell.hpp"
#include "Headers/Global.hpp"

Cell* get_cell(int i_x, int i_y, std::vector<Cell>& i_cells)
{
    return &i_cells[i_x + COLUMNS * i_y];
}