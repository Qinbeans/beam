#include "beam/objects/audio_source.h"

namespace beam {

AudioSource::AudioSource(SharedManager manager, const std::string &name,
                          const std::string &soundPath, float volume,
                          float pitch, float pan)
    : GameObject(name), volume(volume), pitch(pitch), pan(pan) {
  std::string assetKey = soundPath + "Sound";
  if (!manager->hasAsset(assetKey)) {
    manager->setAsset<Sound>(assetKey, LoadSound(soundPath.c_str()));
  }
  sound = manager->getAsset<Sound>(assetKey);

  SetSoundVolume(sound, volume);
  SetSoundPitch(sound, pitch);
  SetSoundPan(sound, pan);
}

void AudioSource::update(float delta, SharedManager manager) {
  GameObject::update(delta, manager);
  if (updateCallback) {
    updateCallback(delta, this, manager);
  }
}

void AudioSource::play() { PlaySound(sound); }

void AudioSource::stop() { StopSound(sound); }

void AudioSource::pause() { PauseSound(sound); }

void AudioSource::resume() { ResumeSound(sound); }

bool AudioSource::isPlaying() const { return IsSoundPlaying(sound); }

void AudioSource::setVolume(float volume) {
  this->volume = volume;
  SetSoundVolume(sound, volume);
}

void AudioSource::setPitch(float pitch) {
  this->pitch = pitch;
  SetSoundPitch(sound, pitch);
}

void AudioSource::setPan(float pan) {
  this->pan = pan;
  SetSoundPan(sound, pan);
}

float AudioSource::getVolume() const { return volume; }

float AudioSource::getPitch() const { return pitch; }

float AudioSource::getPan() const { return pan; }

void AudioSource::onUpdate(
    std::function<void(float, AudioSource *, SharedManager)> callback) {
  updateCallback = callback;
}

} // namespace beam
