#include "insideFisi.hpp"

jam::InsideFisi::InsideFisi(sf::RenderWindow* window) : Scene(window), levelID(0),
    item_keyToFind("assets/key.png", { 11.f, 7.f }),
    item_fisiCat("assets/fisicat.png", { 9.f, 8.3f }),
    mariaelena_screamer("assets/screamers/mariaelena.png"), huayna_screamer("assets/screamers/huayna.png"),
    blackScreen()
{
    this->camera = jam::Camera(this->levels[this->levelID].spawnPosition, this->levels[this->levelID].spawnDirection);
    renderTexture.create(SCREEN_WIDTH, SCREEN_HEIGHT);
    renderSprite.setTexture(renderTexture.getTexture());
    jam::AudioManager::loadMusic("assets/audio/backroomsAmbient.wav", "backroomsAmbient_music"); 
    jam::AudioManager::getMusic("backroomsAmbient_music")->setLoop(true);

    this->setLevel(0);

    // LOAD MAPS
    load_map(levels[0].map, "assets/maps/map1.txt");
    load_map(levels[1].map, "assets/maps/map2.txt");

    bool hasTheKey = false;
    bool showKeyAdvice = false;
    this->previousMousePosition = sf::Mouse::getPosition(*this->m_window);

    blackScreen.setSize({ static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT) });
    blackScreen.setPosition({ 0.f, 0.f });
    blackScreen.setFillColor(sf::Color(0, 0, 0, 255));
}

void jam::InsideFisi::setLevel(int level) {
    struct Level &currentLevel = levels[level];
    this->levelID = std::min(std::max(level, 0), levelStages);

    this->camera.position.x  = currentLevel.spawnPosition.x;
    this->camera.position.y  = currentLevel.spawnPosition.y;
    this->camera.direction.x = currentLevel.spawnDirection.x;
    this->camera.direction.y = currentLevel.spawnDirection.y;
    this->camera.aerial.x = 0;
    this->camera.aerial.y = this->camera.fov;
}

void jam::InsideFisi::init(float delta) {
    this->initiated = true;
    jam::AudioManager::playMusic("background_music");
    jam::AudioManager::playMusic("breathing_music");
    jam::AudioManager::playMusic("creppy_background_music");
}

void jam::InsideFisi::handlePrimaryInput(float delta) {
        if (transitionTimer > 0.f) {
            this->camera.update(delta, false, currentLevel().map);
            return;
        }
        if (mariaelena_screamer.isScreaming) {
            return;
        }
        // Character movement
        sf::Vector2f givenDirection(
            (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) -
            (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)),

            (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) -
            (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        );

        //Check for movement
        bool isMoving = givenDirection.x != 0 || givenDirection.y != 0;

        this->camera.update(delta, isMoving, currentLevel().map);

        // Movement for forward or back and diagonal
        // Set current movement speed, delta
        float moveSpeed = delta * this->camera.movementSpeed;

        // If we are moving diagonally get correct movement speed (diagonal of the square)
        if (givenDirection.x != 0 && givenDirection.y != 0) {
            moveSpeed /= M_SQRT2;
        }

        // Foreward and backwards
        sf::Vector2f &camDir = this->camera.direction;
        if (givenDirection.y != 0) {
            sf::Vector2f dir = sf::Vector2f(camDir.x, camDir.y) * givenDirection.y;
            this->camera.displaceCamera(dir * moveSpeed, currentLevel().map);
        }

        // Left and right movement
        if (givenDirection.x != 0) {
            sf::Vector2f dir = sf::Vector2f { camDir.y, -camDir.x } * givenDirection.x;
            this->camera.displaceCamera(dir * moveSpeed, currentLevel().map);
        }
    }

    void jam::InsideFisi::renderTransition(float delta) {
        std::cout << "Transition timer: " << this->transitionTimer << std::endl;
        if (transitionTimer == 5.f) jam::AudioManager::playSound("transition_sound");
        transitionTimer -= delta;
        if (transitionTimer <= 0) transitionTimer = 0;

        float opacity = (5.f - transitionTimer) / 5.f;
        float linear_opacity = opacity * 255;
        unsigned char new_opacity = 255 - static_cast<unsigned char>(std::exp(0.0217304452751f*linear_opacity));
        blackScreen.setFillColor({ 0, 0, 0, new_opacity });
        std::cout << "Opacity: "  << std::to_string(new_opacity) << std::endl;
    }

    void jam::InsideFisi::mainloop(float delta) {
        if (transitionTimer <= 0 && !this->initiated) this->init(delta);
        else if (!this->initiated) this->renderTransition(delta);

        this->renderTexture.clear(sf::Color::Transparent);
        if (!g_gamePaused) this->handlePrimaryInput(delta);

        // -------------------
        // COLLISSIONS CHECKS
        // LLAVE PARA EL KIT CACHIMBO collision
        if (item_keyToFind.isColliding(this->camera.position)) {
            hasTheKey = true;
        }
        // Floor collision check
        const char floor = currentLevel().map[int(this->camera.position.x)][int(this->camera.position.y)];
        if (floor == FLOOR_FINAL_DOOR) {
            if (hasTheKey) {
                // WIIIIINNNN
                sf::Music* winMusic = jam::AudioManager::getMusic("win_music");
                if (winMusic->getStatus() != sf::SoundSource::Status::Playing) {
                    jam::AudioManager::stopAll();
                    jam::AudioManager::playMusic("win_music");
                }
            }
            else {
                showKeyAdvice = true;
            }
        }
        else {
            showKeyAdvice = false;
        }
        auto backroomsMusic_status = jam::AudioManager::getMusic("backroomsAmbient_music")->getStatus();
        if (floor == FLOOR_BACKROOMS) {
            if (backroomsMusic_status != sf::SoundSource::Status::Playing) {
                jam::AudioManager::playMusic("backroomsAmbient_music");
            }   
        }
        else {
            if (backroomsMusic_status == sf::SoundSource::Status::Playing) {
                // backrooms fade out
                sf::Music* backroomsMusic = jam::AudioManager::getMusic("backroomsAmbient_music");
                int fadeOut = backroomsMusic->getVolume() - 0.5f > 0 ? backroomsMusic->getVolume() - 0.5f : 0;
                backroomsMusic->setVolume(fadeOut);
                if (fadeOut == 0) {
                    jam::AudioManager::stopMusic("backroomsAmbient_music");
                }
            }
        }
        // SCREAMERS
        if (floor == FLOOR_SCREAMER_MARIAELENA) {
            mariaelena_screamer.screamOnce();
        }
        mariaelena_screamer.update(delta);

        if (floor == FLOOR_SCREAMER_HUAYNA) {
            huayna_screamer.screamOnce();
        }
        huayna_screamer.update(delta);

        if (showKeyAdvice && !hasTheKey) {
            jam::UI::drawDialogText("Cerrado... necesitas una llave");
        }

        std::cout << "Player position: (" << this->camera.position.x << ", " << this->camera.position.y << ")" << std::endl;

        this->camera.renderIntoTexture(renderTexture);

        // MOVE THIS TO CAMERA
        item_fisiCat.update(this->camera);
        if (!hasTheKey) {
            item_keyToFind.update(this->camera);
        }
        renderTexture.draw(item_fisiCat);

        float distKey = magnitude(this->camera.position - item_keyToFind.position);
        float distCat = magnitude(this->camera.position - item_fisiCat.position);

        // Draw 
        renderTexture.draw(item_fisiCat);
        renderTexture.draw(item_keyToFind);

        // Draw screamer (if screaming)
        renderTexture.draw(mariaelena_screamer);
        renderTexture.draw(huayna_screamer);

        if (transitionTimer > 0) renderTexture.draw(blackScreen);
        this->renderTexture.display();

        if (g_gamePaused) {
            // if mouse is invisible
            m_window->setMouseCursorVisible(true);
            m_window->setMouseCursorGrabbed(false);
        }
        else {
            m_window->setMouseCursorVisible(false);
            m_window->setMouseCursorGrabbed(true);
            // Center mouse
            sf::Mouse::setPosition(previousMousePosition = windowCenter, *m_window);
        }
    }

    void jam::InsideFisi::dispatchEvent(const sf::Event &event) {
        if (event.type == sf::Event::MouseMoved) {
            if (g_gamePaused) return;
            sf::Vector2i now = { event.mouseMove.x, event.mouseMove.y };
            this->camera.moveCamera(this->camera.sensitivity * (now.x - previousMousePosition.x)); // HELP HERE
            previousMousePosition = now;
        }
    }
