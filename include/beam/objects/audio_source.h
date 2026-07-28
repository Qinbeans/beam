#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <string>

namespace beam {

/**
 * @brief A one-shot/fire-and-forget sound effect, wrapping raylib's Sound.
 *
 * The underlying ::Sound is loaded once per file path and cached on the
 * Manager (keyed by `path + "Sound"`), mirroring Sprite's texture-cache
 * pattern, so multiple AudioSources for the same file share one buffer.
 * Loading a Sound lazily starts raylib's audio device via
 * Manager::ensureAudioDevice() (see manager.h).
 */
class AudioSource : public GameObject {
private:
  Sound sound;
  float volume;
  float pitch;
  float pan;

  std::function<void(float, AudioSource *, SharedManager)> updateCallback;

public:
  /**
   * @brief Load (or reuse a cached) sound and construct an AudioSource for
   * it.
   * @param manager Manager used to cache the underlying Sound asset.
   * @param name Node name.
   * @param soundPath Path to a sound file (wav/ogg/mp3/flac/qoa, per
   *   raylib's LoadSound).
   * @param volume Initial volume (1.0 is max level).
   * @param pitch Initial pitch (1.0 is base level).
   * @param pan Initial stereo pan (0.5 is center).
   */
  AudioSource(SharedManager manager, const std::string &name,
              const std::string &soundPath, float volume = 1.0f,
              float pitch = 1.0f, float pan = 0.5f);

  /// @brief Invoke the update callback, if any. AudioSource otherwise needs
  /// no per-frame work (unlike MusicPlayer's streaming buffers).
  void update(float, SharedManager) override;

  /// @brief Start (or restart) playback.
  void play();
  /// @brief Stop playback.
  void stop();
  /// @brief Pause playback.
  void pause();
  /// @brief Resume a paused sound.
  void resume();
  /// @brief Whether the sound is currently playing.
  bool isPlaying() const;

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

  /// @brief Register a callback invoked at the end of every update().
  void onUpdate(std::function<void(float, AudioSource *, SharedManager)>);
};

} // namespace beam
