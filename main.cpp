#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include <cmath>
#include <iostream>

#include "src/include/utils.hpp"
#include "src/include/map.hpp"
#include "src/include/textures.hpp"
#include "src/include/item.hpp"
#include "src/include/screamer.hpp"

// Scenes and managers
#include "src/scenes/intro.hpp"
#include "src/scenes/insideFisi.hpp"
#include "src/include/AudioManager.hpp"
#include "src/include/UI.hpp"
// #include "src/scenes/menu.hpp"

#include "src/include/globals.hpp"
bool g_gamePaused = false;

using namespace std::chrono_literals;
#define WALK_MULTIPLIER 10.f


int main() {
    jam::AudioManager::loadSound("assets/audio/step_1.wav", "audio/step_1");
    jam::AudioManager::loadSound("assets/audio/step_2.wav", "audio/step_2");
    jam::AudioManager::loadSound("assets/audio/step_3.wav", "audio/step_3");

    jam::AudioManager::loadMusic("assets/audio/backgroundSound.ogg", "background_music");
    jam::AudioManager::getMusic("background_music")->setLoop(true);

    jam::AudioManager::loadMusic("assets/audio/breathing.ogg", "breathing_music");
    jam::AudioManager::getMusic("breathing_music")->setLoop(true);

    jam::AudioManager::loadMusic("assets/audio/laughSingingScream.ogg", "creppy_background_music");
    jam::AudioManager::getMusic("creppy_background_music")->setLoop(true);

    jam::AudioManager::loadMusic("assets/audio/winMusic.wav", "win_music");

    jam::AudioManager::loadSound("assets/audio/transition.ogg", "transition_sound");

    jam::AudioManager::loadSound("assets/audio/screamer1.wav", "screamer_sound");

	// Clock
	sf::Clock clock;

    sf::Texture winTextureImage;
    if (!winTextureImage.loadFromFile("assets/winImage.png")) {
        throw -1;
    }
    winTextureImage.setSmooth(true);
    sf::Sprite winImageSprite(winTextureImage);

    sf::Clock timer;
	float delta = 0.f;

	// Make main window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Fisijam");
	window.setVerticalSyncEnabled(false);
	window.setMouseCursorVisible(false);
	window.setMouseCursorGrabbed(true);
    window.setFramerateLimit(60); // HELP

    jam::Intro introScene(&window);
    jam::InsideFisi insideFisiScene(&window);
    jam::UI::setup();

	while (window.isOpen()) {
        window.clear(sf::Color::Black);
        jam::UI::clear();
        delta = timer.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            // Global events
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                g_gamePaused = !g_gamePaused;
                if (g_gamePaused) jam::AudioManager::pauseAll();
                else              jam::AudioManager::resumeAll();
            }

            if (g_gamePaused) continue;

            if (!introScene.ended) introScene.dispatchEvent(event);
            else insideFisiScene.dispatchEvent(event);
        }

        if (!introScene.ended) {
            introScene.mainloop(delta);
            window.draw(introScene.renderSprite);
        }
        else {
            insideFisiScene.mainloop(delta);
            window.draw(insideFisiScene.renderSprite);
        }

        if (g_gamePaused) jam::UI::drawPausedText();

        jam::UI::display();
        window.draw(jam::UI::getSprite());
        window.display();

        // Wipe sounds
        jam::AudioManager::refreshSoundStatus();
    }

    jam::AudioManager::stopAll();
    return EXIT_SUCCESS;
}
