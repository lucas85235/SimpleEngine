#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <al.h>

using namespace std;

struct FMTCHUNK {
  short format;
  short channels;
  unsigned long srate;
  unsigned long bps;
  short balign;
  short samp;
};

class Audio {
  friend class AudioManager ;
public :
  char* GetData () { return data; }
  int GetBitRate () { return bitRate; }
  float GetFrequency () { return freqRate; }
  int GetChannels () { return channels; }
  int GetSize () { return size; }
  ALuint GetBuffer () { return buffer; }

  ALenum GetOALFormat ();
  float GetLength ();

protected :
  Audio ();
  ~Audio (void);

  void LoadFromWAV (string filename);
  void LoadWAVChunkInfo (ifstream &file, string &name, unsigned int &size);

  char* data;

  float length;
  int bitRate;
  float freqRate;
  int size;
  int channels;

  ALuint buffer;
};