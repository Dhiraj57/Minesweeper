#pragma once

class Cell
{
    bool is_flagged;
    bool is_mine;
    bool is_open;

    int effect_timer;
    int mines_around;
    int mouse_state;
    int x;
    int y;

public:
    Cell(int x, int y);

    bool get_is_flagged();
    bool get_is_mine();
    bool get_is_open();
    bool open(std::vector<Cell>& i_cells);
    bool update_effect_timer();

    int get_effect_timer();
    int get_mines_around();
    int get_mouse_state();

    void count_mines_around(std::vector<Cell>& i_cells);
    void flag();
    void reset();
    void set_effect_timer(int i_effect_timer);
    void set_mine();
    void set_mouse_state(int i_mouse_state);
};