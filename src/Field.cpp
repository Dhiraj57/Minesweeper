#include <random>
#include <SFML/Graphics.hpp>

#include "Headers/Cell.hpp"
#include "Headers/Field.hpp"
#include "Headers/GetCell.hpp"
#include "Headers/Global.hpp"

Field::Field() : 
    random_engine(random_device())
{
    game_over = 1;

    // Here we're adding cells to the cells vector.
    for(int a = 0; a < ROWS; a++)
    {
        for(int b = 0; b < COLUMNS; b++)
        {
            cells.push_back(Cell(b, a));
        }
    }
    restart();
}


bool Field::effect_over()
{
    for(Cell& cell : cells)
    {
        // If any cell's effect timer is not over yet.
        if(cell.get_effect_timer() > 0)
        {
            return 0;
        }
    }
    return 1;
}


int Field::get_game_over()
{
    return game_over;
}


int Field::get_flags()
{
    int total_flags = 0;

    // We count the total number of flagged cells.
    for(Cell& cell : cells)
    {
        total_flags += cell.get_is_flagged();
    }

    return total_flags;
}


void Field::draw(sf::RenderWindow& i_window)
{
    // To draw effects.
    sf::RectangleShape cell_shape(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

    // To draw cells.
    sf::Sprite tile_sprite;
    sf::Texture tile_texture;
    tile_texture.loadFromFile("src/Resources/Images/tiles.jpg");
    tile_texture.setSmooth(true);
    tile_sprite.setTexture(tile_texture);
    tile_sprite.setTextureRect(sf::IntRect(10 * FONT_HEIGHT * 2 + 0.5, 0 + 0.5, FONT_HEIGHT * 2 - 0.5, FONT_HEIGHT * 2 - 0.5));
    tile_sprite.setScale(0.5f, 0.5f);

    // We loop through each cell.
    for(int a = 0; a < COLUMNS; a++)
    {
        for(int b = 0; b < ROWS; b++)
        {
            float x = a + OFFSET_WIDTH;
            float y = b + OFFSET_HEIGHT;

            tile_sprite.setPosition(static_cast<int>(CELL_SIZE * x), static_cast<int>(CELL_SIZE * y));
            i_window.draw(tile_sprite);

            // If current cell is open.
            if(get_cell(a, b, cells)->get_is_open() == 1)
            {
                // We get number of mines arround this cell.
                int mines_around = get_cell(a, b, cells)->get_mines_around();

                tile_sprite.setTextureRect(sf::IntRect(mines_around * FONT_HEIGHT * 2, 0, FONT_HEIGHT * 2 , FONT_HEIGHT * 2 ));
                i_window.draw(tile_sprite);
            }

            // If the cell is closed.
            else
            {
                tile_sprite.setTextureRect(sf::IntRect(10 * FONT_HEIGHT * 2, 0, FONT_HEIGHT * 2 , FONT_HEIGHT * 2 ));

                if(game_over == 0 && get_cell(a, b, cells)->get_mouse_state() == 2)
                {
                    tile_sprite.setTextureRect(sf::IntRect(0 * FONT_HEIGHT * 2, 0, FONT_HEIGHT * 2 , FONT_HEIGHT * 2 ));
                }
                i_window.draw(tile_sprite);

                // If the cell is flagged.
                if(get_cell(a, b, cells)->get_is_flagged() == 1)
                {
                    tile_sprite.setTextureRect(sf::IntRect(11 * FONT_HEIGHT * 2, 0, FONT_HEIGHT * 2 , FONT_HEIGHT * 2 ));
                    i_window.draw(tile_sprite);
                }
            }


            // Reseting cells mouse state.
            get_cell(a, b, cells)->set_mouse_state(0);


            // To draw game over and game win effects.
            // We don't draw the effect if the game is not over or the cell effect timer hasn't yet started.
            if(game_over != 0 && EFFECT_DURATION > get_cell(a, b, cells)->get_effect_timer())
            {
                // We gradually increase effect of size from 0 to CELL_SIZE.
                int effect_size = static_cast<int>(2 * round(0.5f * CELL_SIZE * ((EFFECT_DURATION - get_cell(a, b, cells)->get_effect_timer()) / static_cast<float>(EFFECT_DURATION))));
                // It also makes effect smooth.

                // The effect timer of each cell will have a random duration.
                std::uniform_int_distribution<int> effect_duration_distribution(1, EFFECT_DURATION - 1);

                // We will use the cell shape to draw effects.
                cell_shape.setPosition(floor(CELL_SIZE * (0.5f + x) - 0.5f * effect_size), floor(CELL_SIZE * (0.5f + y) - 0.5f * effect_size));
                cell_shape.setSize(sf::Vector2f(effect_size, effect_size));


                if(game_over == -1)
                {   
                    // Game lost.
                    cell_shape.setFillColor(sf::Color(192, 192, 192));
                }
                else
                {
                    // Game won.
                    cell_shape.setFillColor(sf::Color(192, 192, 192));
                }

                i_window.draw(cell_shape);

                // If the effect timer is over.
                if(get_cell(a, b, cells)->update_effect_timer() == 1)
                {
                    // Then we start each neighboring cell's effect timer.
                    if(a - 1 >= 0 && EFFECT_DURATION == get_cell(a - 1, b, cells)->get_effect_timer())
                    {
                        get_cell(a - 1, b, cells)->set_effect_timer(static_cast<int>(effect_duration_distribution(random_engine)));
                    }

                    if(b - 1 >= -1 && EFFECT_DURATION == get_cell(a, b-1, cells)->get_effect_timer())
                    {
                        get_cell(a, b-1, cells)->set_effect_timer(static_cast<int>(effect_duration_distribution(random_engine)));
                    }

                    if(a + 1 < COLUMNS && EFFECT_DURATION == get_cell(a+1, b, cells)->get_effect_timer())
                    {
                        get_cell(a+1, b, cells)->set_effect_timer(static_cast<int>(effect_duration_distribution(random_engine)));
                    }

                    if(b + 1 < ROWS && EFFECT_DURATION == get_cell(a, b+1, cells)->get_effect_timer())
                    {
                        get_cell(a, b+1, cells)->set_effect_timer(static_cast<int>(effect_duration_distribution(random_engine)));
                    }
                }
            }
        }
    }
}


void Field::flag_cell(int i_x, int i_y)
{
    if(game_over == 0)
    {
        get_cell(i_x, i_y, cells)->flag();
    }
}


void Field::open_cell(int i_x, int i_y)
{
    // If this is the first cell we're opening.
    if(first_click == 0)
    {
        // Co-ordinate distribution i.e. selecting random position for mines.
        std::uniform_int_distribution<int> x_distribution(0, COLUMNS - 1);
        std::uniform_int_distribution<int> y_distribution(0, ROWS - 1);

        first_click = 1;

        // We generate mines.
        for(int a = 0; a < MINES; a++)
        {
            int mine_x = static_cast<int>(x_distribution(random_engine));
            int mine_y = static_cast<int>(y_distribution(random_engine));

            // If the cell is already mine or it's a cell that the player wants to open.
            if(get_cell(mine_x, mine_y, cells)->get_is_mine() == 1 || (i_x == mine_x && i_y == mine_y))
            {
                a--;
            }
            else
            {
                // We set the mine.
                get_cell(mine_x, mine_y, cells)->set_mine();
            }
        }

        // Each cell counts how many mines are surrounding it.
        for(Cell& cell : cells)
        {
            cell.count_mines_around(cells);
        }
    }

    //We don't open the cell when the game is over or when the cell is flagged
    if(game_over == 0 && get_cell(i_x, i_y, cells)->get_is_flagged() == 0)
    {
        if(get_cell(i_x, i_y, cells)->open(cells) == 1)
        {
            // When the player opens a cell with a mine.
            game_over = -1;
        }
        else
        {
            int total_closed_cells = 0;

            // We count how many cells are closed.
            for(Cell& cell : cells)
            {
                total_closed_cells += 1 - cell.get_is_open();
            }

            // If the number of closed cells equals to the total number of mines, we'll consider that the game is won.
            if(MINES == total_closed_cells)
            {
                game_over = 1;

                // Then we start the effect.
                get_cell(i_x, i_y, cells)->set_effect_timer(EFFECT_DURATION -1);
            }
        }
    }
}


void Field::restart()
{
    if(game_over != 0)
    {
        first_click = 0;
        game_over = 0;

        for(Cell& cell : cells)
        {
            cell.reset();
        }
    }
}


void Field::set_mouse_state(int i_mouse_state, int i_x, int i_y)
{
    get_cell(i_x, i_y, cells)->set_mouse_state(i_mouse_state);
}