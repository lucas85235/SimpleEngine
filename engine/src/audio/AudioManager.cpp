#include "engine/audio/AudioManager.h"
#include "engine/Log.h"

map<string, Audio*> AudioManager::audios;

void AudioManager::AddAudio(string name) {

  if (!GetAudio(name)) {
    Audio* s = new Audio();

    string extension = name.substr(name.length() -3, 3);

    if (extension == "wav") {
      s->LoadFromWAV(name);
      alGenBuffers(1, &s->buffer);
      alBufferData(s->buffer, s->GetOALFormat(), s->GetData(), s->GetSize(), (ALsizei) s->GetFrequency());
    } else {
      SE_LOG_WARN("OpenAL: Invalid extension!");
    }
    audios.insert(make_pair(name, s));
  }
}

Audio* AudioManager::GetAudio(string name) {
  map<string, Audio*>::iterator s = audios.find(name);
  return (s != audios.end() ? s->second : NULL);
}

void AudioManager::DeleteAudio() {
  for ( map<string, Audio*>::iterator i = audios.begin(); i != audios.end(); ++ i) {
    delete i->second ;
  }
}
