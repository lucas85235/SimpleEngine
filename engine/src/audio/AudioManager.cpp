#include "engine/audio/AudioManager.h"
#include "engine/Log.h"

map<string, Audio*> AudioManager::audios;

void AudioManager::AddAudio(string name) {

    if (!GetAudio(name)) {
        Audio* s = new Audio();

        string extension = name.substr(name.length() - 3, 3);

        if (extension == "wav") {
            s->LoadFromWAV(name);
            alGenBuffers(1, &s->buffer);

            ALenum err = alGetError();
            if (err != AL_NO_ERROR)
                SE_LOG_ERROR("alGenBuffers error: {}", err);

            // debug guard before alBufferData
            ALenum fmt = s->GetOALFormat();
            const void* dataptr = s->GetData();
            uint64_t usize = s->GetSize();
            unsigned int freq_u = s->GetFrequency();
            int channels = s->GetChannels();

            // Safety casts and checks
            const ALsizei size_si =
                (usize > static_cast<uint64_t>(std::numeric_limits<ALsizei>::max()))
                    ? -1
                    : static_cast<ALsizei>(usize);
            const ALsizei freq_si =
                (freq_u > static_cast<unsigned int>(std::numeric_limits<ALsizei>::max()))
                    ? -1
                    : static_cast<ALsizei>(freq_u);

            std::cerr << "[DEBUG] alBufferData about to be called:\n";
            std::cerr << "  buffer = " << s->buffer << "\n";
            std::cerr << "  format = " << fmt << "\n";
            std::cerr << "  data ptr = " << dataptr << "\n";
            std::cerr << "  size (bytes) = " << usize << " (ALsizei_cast=" << size_si << ")\n";
            std::cerr << "  freq = " << freq_u << " (ALsizei_cast=" << freq_si << ")\n";

            // basic validation
            if (!dataptr) {
                std::cerr << "[ERROR] dataptr is NULL - cannot call alBufferData\n";
            } else if (size_si <= 0) {
                std::cerr << "[ERROR] size is <= 0 or overflowed when cast to ALsizei\n";
            } else if (!(fmt == AL_FORMAT_MONO8 || fmt == AL_FORMAT_MONO16 ||
                         fmt == AL_FORMAT_STEREO8 || fmt == AL_FORMAT_STEREO16)) {
                std::cerr << "[ERROR] invalid AL format: " << fmt << "\n";
            } else if (freq_si <= 0) {
                std::cerr << "[ERROR] invalid sample rate: " << freq_u << "\n";
            } else {
                alBufferData(s->buffer, fmt, dataptr, size_si, freq_si);
                ALenum err = alGetError();
                if (err != AL_NO_ERROR) {
                    std::cerr << "[ERROR] alBufferData returned: " << err << "\n";
                } else {
                    std::cerr << "[OK] alBufferData success\n";
                }
            }

            alBufferData(s->buffer, s->GetOALFormat(), s->GetData(), (ALsizei)s->GetSize(),
                         (ALsizei)s->GetFrequency());

            err = alGetError();
            if (err != AL_NO_ERROR)
                SE_LOG_ERROR("alBufferData error: {} (file={})", err, name);
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
    for (map<string, Audio*>::iterator i = audios.begin(); i != audios.end(); ++i) {
        delete i->second;
    }
}
