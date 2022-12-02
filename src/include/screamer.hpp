#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

namespace jam {

class Screamer : public sf::Drawable {
    sf::Sprite screamerSprite;
    sf::Texture screamerTexture;
  public:
    float screamTime = 3.f;
    bool isScreaming;
    bool ended;
    Screamer(std::string filename);
    ~Screamer();
    void screamOnce();
    void update(float delta);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        if (this->isScreaming) {
            target.draw(this->screamerSprite, states);
            std::cout << "screaming: " << this->screamTime << std::endl;
            std::cout << "bool: " << this->isScreaming << std::endl;
        }
    }
};

}
