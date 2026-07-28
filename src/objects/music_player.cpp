#include "beam/objects/music_player.h"

namespace beam {

MusicPlayer::MusicPlayer(SharedManager manager, const std::string &name,
                          const std::string &musicPath, bool looping,
                          float volume, float pitch, float pan)
    : GameObject(name), volume(volume), pitch(pitch), pan(pan) {
  std::string assetKey = musicPath + "Music";
  if (!manager->hasAsset(assetKey)) {
    manager->setAsset<Music>(assetKey, LoadMusicStream(musicPath.c_str()));
  }
  music = manager->getAsset<Music>(assetKey);
  music.looping = looping;

  SetMusicVolume(music, volume);
  SetMusicPitch(music, pitch);
  SetMusicPan(music, pan);
}

void MusicPlayer::update(float delta, SharedManager manager) {
  GameObject::update(delta, manager);
  if (!isActive())
    return;

  if (IsMusicStreamPlaying(music)) {
    UpdateMusicStream(music);
  }

  if (updateCallback) {
    updateCallback(delta, this, manager);
  }
}

void MusicPlayer::play() { PlayMusicStream(music); }

void MusicPlayer::stop() { StopMusicStream(music); }

void MusicPlayer::pause() { PauseMusicStream(music); }

void MusicPlayer::resume() { ResumeMusicStream(music); }

bool MusicPlayer::isPlaying() const { return IsMusicStreamPlaying(music); }

void MusicPlayer::setLooping(bool looping) { music.looping = looping; }

bool MusicPlayer::isLooping() const { return music.looping; }

void MusicPlayer::setVolume(float volume) {
  this->volume = volume;
  SetMusicVolume(music, volume);
}

void MusicPlayer::setPitch(float pitch) {
  this->pitch = pitch;
  SetMusicPitch(music, pitch);
}

void MusicPlayer::setPan(float pan) {
  this->pan = pan;
  SetMusicPan(music, pan);
}

float MusicPlayer::getVolume() const { return volume; }

float MusicPlayer::getPitch() const { return pitch; }

float MusicPlayer::getPan() const { return pan; }

float MusicPlayer::getTimePlayed() const { return GetMusicTimePlayed(music); }

float MusicPlayer::getTimeLength() const { return GetMusicTimeLength(music); }

void MusicPlayer::onUpdate(
    std::function<void(float, MusicPlayer *, SharedManager)> callback) {
  updateCallback = callback;
}

} // namespace beam
