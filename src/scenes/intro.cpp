#include "intro.hpp"
#include "../include/globals.hpp"

jam::Intro::Intro(sf::RenderWindow* window) :
    Scene(window),
    walk(0.f), hasEnteredToFISI(false), stepOffset(0.f), showIntroTutorial(true),
    fisiSprite(), opacityClock() {
    // Load textures
    bool loaded = fisiTexture.loadFromFile("assets/fisi.png");
    if (!loaded) {
        throw std::runtime_error("Failed to load FISI texture");
    }
    fisiTexture.setSmooth(true);
    fisiSprite.setTexture(fisiTexture);

    fisiSprite.setScale(SCREEN_WIDTH / fisiSprite.getLocalBounds().width, SCREEN_HEIGHT / fisiSprite.getLocalBounds().height);

    renderTexture.create(SCREEN_WIDTH, SCREEN_HEIGHT);
    renderSprite.setTexture(renderTexture.getTexture());
}

void jam::Intro::init(float delta) {
    // Play ambient
    // if (!playedAmbient) {
    //     AudioManager::playMusic("ambient");
    //     playedAmbient = true;
    // }
}

void jam::Intro::mainloop(float delta) {
    if (!this->initiated) this->init(delta);
    renderTexture.clear(sf::Color::Black);
    
    if (!g_gamePaused) {
        // Handle keyboard input
        this->handlePrimaryInput(delta);
    }
    // Draw FISI faculty
    renderTexture.draw(fisiSprite);
    if (hasEnteredToFISI) {
        this->end();
    }

    if (showIntroTutorial) {
        std::cout << "Showing intro tutorial" << std::endl;
        jam::UI::drawDialogText("Presiona W para avanzar a la FISI");
    }
    renderTexture.display();
}

void jam::Intro::dispatchEvent(const sf::Event &event) { }

void jam::Intro::handlePrimaryInput(float delta) {
    /* ---------------- INTRO LOGIG ---------------- */
    // Update
    static bool walkSoundPlayed = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        // walk += 5.f * delta; // HERE: walk velocity
        walk += 20.f * delta;
        if (walk >= 50.f) {
            hasEnteredToFISI = true;
        }
        if (showIntroTutorial) {
            showIntroTutorial = false;
        }
        std::cout << "walk: " << walk << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        if (walk > 10.f * delta) {
            walk -= 10.f * delta;
        }
    }
    stepOffset = std::abs(std::sin(walk) * 10) * 0.5f;
    if (stepOffset < 0.25f) {
        if (!walkSoundPlayed) {
            // get random step sound from loaded files
            walkSoundPlayed = true;
            jam::AudioManager::playSound(
                "audio/step_" + std::to_string(int(std::floor(random_float(1, 3 + 1)))), random_float(0.75f, 1.25f)
            );
        }
    }
    else {
        walkSoundPlayed = false;
    }
    float to_walk = walk * WALK_MULTIPLIER;
    // zoom by walk
    float scalex = ((SCREEN_WIDTH + 2*to_walk) / fisiSprite.getLocalBounds().width);
    float aspectRatio = SCREEN_WIDTH / SCREEN_HEIGHT;
    float scaley = (1/aspectRatio) * scalex;
    fisiSprite.setScale(scalex, scaley);
    fisiSprite.setPosition(
        -to_walk,
        SCREEN_HEIGHT - fisiSprite.getLocalBounds().height * fisiSprite.getScale().y + stepOffset
    );
}
