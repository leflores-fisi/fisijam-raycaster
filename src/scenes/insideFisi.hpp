#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Scene.hpp"
#include "../include/AudioManager.hpp"
#include "../include/camera.hpp"
#include "../include/utils.hpp"
#include "../include/UI.hpp"
#include "../include/item.hpp"
#include "../include/map.hpp"
#include "../include/screamer.hpp"
#include "../include/globals.hpp"

#define WALK_MULTIPLIER 10.f

namespace jam {

class InsideFisi : public Scene {
    
    jam::Camera camera;
    int levelID;
    const int levelStages = 2;
    // Level locals
    struct Level {
        sf::Vector2f spawnDirection;
        sf::Vector2f spawnPosition;
        char map[MAP_HEIGHT][MAP_WIDTH];
    };
    jam::Item item_keyToFind;
    jam::Item item_fisiCat;
    bool hasTheKey = false;
    bool showKeyAdvice = false;

    sf::Vector2i windowCenter = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	sf::Vector2i previousMousePosition;

    Level levels[2] = {
        { 
            sf::Vector2f(-1.f, 0.f), // spawn look direction
            sf::Vector2f(27.f, 5.f), // spawn position
            {  }
        },
        { 
            sf::Vector2f(-1.f, 0.f),
            sf::Vector2f(25.f, 10.f),
            {  }
        },
    };
    jam::Screamer mariaelena_screamer;
    jam::Screamer huayna_screamer;

    sf::RectangleShape blackScreen = sf::RectangleShape();
    float transitionTimer = 5.f;

  public:
    InsideFisi(sf::RenderWindow* window);

    Level currentLevel() {
        return levels[levelID];
    }
    void setLevel(int level);

    void init(float delta);

    void handlePrimaryInput(float delta);

    void renderTransition(float delta);

    void mainloop(float delta);

    void dispatchEvent(const sf::Event &event);
};

} // namespace jam
