#pragma once

#include <string>
#include <vector>
#include <memory>

struct ma_engine;
struct ma_sound;

class Audio {
public:
    static bool init();
    static void shutdown();
    static bool playSound(const std::string& filePath, bool loop = false);
    static bool playSound3D(const std::string& filePath, float x, float y, float z, bool loop = false);
    static void updateListener(float posX, float posY, float posZ,
                               float velX, float velY, float velZ,
                               float atX,  float atY,  float atZ,
                               float upX,  float upY,  float upZ);

    static void update();

private:
    static ma_engine engine;
    struct ActiveSound {
        std::unique_ptr<ma_sound> sound;
        bool is3D;
        bool loop;
        float x, y, z;
    };

    static std::vector<ActiveSound> activeSounds;
    static void removeStoppedSounds();
};