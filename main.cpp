#include <chrono>
#include <random>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "src/Math.cpp"
#include "src/Headers/Cell.hpp"
#include "src/Headers/DrawText.hpp"
#include "src/Headers/Field.hpp"
#include "src/Headers/GetCell.hpp"
#include "src/Headers/Global.hpp"

    
int main()
{ 
    // To make game framerate independent.
    int lag = 0;
    
    std::chrono::time_point<std::chrono::steady_clock> previous_time;

    sf::Event event;
    sf::RenderWindow window(sf::VideoMode(BOARD_WIDTH * SCREEN_RESIZE, BOARD_HEIGHT * SCREEN_RESIZE), "Minesweeper", sf::Style::Close);
    window.setView(sf::View(sf::FloatRect(0, 0, BOARD_WIDTH, BOARD_HEIGHT)));

    // Sound addition.
    sf::SoundBuffer bufferMusic;
    bufferMusic.loadFromFile("src/Resources/Audio/GameMusic_02.wav");
    sf::Sound music;
    music.setBuffer(bufferMusic);
    music.setLoop(true);
    music.play();

    sf::Sprite Alexander;
    sf::Texture Alexander_texture1;
    sf::Texture Alexander_texture2;
    sf::Texture Alexander_texture3;
    Alexander_texture1.loadFromFile("src/Resources/Images/Alexander1.png");
    Alexander_texture2.loadFromFile("src/Resources/Images/Alexander2.png");
    Alexander_texture3.loadFromFile("src/Resources/Images/Alexander3.png");
    Alexander_texture1.setSmooth(true);
    Alexander_texture2.setSmooth(true);
    Alexander_texture3.setSmooth(true);
    Alexander.setScale(0.138, 0.138);
    Alexander.setPosition(CELL_SIZE * COLUMNS - 24, 13);
    Alexander.setTexture(Alexander_texture1);

    sf::Sprite board;
    sf::Texture board_texture;
    board_texture.loadFromFile("src/Resources/Images/board9.png");
    board_texture.setSmooth(true);
    board.setScale(0.663, 0.663);
    board.setPosition(0, 0);
    board.setTexture(board_texture);

    Field field;

    previous_time = std::chrono::steady_clock::now();

    while(window.isOpen())
    {
        //Here we're calculating the lag.
        int delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();

        lag += delta_time;

        previous_time += std::chrono::microseconds(delta_time);

        // When lag exceeds one frame duration.
        while(FAMRE_DURATION <= lag)
        {
            // To get co-ordinates of the cell under the mouse.
            int mouse_cell_x = -1;
            int mouse_cell_y = -1;

            // std::cout << static_cast<float>(sf::Mouse::getPosition(window).x) << " " << sf::Mouse::getPosition(window).y << std::endl;

            // To check if mouse is in the grid.
            if(sf::Mouse::getPosition(window).x  < CELL_SIZE * SCREEN_RESIZE * (COLUMNS + OFFSET_WIDTH) && sf::Mouse::getPosition(window).x >= OFFSET_WIDTH * CELL_SIZE * SCREEN_RESIZE && 
                sf::Mouse::getPosition(window).y < CELL_SIZE * SCREEN_RESIZE * (ROWS + OFFSET_HEIGHT) && sf::Mouse::getPosition(window).y >= OFFSET_HEIGHT * CELL_SIZE * SCREEN_RESIZE)
            {
                mouse_cell_x = clamp<int>(static_cast<int>(((sf::Mouse::getPosition(window).x - (OFFSET_WIDTH * SCREEN_RESIZE * CELL_SIZE))/ static_cast<float>(CELL_SIZE * SCREEN_RESIZE))), 0, COLUMNS - 1) ;
                mouse_cell_y = clamp<int>(static_cast<int>(((sf::Mouse::getPosition(window).y - (OFFSET_HEIGHT * SCREEN_RESIZE * CELL_SIZE))/ static_cast<float>(CELL_SIZE * SCREEN_RESIZE))), 0, ROWS - 1) ;
            }

            // To reset timer to zero.
            lag -= FAMRE_DURATION;

            while (window.pollEvent(event))
            {
                switch(event.type)
                {
                    case sf::Event::Closed:
                    {
                        window.close();
                        break;
                    }
                    case sf::Event::KeyReleased:
                    {
                        switch(event.key.code)
                        {
                            case sf::Keyboard::Enter:
                            {
                                // We restart the game after Enter is released.
                                if(field.get_game_over() != 0)
                                {
                                    music.play();
                                }
                                field.restart();
                            }
                        }

                        break;
                    }
                    case sf::Event::MouseButtonReleased:
                    {
                        if(mouse_cell_x != -1)
                        {
                            switch(event.mouseButton.button)
                            {
                                case sf::Mouse::Left:
                                {
                                    field.open_cell(mouse_cell_x, mouse_cell_y);
                                    break;
                                }
                                case sf::Mouse::Right:
                                {                                    
                                    field.flag_cell(mouse_cell_x, mouse_cell_y);
                                }
                            }
                        }
                    }
                }
            }


            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse_cell_x != -1)
            {
                // We change the mouse state of the cell.
                field.set_mouse_state(2, mouse_cell_x, mouse_cell_y);
            }
            else
            {
                field.set_mouse_state(1, mouse_cell_x, mouse_cell_y);
            }

            // If we loose the game.
            if(field.get_game_over() == -1)
            {
                music.stop();
                Alexander.setTexture(Alexander_texture3);
            }
            // If we win the game.
            else if(field.get_game_over() == 1)
            {
                music.stop();
                Alexander.setTexture(Alexander_texture2);
            }
            else
            {
                Alexander.setTexture(Alexander_texture1);
            }

            // Here we draw all things.
            if(FAMRE_DURATION > lag)
            {
                window.clear();

                window.draw(board);

                // Drawing cells.
                field.draw(window);

                if(field.effect_over() == 1)
                {
                    if(field.get_game_over() == 1)
                    {
                        draw_text(static_cast<int>(round(0.5f * (CELL_SIZE * (COLUMNS + OFFSET_WIDTH) - 9 * FONT_WIDTH))), static_cast<int>(round(0.5f * (CELL_SIZE * (ROWS + OFFSET_HEIGHT) - 0 * FONT_HEIGHT))), 25, "VICTORY", "DS-DIGIB", window, sf::Color::Black);    
                    }
                    else
                    {
                        draw_text(static_cast<int>(round(0.5f * (CELL_SIZE * (COLUMNS + OFFSET_WIDTH) - 5 * FONT_WIDTH))), static_cast<int>(round(0.5f * (CELL_SIZE * (ROWS + OFFSET_HEIGHT) - 1 * FONT_HEIGHT))), 25, "GAME\nOVER", "DS-DIGIB", window, sf::Color::Black);
                    }
                }

                draw_text(22 + OFFSET_WIDTH, 8, 26, "Mines : " + std::to_string(MINES - field.get_flags()), "Lateef-Regular", window, sf::Color::Black);

                window.draw(Alexander);
                window.display();
            }
        }
    }

    return 0;
}
