// LibEXG
// https://github.com/upsidedownlabs/libexg

// Upside Down Labs invests time and resources providing this open source code,
// please support Upside Down Labs and open-source hardware by purchasing
// products from Upside Down Labs!

// Copyright (c) Chinmay Lonkar chinmay20220@gmail.com
// Copyright (c) 2021 Moteen Shah moteenshah.02@gmail.com
// Copyright (c) 2021 Upside Down Labs - contact@upsidedownlabs.tech

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


/*
   Copyright 2019, OYMotion Inc.
   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
   COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
   OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
   THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
   DAMAGE.
*/

#include "Filters.h"

#define _DEBUG 0

#define SerialToUSB Serial

// Define number of ADC pins to be use
#define NUM_CHANNELS 2

#define ARR_SIZE(a) (sizeof(a) / sizeof(a[0]))

// Cycle buffer
typedef struct
{
  uint8_t index;
  uint16_t buf[64]; /* Buffer for rectified AC value */
  uint32_t sum;     /* Sum for fast caculation of mean value */
} CycleBuf_t;

// Append to cycle buffer
#define CYCLE_BUF_ADD(cb, val)                    \
  {                                               \
    cb.sum -= cb.buf[cb.index];                   \
    cb.sum += (val);                              \
    cb.buf[cb.index] = (val);                     \
    cb.index = (cb.index + 1) % ARR_SIZE(cb.buf); \
  }

/* Get mean value of cycle buffer */
#define CYCLE_BUF_MEAN(cb) (cb.sum / ARR_SIZE(cb.buf))

CycleBuf_t rectifiedAcBuf[NUM_CHANNELS];

// IMPORTANT NOTE: Please unplug any power adapters when using sEMG sensors.
// Only battery powered systems an be connected directly/indirectly to sEMG
// sensors.
//
// Connect sensor to arduino board then compile & upload this sketch.
// When running, open SerilaPlot window (Menu->Tools->Serial Plotter) then
// you can see the curves.

EMGFilters myFilter[NUM_CHANNELS];

// Set the input frequency.
//
// The filters work only with fixed sample frequency of
// `SAMPLE_FREQ_500HZ` or `SAMPLE_FREQ_1000HZ`.
// Inputs at other sample rates will bypass

// Note that sample rate cannot be less than 500Hz for EMG signal 
SAMPLE_FREQUENCY sampleRate = SAMPLE_FREQ_500HZ;

// Time interval for processing the input signal.
unsigned long long interval = 1000000ul / sampleRate;

// Set the frequency of power line hum to filter out.
//
// For countries with 60Hz power line, change to "NOTCH_FREQ_60HZ"
NOTCH_FREQUENCY humFreq = NOTCH_FREQ_50HZ;

void setup()
{
  /* add setup code here */

  // initialization
  myFilter[0].init(sampleRate, humFreq, EMG_MODE, A0, true, true, true);
  myFilter[1].init(sampleRate, humFreq, ECG_MODE, A1, true, true, true);

  for (int i = 0; i < NUM_CHANNELS; i++)
  {

    rectifiedAcBuf[i].sum = 0;
    rectifiedAcBuf[i].index = 0;

    for (int j = 0; j < ARR_SIZE(rectifiedAcBuf[i].buf); j++)
    {
      rectifiedAcBuf[i].buf[j] = 0;
    }
  }

  // open serial
  SerialToUSB.begin(115200);
}

void loop()
{
  // Note: `micros()` will overflow and reset every about 70 minutes.
  unsigned long long timeStamp = micros();

  // filter processing
  int data = 0, dataAfterFilter = 0;

  for (int i = 0; i < NUM_CHANNELS; i++)
  {
    data = analogRead(myFilter[i].m_input_pin);
    dataAfterFilter = myFilter[i].update(data);

    // Rectification
    // CYCLE_BUF_ADD(rectifiedAcBuf[i], abs(dataAfterFilter));

    // Simple envelope calculation, use 2 * rectified value
    // uint16_t envelope = CYCLE_BUF_MEAN(rectifiedAcBuf[i]) * 2;

#if !_DEBUG
    SerialToUSB.print(dataAfterFilter); // Draw offset = 128
    SerialToUSB.print(" ");
    // SerialToUSB.print(envelope);
    // SerialToUSB.print(" ");
#endif
  }

  unsigned long timeElapsed = micros() - timeStamp;

#if _DEBUG
  SerialToUSB.print("Filters cost time: ");
  SerialToUSB.println(timeElapsed);
#else
  if (interval > timeElapsed)
  {
    delay((interval - timeElapsed) / 1000);
  }
#endif

  SerialToUSB.println();
}