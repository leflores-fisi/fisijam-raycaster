#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <vector>
#include <string>

namespace jam {

class AudioManager {
    inline static std::unordered_map<std::string, sf::SoundBuffer> soundBuffers = {};
    inline static std::unordered_map<std::string, sf::Sound> soundObjects = {};
    inline static std::unordered_map<std::string, sf::Music> musicObjects = {};

    inline static std::vector<sf::Sound *> playingSounds = {};
    inline static std::vector<sf::Music *> playingMusics = {}; // by reference because sf::Music is not copyable
public:
    // Explicitly disallow copying
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    static void loadSound(const std::string &filename, const std::string &name);
    static void playSound(const std::string &name, float pitch = 1.0f, float volume = 100.f);
    static void pauseSound(const std::string &name);
    static void stopSound(const std::string &name);
    static sf::Sound* getSound(const std::string &name);
    static void refreshSoundStatus();

    static void loadMusic(const std::string &filename, const std::string &name);
    static void playMusic(const std::string &name, float pitch = 1.0f, float volume = 100.f);
    static void pauseMusic(const std::string &name);
    static void stopMusic(const std::string &name);
    static sf::Music* getMusic(const std::string &name);

    static void pauseAll();
    static void stopAll();
    static void resumeAll();
private:
    ~AudioManager();
};

}
