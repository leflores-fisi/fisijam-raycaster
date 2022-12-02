#pragma once
#include <SFML/Graphics.hpp>

#define TEXTURES_WIDTH 256
#define TEXTURES_HEIGHT 256

sf::Vector2f getTexturePlacement(const sf::Vector2i& texindexX);
sf::Vector2f getTexturePlacement(const char type);
