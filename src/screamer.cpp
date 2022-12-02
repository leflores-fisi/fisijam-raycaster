#include "include/screamer.hpp"
#include "include/AudioManager.hpp"

jam::Screamer::Screamer(std::string filename) :
    screamerSprite(sf::Sprite()),
    screamerTexture(sf::Texture()),
    isScreaming(false), ended(false)
{
    bool textureLoaded = this->screamerTexture.loadFromFile(filename);
    if (!textureLoaded) {
        throw std::runtime_error("No se pudo cargar el screamer " + filename);
    }
    this->screamerTexture.setSmooth(true);
    this->screamerSprite.setTexture(this->screamerTexture);
}

jam::Screamer::~Screamer() { }

void jam::Screamer::screamOnce() {
    if (this->ended || this->isScreaming) return;
    jam::AudioManager::playSound("screamer_sound");
    this->isScreaming = true;
}

void jam::Screamer::update(float delta) {
    if (this->isScreaming) {
        this->screamTime -= delta;
        if (this->screamTime < 0.f) {
            screamTime = 0.f;
            this->isScreaming = false;
            this->ended = true;
        }
        else {
            // zoom in centered
            sf::Vector2f scale = this->screamerSprite.getScale();
            scale.x += delta;
            scale.y += delta;

            this->screamerSprite.setScale(scale);

            this->screamerSprite.setScale(scale);
            this->screamerSprite.setPosition(
                this->screamerSprite.getPosition().x - delta/2,
                this->screamerSprite.getPosition().y - delta/2
            );
        }
    }
}
