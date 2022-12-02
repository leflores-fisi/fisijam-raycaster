#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Scene.hpp"
#include "../include/AudioManager.hpp"
#include "../include/utils.hpp"
#include "../include/UI.hpp"

#define SCREEN_WIDTH 1280 // FIX: defined in other files
#define SCREEN_HEIGHT 720

#define WALK_MULTIPLIER 10.f

namespace jam {

class Intro : public Scene {
    float walk;
    bool hasEnteredToFISI;
    float stepOffset;

    bool showIntroTutorial;

    sf::Texture fisiTexture;
    sf::Sprite fisiSprite;
    sf::Clock opacityClock;

  public:
    Intro(sf::RenderWindow* window);
    void init(float delta);
    void mainloop(float delta);
    void dispatchEvent(const sf::Event &event);
    void handlePrimaryInput(float delta);
};

} // namespace jam
