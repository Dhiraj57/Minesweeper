#include <string>

#include <SFML/Graphics.hpp>
#include "Headers/DrawText.hpp"

void draw_text(int i_x, int i_y, int characterSize, const std::string& i_text, const std::string& i_font, sf::RenderWindow& i_window, sf::Color color)
{
    sf::Font font;
    font.loadFromFile("src/Resources/Fonts/"+ i_font +".ttf");

    sf::Text text;
    text.setFont(font);
    text.setString(i_text);
    text.setCharacterSize(characterSize);
    text.setFillColor(color);
    text.setPosition(i_x, i_y);
    i_window.draw(text);
}



void draw_textFromTexture(bool i_black, int i_x, int i_y, const std::string& i_text, sf::RenderWindow& i_window)
{
    int character_x = i_x;
    int character_y = i_y;
    int character_width;

    sf::Sprite character_sprite;
    sf::Texture font_texture;
    font_texture.loadFromFile("src/Resources/Images/Font.png");
    font_texture.setSmooth(true);
    character_sprite.setTexture(font_texture);

    character_width = font_texture.getSize().x / 96;

    if(i_black)
    {
        character_sprite.setColor(sf::Color(0, 0, 0));
    }

    for(const char a : i_text)
    {
        if('\n' == a)
        {
            character_x = i_x;
            character_y += font_texture.getSize().y;
            continue;
        }

        character_sprite.setPosition(character_x, character_y);
        character_sprite.setTextureRect(sf::IntRect(character_width * (a - 32), 0, character_width, font_texture.getSize().y));

        character_x += character_width;

        i_window.draw(character_sprite);
    }
}