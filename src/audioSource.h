#pragma once

#include <memory>
#include "transform.h"
#include "resourceManager.h"

class AudioSource {
public:
    std::shared_ptr<AudioClip> clip;
    bool loop = false;
    float volume = 1.0f;
    float pitch = 1.0f;

    AudioSource() = default;
    explicit AudioSource(const std::shared_ptr<AudioClip>& audioClip) : clip(audioClip) {}

    void play() const;
    void play3D(const Transform& transform) const;
    void setLooping(bool shouldLoop);
    void setVolume(float vol);
};