#include "audioSource.h"
#include "audio.h"

void AudioSource::play() const {
    if (!clip) return;
    Audio::playSound(clip->filePath, loop);
}

void AudioSource::play3D(const Transform& transform) const {
    if (!clip) return;
    Audio::playSound3D(clip->filePath,
                       transform.position.x,
                       transform.position.y,
                       transform.position.z,
                       loop);
}

void AudioSource::setLooping(bool shouldLoop) {
    loop = shouldLoop;
}

void AudioSource::setVolume(float vol) {
    volume = glm::clamp(vol, 0.0f, 1.0f);
}