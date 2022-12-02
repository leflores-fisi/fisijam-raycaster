#pragma once
#include <SFML/Graphics.hpp>

#define SCREEN_WIDTH 1280 // FIX: defined in other files
#define SCREEN_HEIGHT 720

namespace jam {

class UI {
    inline static sf::Font defaultFont = sf::Font();
    inline static sf::Text dialogText = sf::Text();
    inline static sf::RectangleShape dialogBox = sf::RectangleShape();

    inline static sf::Text pausedText = sf::Text();
    inline static sf::Text pausedSubText = sf::Text();
    inline static sf::RectangleShape pausedBox = sf::RectangleShape();

    inline static sf::Sprite renderSprite = sf::Sprite();
    inline static sf::RenderTexture renderTexture = sf::RenderTexture();
  public:
    // Explicitly disallow copying
    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;

    static void setup();
    static sf::Sprite getSprite();
    static void drawDialogText(std::string text);
    static void drawPausedText();
    static void drawTransition(float delta);
    static void clear();
    static void display();
  private:
    ~UI();
};

}
