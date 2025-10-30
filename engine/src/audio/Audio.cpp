#include "engine/audio/Audio.h"

#include "alc.h"
#include "engine/Log.h"
#include <cstring>

Audio::Audio() {
    buffer = 0;
    bitRate = 0;
    freqRate = 0;
    length = 0;
    data = NULL;
    size = 0;
    channels = 0;
}

Audio::~Audio(void) {
    if (data) {
        delete[] data;
        data = NULL;
    }
}

ALenum Audio::GetOALFormat() {
    if (GetBitRate() == 16) {
        return GetChannels() == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
    } else if (GetBitRate() == 8) {
        return GetChannels() == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
    }
    return AL_FORMAT_MONO8;
}

float Audio::GetLength() {
    return length;
}

void Audio::LoadFromWAV(string filename) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        SE_LOG_ERROR("OpenAL: Failed to open WAV file: {}", filename);
        return;
    }

    // Ler header RIFF
    char riffHeader[4];
    file.read(riffHeader, 4);
    if (strncmp(riffHeader, "RIFF", 4) != 0) {
        SE_LOG_ERROR("OpenAL: File is not a valid WAV file (missing RIFF): {}", filename);
        file.close();
        return;
    }

    // Ler tamanho do arquivo
    unsigned int fileSize;
    file.read((char*)&fileSize, 4);

    // Ler formato WAVE
    char waveHeader[4];
    file.read(waveHeader, 4);
    if (strncmp(waveHeader, "WAVE", 4) != 0) {
        SE_LOG_ERROR("OpenAL: File is not a valid WAV file (missing WAVE): {}", filename);
        file.close();
        return;
    }

    // Procurar por chunks
    bool foundFmt = false;
    bool foundData = false;

    while (!file.eof() && file.good()) {
        char chunkType[4];
        unsigned int chunkSize;

        file.read(chunkType, 4);
        if (file.gcount() != 4) break;

        file.read((char*)&chunkSize, 4);
        if (file.gcount() != 4) break;

        if (strncmp(chunkType, "fmt ", 4) == 0) {
            // Ler chunk fmt - deve ter pelo menos 16 bytes
            if (chunkSize < 16) {
                SE_LOG_ERROR("WAV fmt chunk too small: {} bytes", chunkSize);
                file.seekg(chunkSize, ios_base::cur);
                continue;
            }

            // Ler os campos do fmt chunk manualmente para evitar problemas de alinhamento
            unsigned short audioFormat;
            unsigned short numChannels;
            unsigned int sampleRate;
            unsigned int byteRate;
            unsigned short blockAlign;
            unsigned short bitsPerSample;

            file.read((char*)&audioFormat, 2);
            file.read((char*)&numChannels, 2);
            file.read((char*)&sampleRate, 4);
            file.read((char*)&byteRate, 4);
            file.read((char*)&blockAlign, 2);
            file.read((char*)&bitsPerSample, 2);

            // Verificar se é PCM
            if (audioFormat != 1) {
                SE_LOG_ERROR("WAV file is not PCM format (format={})", audioFormat);
                file.close();
                return;
            }

            bitRate = bitsPerSample;
            freqRate = (float)sampleRate;
            channels = numChannels;
            foundFmt = true;

            SE_LOG_INFO("WAV fmt chunk: format={}, channels={}, sampleRate={}, bitsPerSample={}",
                        audioFormat, numChannels, sampleRate, bitsPerSample);

            // Se o chunk for maior que 16 bytes (formato estendido), pular o resto
            if (chunkSize > 16) {
                file.seekg(chunkSize - 16, ios_base::cur);
            }
        }
        else if (strncmp(chunkType, "data", 4) == 0) {
            // Ler chunk de dados
            size = chunkSize;

            if (size > 0) {
                data = new char[size];
                file.read(data, size);

                if (file.gcount() != (streamsize)size) {
                    SE_LOG_ERROR("WAV data chunk: expected {} bytes but read {}", size, file.gcount());
                    delete[] data;
                    data = NULL;
                    size = 0;
                } else {
                    foundData = true;
                    SE_LOG_INFO("WAV data chunk: read {} bytes successfully", size);
                }
            }
        }
        else {
            // Chunk desconhecido, pular
            SE_LOG_INFO("WAV: Skipping unknown chunk: {}{}{}{} (size: {})",
                       chunkType[0], chunkType[1], chunkType[2], chunkType[3], chunkSize);
            file.seekg(chunkSize, ios_base::cur);
        }
    }

    file.close();

    if (!foundFmt) {
        SE_LOG_ERROR("WAV file missing fmt chunk: {}", filename);
        return;
    }

    if (!foundData) {
        SE_LOG_ERROR("WAV file missing data chunk: {}", filename);
        return;
    }

    // Calcular comprimento
    if (size > 0 && channels > 0 && freqRate > 0 && bitRate > 0) {
        length = (float)size / (channels * freqRate * (bitRate / 8.0f)) * 1000.0f;
        SE_LOG_INFO("WAV file loaded: {} - Duration: {:.2f}ms", filename, length);
    } else {
        SE_LOG_ERROR("Invalid WAV file parameters: size={}, channels={}, freq={}, bitrate={}",
                     size, channels, freqRate, bitRate);
        length = 0.0f;

        // Limpar dados inválidos
        if (data) {
            delete[] data;
            data = NULL;
        }
        size = 0;
    }
}

void Audio::LoadWAVChunkInfo(ifstream& file, string& name, unsigned int& size) {
    char chunk[4];
    file.read((char*)&chunk, 4);
    file.read((char*)&size, 4);

    name = string(chunk, 4);
}