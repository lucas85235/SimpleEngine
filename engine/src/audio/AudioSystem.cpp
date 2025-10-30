#include <glm.hpp>

#include "engine/audio/AudioSystem.h"
#include "engine/ecs/Components.h"
#include "engine/Log.h"

AudioSystem* AudioSystem::instance = NULL;

AudioSystem::AudioSystem(unsigned int channels)
{
    listener = NULL;
    masterVolume = 1.0f;

    SE_LOG_INFO("OpenAL: Creating AudioSystem");

    device = alcOpenDevice(NULL);
    const ALCchar* devices = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);

    if (!device)
    {
        SE_LOG_ERROR("OpenAL: AudioSystem creation failed! No valid device!");
        return;
    }

    ALboolean enumeration;
    enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");

    if (enumeration == AL_FALSE)
    {
        SE_LOG_ERROR("OpenAL: AudioSystem creation failed! No valid device!");
        return;
    }
    else if (enumeration == AL_TRUE)
    {
        const ALCchar* deviceList = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
        SE_LOG_INFO("OpenAL: device list: {}", deviceList);
    }

    context = alcCreateContext(device, NULL);
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
            break;
        }
    }

    SE_LOG_INFO("OpenAL: AudioSystem has: {} channels available!", sources.size());
}

AudioSystem::~AudioSystem(void)
{
    alcMakeContextCurrent(NULL);
    for (vector<OALSource*>::iterator i = sources.begin(); i != sources.end(); ++i)
    {
        alDeleteSources(1, &(*i)->source);
        delete(*i);
    }
    alcDestroyContext(context);
    alcCloseDevice(device);
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

void AudioSystem::UpdateListener()
{
    if (listener)
    {
        glm::vec3 worldPos = listenerPosition;

        glm::vec3 dirup[2];
        // forward
        dirup[0].x = -listenerPosition.x;
        dirup[0].y = -listenerPosition.y;
        dirup[0].z = -listenerPosition.z;
        // Up
        dirup[1].x = listenerPosition.x;
        dirup[1].y = listenerPosition.y;
        dirup[1].z = listenerPosition.z;

        alListenerfv(AL_POSITION, (float*)&worldPos);
        alListenerfv(AL_ORIENTATION, (float*)&dirup);
    }
}

void AudioSystem::Update(float msec)
{
    for (vector<AudioEmitter*>::iterator i = emitters.begin(); i != emitters.end(); ++i)
    {
        frameEmitters.push_back((*i));
        (*i)->Update(msec);
    }
    UpdateListener();

    // CullNodes();
    if (frameEmitters.size() > sources.size())
    {
        std::sort(frameEmitters.begin(), frameEmitters.end(), AudioEmitter::CompareNodesByPriority);

        DetachSources(frameEmitters.begin() + (sources.size() + 1), frameEmitters.end());
        AttachSources(frameEmitters.begin(), frameEmitters.begin() + sources.size());
    }
    else
    {
    }
    AttachSources(frameEmitters.begin(), frameEmitters.end());

    frameEmitters.clear();
}

void AudioSystem::CullNodes()
{
    for (vector<AudioEmitter*>::iterator i = frameEmitters.begin(); i != frameEmitters.end();)
    {
        AudioEmitter* e = (*i);

        float distance = glm::distance(listenerPosition, e->position);

        if (distance > e->GetRadius() || !e->GetAudio() || e->GetTimeLeft() < 0)
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
