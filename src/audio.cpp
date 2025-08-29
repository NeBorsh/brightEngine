#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "audio.h"

#include <iostream>
#include <algorithm>

ma_engine Audio::engine = {};
std::vector<Audio::ActiveSound> Audio::activeSounds;

bool Audio::init() {
    ma_result result = ma_engine_init(nullptr, &engine);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to initialize audio engine: " << result << std::endl;
        return false;
    }
    std::cout << "miniaudio initialized successfully." << std::endl;
    return true;
}

void Audio::shutdown() {
    for (auto& s : activeSounds) {
        if (s.sound) {
            ma_sound_stop(s.sound.get());
            ma_sound_uninit(s.sound.get());
        }
    }
    activeSounds.clear();
    ma_engine_uninit(&engine);
}

bool Audio::playSound(const std::string& filePath, bool loop) {
    auto sound = std::make_unique<ma_sound>();
    ma_result result = ma_sound_init_from_file(&engine, filePath.c_str(),
                                               loop ? MA_SOUND_FLAG_LOOPING : 0,
                                               nullptr, nullptr, sound.get());
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to load sound: " << filePath << " | Error: " << result << std::endl;
        return false;
    }

    ma_sound_start(sound.get());

    activeSounds.push_back({
        std::move(sound),
        false,
        loop,
        0, 0, 0
    });

    return true;
}

bool Audio::playSound3D(const std::string& filePath, float x, float y, float z, bool loop) {
    auto sound = std::make_unique<ma_sound>();
    ma_result result = ma_sound_init_from_file(&engine, filePath.c_str(),
                                               loop ? MA_SOUND_FLAG_LOOPING : 0,
                                               nullptr, nullptr, sound.get());
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to load 3D sound: " << filePath << " | Error: " << result << std::endl;
        return false;
    }

    ma_sound_set_position(sound.get(), x, y, z);
    ma_sound_set_volume(sound.get(), 1.0f);
    ma_sound_start(sound.get());

    activeSounds.push_back({
        std::move(sound),
        true,
        loop,
        x, y, z
    });

    return true;
}

void Audio::updateListener(float posX, float posY, float posZ,
                           float velX, float velY, float velZ,
                           float atX,  float atY,  float atZ,
                           float upX,  float upY,  float upZ)
{
    ma_engine_listener_set_position(&engine, 0, posX, posY, posZ);
    ma_engine_listener_set_velocity(&engine, 0, velX, velY, velZ);
    ma_engine_listener_set_direction(&engine, 0, atX, atY, atZ);
    ma_engine_listener_set_world_up(&engine, 0, upX, upY, upZ);
}

void Audio::update() {
    removeStoppedSounds();
}

void Audio::removeStoppedSounds() {
    auto it = std::remove_if(activeSounds.begin(), activeSounds.end(),
        [](const ActiveSound& s) {
            if (s.loop) return false;
            if (s.sound && !ma_sound_is_playing(s.sound.get())) {
                ma_sound_uninit(s.sound.get());
                return true;
            }
            return false;
        });
    activeSounds.erase(it, activeSounds.end());
}