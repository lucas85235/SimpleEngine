#include <glm.hpp>

#include "engine/audio/AudioSystem.h"
#include "engine/ecs/Components.h"
#include "engine/Log.h"

AudioSystem* AudioSystem::instance = NULL;

AudioSystem::AudioSystem(unsigned int channels)
{
    listener = NULL;
    masterVolume = 1.0f;
    listenerPosition = glm::vec3(0.0f);

    SE_LOG_INFO("OpenAL: Creating AudioSystem");

    device = alcOpenDevice(NULL);

    if (!device)
    {
        SE_LOG_ERROR("OpenAL: AudioSystem creation failed! No valid device!");
        return;
    }

    ALboolean enumeration;
    enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");

    if (enumeration == AL_FALSE)
    {
        SE_LOG_ERROR("OpenAL: Enumeration extension not available!");
    }
    else if (enumeration == AL_TRUE)
    {
        const ALCchar* deviceList = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
        if (deviceList)
        {
            SE_LOG_INFO("OpenAL: Available devices: {}", deviceList);
        }
    }

    context = alcCreateContext(device, NULL);
    if (!context)
    {
        SE_LOG_ERROR("OpenAL: Failed to create context!");
        alcCloseDevice(device);
        return;
    }

    alcMakeContextCurrent(context);

    alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);

    for (unsigned int i = 0; i < channels; ++i)
    {
        ALuint source;

        alGenSources(1, &source);
        ALenum error = alGetError();

        if (error == AL_NO_ERROR)
        {
            sources.push_back(new OALSource(source));
        }
        else
        {
            SE_LOG_WARN("OpenAL: Could not create source {}, error: {}", i, error);
            break;
        }
    }

    SE_LOG_INFO("OpenAL: AudioSystem has {} channels available!", sources.size());
}

AudioSystem::~AudioSystem(void)
{
    for (vector<OALSource*>::iterator i = sources.begin(); i != sources.end(); ++i)
    {
        alDeleteSources(1, &(*i)->source);
        delete(*i);
    }
    sources.clear();

    alcMakeContextCurrent(NULL);

    if (context)
    {
        alcDestroyContext(context);
    }

    if (device)
    {
        alcCloseDevice(device);
    }
}

void AudioSystem::SetMasterVolume(float value)
{
    value = max(0.0f, value);
    value = min(1.0f, value);
    masterVolume = value;
    alListenerf(AL_GAIN, masterVolume);
}

ALCdevice* AudioSystem::GetDevice()
{
    return device;
}

void AudioSystem::RemoveAudioEmitter(AudioEmitter* s)
{
    for (vector<AudioEmitter*>::iterator i = emitters.begin(); i != emitters.end(); ++i)
    {
        if ((*i) == s)
        {
            (*i)->DetachSource();
            emitters.erase(i);
            return;
        }
    }
}

void AudioSystem::UpdateListener()
{
    glm::vec3 worldPos = listenerPosition;

    glm::vec3 dirup[2];
    // Forward direction (assuming looking down negative Z)
    dirup[0] = glm::vec3(0.0f, 0.0f, -1.0f);
    // Up direction
    dirup[1] = glm::vec3(0.0f, 1.0f, 0.0f);

    alListenerfv(AL_POSITION, (float*)&worldPos);
    alListenerfv(AL_ORIENTATION, (float*)&dirup);
}

void AudioSystem::Update(float msec)
{
    frameEmitters.clear();

    for (vector<AudioEmitter*>::iterator i = emitters.begin(); i != emitters.end(); ++i)
    {
        (*i)->Update(msec);
        frameEmitters.push_back((*i));
    }

    UpdateListener();

    CullNodes();

    if (frameEmitters.size() > sources.size())
    {
        std::sort(frameEmitters.begin(), frameEmitters.end(), AudioEmitter::CompareNodesByPriority);

        DetachSources(frameEmitters.begin() + sources.size(), frameEmitters.end());
        AttachSources(frameEmitters.begin(), frameEmitters.begin() + sources.size());
    }
    else
    {
        AttachSources(frameEmitters.begin(), frameEmitters.end());
    }
}

void AudioSystem::CullNodes()
{
    for (vector<AudioEmitter*>::iterator i = frameEmitters.begin(); i != frameEmitters.end();)
    {
        AudioEmitter* e = (*i);

        if (!e->GetAudio())
        {
            e->DetachSource();
            i = frameEmitters.erase(i);
            continue;
        }

        float distance = glm::distance(listenerPosition, e->position);

        if (distance > e->GetRadius())
        {
            e->DetachSource();
            i = frameEmitters.erase(i);
        }
        else if (!e->GetLooping() && e->GetTimeLeft() <= 0.0f)
        {
            e->DetachSource();
            i = frameEmitters.erase(i);
        }
        else
        {
            ++i;
        }
    }
}

void AudioSystem::DetachSources(vector<AudioEmitter*>::iterator from, vector<AudioEmitter*>::iterator to)
{
    for (vector<AudioEmitter*>::iterator i = from; i != to; ++i)
    {
        (*i)->DetachSource();
    }
}

void AudioSystem::AttachSources(vector<AudioEmitter*>::iterator from, vector<AudioEmitter*>::iterator to)
{
    for (vector<AudioEmitter*>::iterator i = from; i != to; ++i)
    {
        if (!(*i)->GetSource())
        {
            (*i)->AttachSource(GetSource());
        }
    }
}

OALSource* AudioSystem::GetSource()
{
    for (vector<OALSource*>::iterator i = sources.begin(); i != sources.end(); ++i)
    {
        OALSource* s = *i;
        if (!s->inUse)
        {
            return s;
        }
    }
    return NULL;
}