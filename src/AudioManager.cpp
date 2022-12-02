#include "include/AudioManager.hpp"
#include <iostream>

jam::AudioManager::~AudioManager() { }

// --- Load audio files
void jam::AudioManager::loadSound(const std::string &filename, const std::string &name) {
    bool loaded = soundBuffers[name].loadFromFile(filename);
    if (!loaded) {
        throw std::runtime_error("No se pudo cargar el archivo de sonido " + filename);
    }
}
void jam::AudioManager::loadMusic(const std::string &filename, const std::string &name) {
    bool loaded = musicObjects[name].openFromFile(filename);
    if (!loaded) {
        throw std::runtime_error("No se pudo cargar el archivo de musica " + filename);
    }
}

// --- Play audios
void jam::AudioManager::playSound(const std::string &name, float pitch, float volume) {
    // std::cout << "Playing sound " << name << ": " << playingSounds.size() << std::endl;
    playingSounds.push_back(new sf::Sound(soundBuffers.at(name)));
    sf::Sound* sound = playingSounds.back();
    sound->setPitch(pitch);
    sound->setVolume(volume);
    sound->play();
    // std::cout << "New size of sounds:" << playingSounds.size() << std::endl;
}
void jam::AudioManager::playMusic(const std::string &name, float pitch, float volume) {
    sf::Music *music = &musicObjects.at(name);
    playingMusics.push_back(music);
    music->setPitch(pitch);
    music->setVolume(volume);
    music->play();
}

// --- Pause audios
void jam::AudioManager::pauseSound(const std::string &name) {
    sf::SoundBuffer &bufferToFind = soundBuffers.at(name);
    for (auto &sound : playingSounds) {
        if (sound->getBuffer() == &bufferToFind) {
            sound->pause();
        }
    }
}
void jam::AudioManager::pauseMusic(const std::string &name) {
    sf::Music &musicToFind = musicObjects.at(name);
    for (auto &music : playingMusics) {
        if (music == &musicToFind) {
            music->pause();
        }
    }
}

// --- Stop audios
void jam::AudioManager::stopSound(const std::string &name) {
    for (auto it = playingSounds.begin(); it != playingSounds.end(); ++it) {
        if ((*it)->getBuffer() == &soundBuffers[name]) {
            (*it)->stop();
            delete *it;
            playingSounds.erase(it);
            break;
        }
    }
}
void jam::AudioManager::stopMusic(const std::string &name) {
    for (auto it = playingMusics.begin(); it != playingMusics.end(); ++it) {
        if ((*it) == &musicObjects[name]) {
            (*it)->stop();
            playingMusics.erase(it);
            break;
        }
    }
}

// --- Get audios
sf::Sound* jam::AudioManager::getSound(const std::string &name) {
    for (const auto &sound : playingSounds) {
        if (sound->getBuffer() == &soundBuffers[name]) {
            return sound;
        }
    }
    return nullptr;
}
sf::Music* jam::AudioManager::getMusic(const std::string &name) {
    for (auto &music : musicObjects) {
        if (music.first == name) {
            return &music.second;
        }
    }
    return nullptr;
}

void jam::AudioManager::refreshSoundStatus() {
    // // std::cout << "Sounds before: " << playingSounds.size() << std::endl;
    // // std::cout << "Music before: " << playingMusics.size() << std::endl;
    // std::cout << "Deleting sounds" << std::endl;
    size_t i = 0;
    for (const auto &sound : playingSounds) {
        if (sound->getStatus() == sf::Sound::Stopped) {
            delete sound;
            playingSounds.erase(playingSounds.begin() + i);
        }
        i++;
    }
    // std::cout << "Deleting music" << std::endl;
    i = 0;
    for (const auto &music : playingMusics) {
        if (music->getStatus() == sf::Music::Stopped) {
            playingMusics.erase(playingMusics.begin() + i);
        }
        i++;
    }
    // std::cout << "Playing sounds: " << playingSounds.size() << std::endl;
    // std::cout << "Playing musics: " << playingMusics.size() << std::endl;
}

void jam::AudioManager::pauseAll() {
    for (const auto &music : playingMusics) music->pause();
    for (const auto &sound : playingSounds) sound->pause();
}
void jam::AudioManager::resumeAll() {
    for (const auto &music : playingMusics) {
        if (music->getStatus() == sf::Music::Paused) {
            music->play();
        }
    }
    for (const auto &sound : playingSounds) {
        if (sound->getStatus() == sf::Sound::Paused) {
            sound->play();
        }
    }
}
void jam::AudioManager::stopAll() {
    for (const auto &music : playingMusics) music->stop();
    for (const auto &sound : playingSounds) {
        sound->stop();
        delete sound;
    }

    playingMusics.clear();
    playingSounds.clear();
    // std::cout << "All sounds and musics stopped" << std::endl;
}
