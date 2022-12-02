#include <SFML/Graphics.hpp>
#include <iostream>
#include "include/textures.hpp"

const sf::Vector2i wallTextureIndexes[] = {
	sf::Vector2i { 0, 0 }, // 0 (empty) 'a'
	sf::Vector2i { 1, 0 }, // 1 'b'
	sf::Vector2i { 2, 0 }, // 2 'c'
	sf::Vector2i { 3, 0 }, // 3 'd'
	sf::Vector2i { 4, 0 }, // 4 'e' --

	sf::Vector2i { 0, 1 }, // 5 'f'
	sf::Vector2i { 1, 1 }, // 6 'g'
	sf::Vector2i { 2, 1 }, // 7 'h'
	sf::Vector2i { 3, 1 }, // 8 'i'
	sf::Vector2i { 4, 1 }, // 9 'j' --

	sf::Vector2i { 0, 2 }, // 10 (final door) 'k'
	sf::Vector2i { 1, 2 }, // 11 'l'
	sf::Vector2i { 2, 2 }, // 12 'm'
	sf::Vector2i { 3, 2 }, // 13 'n'
	sf::Vector2i { 4, 2 }, // 14 'o' --

    sf::Vector2i { 0, 3 }, // 15 'p'
	sf::Vector2i { 1, 3 }, // 16 'q'
	sf::Vector2i { 2, 3 }, // 17 'r'
	sf::Vector2i { 3, 3 }, // 18 's'
	sf::Vector2i { 4, 3 }, // 19 't' -- gato: 27.5, 12.5

    sf::Vector2i { 0, 4 }, // 20 'u'
	sf::Vector2i { 1, 4 }, // 21 'v'
	sf::Vector2i { 2, 4 }, // 22 'w'
	sf::Vector2i { 3, 4 }, // 23 'x'
	sf::Vector2i { 4, 4 }, // 24 'y' --
};

sf::Vector2f getTexturePlacement(const sf::Vector2i& texindexX) {
	auto vec = sf::Vector2f(texindexX.x * (TEXTURES_WIDTH + 2) + 1, texindexX.y * (TEXTURES_HEIGHT + 2) + 1);
    return vec;
}

sf::Vector2f getTexturePlacement(const char type) {
    // 'a' -> { 0, 0 }
    // 'b' -> { 1, 0 }
    // 'c' -> { 2, 0 }
    // 'd' -> { 3, 0 }
    // 'e' -> { 0, 1 }
    // 'f' -> { 1, 1 }
    // ...
    // any other character -> { 0, 0 }
    int index = type - 'a';
	return getTexturePlacement(wallTextureIndexes[index]);
}
