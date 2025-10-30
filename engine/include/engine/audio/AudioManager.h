#pragma once

#include "engine/audio/Audio.h"
#include <map>

using std::map;

class AudioManager {

public:
  static void AddAudio(string name);
  static Audio* GetAudio(string name);

  static void DeleteAudio();

protected:
  AudioManager(void);
  ~AudioManager(void);

  static map<string, Audio*> audios;
};