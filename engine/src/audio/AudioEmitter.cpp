#include <glm.hpp>

#include "engine/audio/AudioEmitter.h"

AudioEmitter::AudioEmitter(void) {
  Reset();
}

AudioEmitter::AudioEmitter(Audio *s) {
  Reset();
  SetAudio(s);
}

void AudioEmitter::Reset() {
  priority = AUDIOPRIORTY_LOW;
  volume = 1.0f;
  radius = 500.0f;
  timeLeft = 0.0f;
  isLooping = true;
  currentSource = NULL;
  audio = NULL;
  target = NULL;
  position = glm::vec3(0.0f);
}

AudioEmitter::~AudioEmitter(void) {
  DetachSource();
}

bool AudioEmitter::CompareNodesByPriority(AudioEmitter *a, AudioEmitter *b) {
  return (a->priority > b->priority) ? true : false;
}

void AudioEmitter::SetAudio(Audio *s) {
  audio = s;
  DetachSource();
  if (s) {
    timeLeft = s->GetLength();
  }
}

void AudioEmitter::AttachSource(OALSource* s) {
  currentSource = s;

  if (!currentSource || !audio) {
    return;
  }

  currentSource->inUse = true;

  alSourcef(currentSource->source, AL_MAX_DISTANCE, radius);
  alSourcef(currentSource->source, AL_REFERENCE_DISTANCE, radius * 0.2f);
  alSourcei(currentSource->source, AL_BUFFER, audio->GetBuffer());
  alSourcef(currentSource->source, AL_SEC_OFFSET, (audio->GetLength() / 1000.0f) - (timeLeft / 1000.0f));
  alSourcePlay(currentSource->source);
}

void AudioEmitter::DetachSource() {
  if (!currentSource) {
    return;
  }

  alSourcef(currentSource->source, AL_GAIN, 0.0f);
  alSourceStop(currentSource->source);
  alSourcei(currentSource->source, AL_BUFFER, 0);

  currentSource->inUse = false;
  currentSource = NULL;
}

void AudioEmitter::Update(float msec) {
  if (!audio) {
    return;
  }

  timeLeft -= msec;

  while (isLooping && timeLeft < 0.0f) {
    timeLeft += audio->GetLength();
  }

  if (currentSource) {
    glm::vec3 pos = this->position;

    alSourcefv(currentSource->source, AL_POSITION, (float*)&pos);
    alSourcef(currentSource->source, AL_GAIN, volume);
    alSourcei(currentSource->source, AL_LOOPING, isLooping ? 1 : 0);
    alSourcef(currentSource->source, AL_MAX_DISTANCE, radius);
    alSourcef(currentSource->source, AL_REFERENCE_DISTANCE, radius * 0.2f);
  }
}