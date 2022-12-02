#pragma once
#include <SFML/Graphics.hpp>
#include "camera.hpp"

// REDEFINED
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

namespace jam {

class Item : public sf::Drawable {

    sf::Texture itemTexture;
	sf::RenderTexture itemRenderTexture;
	sf::Sprite itemSprite;
    sf::VertexArray itemLines;
  public:
    sf::Vector2f position;
    Item(std::string filename, sf::Vector2f pos);
    ~Item();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void render();
    void update(const jam::Camera &camera);
    bool isColliding(sf::Vector2f playerPos);
};

}
