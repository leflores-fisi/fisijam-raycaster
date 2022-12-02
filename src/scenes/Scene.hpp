#pragma once

#include <SFML/Graphics.hpp>

namespace jam {

class Scene {
  public:
    bool ended;
    bool initiated;
    bool endTransitionPlayed;
    sf::Sprite renderSprite;
    sf::RenderTexture renderTexture;
    sf::RenderWindow* m_window;
    virtual void init(float delta) = 0;
    virtual void mainloop(float delta) = 0;
    virtual void dispatchEvent(const sf::Event &event) = 0;
    virtual void handlePrimaryInput(float delta) = 0;
    void end() {
        this->ended = true;
        this->initiated = false;
    }
    Scene(sf::RenderWindow* window) : ended(false), renderSprite(), renderTexture() {
        this->m_window = window;
    }
};

} // namespace jam
