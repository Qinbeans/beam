#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <string>

namespace beam {

/**
 * @brief A streamed music track, wrapping raylib's Music.
 *
 * Unlike AudioSource/Sound, Music streams from disk and needs
 * UpdateMusicStream() called every frame while playing to keep its buffers
 * filled - update() does this automatically. The underlying ::Music is
 * loaded once per file path and cached on the Manager (keyed by
 * `path + "Music"`), mirroring Sprite's texture-cache pattern. Loading a
 * Music lazily starts raylib's audio device via
 * Manager::ensureAudioDevice() (see manager.h).
 */
class MusicPlayer : public GameObject {
private:
  Music music;
  float volume;
  float pitch;
  float pan;

  std::function<void(float, MusicPlayer *, SharedManager)> updateCallback;

public:
  /**
   * @brief Load (or reuse a cached) music stream and construct a
   * MusicPlayer for it.
   * @param manager Manager used to cache the underlying Music asset.
   * @param name Node name.
   * @param musicPath Path to a music file (wav/ogg/mp3/flac/xm/mod, per
   *   raylib's LoadMusicStream).
   * @param looping Whether playback should loop (sets Music::looping).
   * @param volume Initial volume (1.0 is max level).
   * @param pitch Initial pitch (1.0 is base level).
   * @param pan Initial stereo pan (0.5 is center).
   */
  MusicPlayer(SharedManager manager, const std::string &name,
              const std::string &musicPath, bool looping = true,
              float volume = 1.0f, float pitch = 1.0f, float pan = 0.5f);

  /// @brief While playing, pump raylib's streaming buffers via
  /// UpdateMusicStream(), then invoke the update callback, if any.
  void update(float, SharedManager) override;

  /// @brief Start (or restart) playback.
  void play();
  /// @brief Stop playback.
  void stop();
  /// @brief Pause playback.
  void pause();
  /// @brief Resume paused playback.
  void resume();
  /// @brief Whether the music is currently playing.
  bool isPlaying() const;

  /// @brief Enable/disable looping.
  void setLooping(bool looping);
  /// @brief Whether looping is enabled.
  bool isLooping() const;

  /// @brief Set the playback volume (1.0 is max level).
  void setVolume(float volume);
  /// @brief Set the playback pitch (1.0 is base level).
  void setPitch(float pitch);
  /// @brief Set the stereo pan (0.5 is center).
  void setPan(float pan);

  /// @brief Get the current playback volume.
  float getVolume() const;
  /// @brief Get the current playback pitch.
  float getPitch() const;
  /// @brief Get the current stereo pan.
  float getPan() const;

  /// @brief Get how much of the track has played, in seconds.
  float getTimePlayed() const;
  /// @brief Get the total track length, in seconds.
  float getTimeLength() const;

  /// @brief Register a callback invoked at the end of every update().
  void onUpdate(std::function<void(float, MusicPlayer *, SharedManager)>);
};

} // namespace beam
