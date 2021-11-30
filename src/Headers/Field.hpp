#pragma once

class Field
{
    bool first_click;

    int game_over;

    sf::SoundBuffer buffer;
    sf::Sound sound;

    // To generate random values.
    std::default_random_engine random_engine;

    // To give random seed to generator.
    std::random_device random_device;

    std::vector<Cell> cells;

public:
    Field();

    bool effect_over();

    int get_game_over();
    int get_flags();

    void draw(sf::RenderWindow& i_winndow);
    void flag_cell(int i_x, int i_y);
    void open_cell(int i_x, int i_y);
    void restart();
    void set_mouse_state(int i_mouse_state, int i_x, int i_y);
};