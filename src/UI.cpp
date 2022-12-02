#include <cmath>
#include "include/UI.hpp"
#include "include/AudioManager.hpp"

jam::UI::~UI() {}

void jam::UI::setup() {
    if (!defaultFont.loadFromFile("assets/fonts/caskaydiaNF.ttf")) {
        // std::cout << "No se pudo cargar la fuente" << std::endl;
        throw std::runtime_error("No se pudo cargar la fuente");
    }
    dialogText.setFont(defaultFont);
    dialogText.setCharacterSize(20);
    dialogText.setFillColor(sf::Color::White);
    dialogText.setPosition(SCREEN_WIDTH/2 - 250, SCREEN_HEIGHT - 70);
    // dialogText.setString("Presiona w para comenzar");

    dialogBox.setSize(sf::Vector2f(SCREEN_WIDTH, 100));
    dialogBox.setFillColor(sf::Color(0, 0, 0, 255));
    dialogBox.setPosition(0, SCREEN_HEIGHT - 100);

    pausedText.setFont(defaultFont);
    pausedText.setCharacterSize(50);
    pausedText.setFillColor(sf::Color::White);
    pausedText.setPosition(SCREEN_WIDTH/2 - 75, SCREEN_HEIGHT/2 - 50);
    pausedText.setString("PAUSA");
    pausedSubText.setFont(defaultFont);
    pausedSubText.setCharacterSize(20);
    pausedSubText.setFillColor(sf::Color::White);
    pausedSubText.setPosition(SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 50);
    pausedSubText.setString("Presiona ESC para continuar");
    pausedBox.setSize({ static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT) });
    pausedBox.setPosition({ 0.f, 0.f });
    pausedBox.setFillColor(sf::Color(0, 0, 0, 100));

    renderTexture.create(SCREEN_WIDTH, SCREEN_HEIGHT);
    renderSprite.setTexture(renderTexture.getTexture());
}

void jam::UI::drawDialogText(std::string text) {
    dialogText.setString(text);
    renderTexture.draw(dialogBox);
    renderTexture.draw(dialogText);
}

void jam::UI::drawPausedText() {
    renderTexture.draw(pausedBox);
    renderTexture.draw(pausedText);
    renderTexture.draw(pausedSubText);
}

void jam::UI::clear() {
    renderTexture.clear(sf::Color::Transparent);
}

void jam::UI::display() {
    renderTexture.display();
}

sf::Sprite jam::UI::getSprite() {
    return renderSprite;
}
