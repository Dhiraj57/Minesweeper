#pragma once

void draw_text(int i_x, int i_y, int characterSize, const std::string& i_text, const std::string& i_font, sf::RenderWindow& i_window, sf::Color color);

void draw_textFromTexture(bool i_black, int i_x, int i_y, const std::string& i_text, sf::RenderWindow& i_window);