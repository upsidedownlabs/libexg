#ifndef BIOAMP_EXG_PILL_H
#define BIOAMP_EXG_PILL_H

#define BUFFER_SIZE_EMG 128

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class LibEXG
{
public:
  // constructor
  LibEXG(bool displayMsg = false);

  // Methods
  float get_ECG(int INPUT_PIN);
  float get_EEG(int INPUT_PIN);
  float get_EOG(int INPUT_PIN);
  float get_EMG(int INPUT_PIN);

  float EEGFilter(float input);
  float EOGFilter(float input);
  float ECGFilter(float input);
  float EMGFilter(float input);

  int get_Envelop(int abs_emg);

private:
  int data_index_emg, sum_emg;
  int circular_buffer_emg[BUFFER_SIZE_EMG];
};

#endif
