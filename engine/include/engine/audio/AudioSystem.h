#pragma once

#include <vector>
#include <algorithm>
#include "Audio.h"
#include "engine/audio/AudioEmitter.h"
#include "engine/ecs/Entity.h"
#include "engine/ecs/Components.h"

#include "al.h"
#include "alc.h"

using std::vector;

class AudioEmitter;

struct OALSource {
  ALuint source;
  bool inUse;

  OALSource(ALuint src) {
    source = src;
    inUse = false;
  }
};

class AudioSystem {
public:
  static void Initialise(unsigned int channels = 32) {
    if (!instance) {
      instance = new AudioSystem(channels);
    }
  }

  static void Destroy() { delete instance; }

  inline static AudioSystem* GetAudioSystem() { return instance; }

  void SetListenerTransform(const glm::vec3 &position) {
    listenerPosition = position;
  }

  void SetListenerForward(const glm::vec3 &forward)
  {
    listenerForward = forward;
  }

  void SetListenerUp(const glm::vec3 &up)
  {
    listenerUp = up;
  }

  glm::vec3 GetListenerTransform() {
    return listenerPosition;
  }

  void AddAudioEmitter(AudioEmitter* s) { emitters.push_back(s); }
  void RemoveAudioEmitter(AudioEmitter* s);

  void Update(float msec);

  void SetMasterVolume(float value);

  ALCdevice* GetDevice();

protected:

  AudioSystem(unsigned int channels = 32);
  ~AudioSystem(void);

  void UpdateListener();
  void UpdateTemporaryEmitters(float msec);

  void DetachSources(vector<AudioEmitter*>::iterator from, vector<AudioEmitter*>::iterator to);
  void AttachSources(vector<AudioEmitter*>::iterator from, vector<AudioEmitter*>::iterator to);

  void CullNodes();
  OALSource* GetSource();

  glm::vec3 listenerPosition;
  glm::vec3 listenerForward;
  glm::vec3 listenerUp;
  float masterVolume;
  ALCcontext* context;
  ALCdevice* device;
  se::Entity* listener;

  vector<OALSource*> sources;
  vector<AudioEmitter*> emitters;
  vector<AudioEmitter*> frameEmitters;

  static AudioSystem* instance;
};