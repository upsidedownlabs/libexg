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


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "ECG.h"
#include "EMG.h"
#include "EEG.h"
#include "EOG.h"
#include "AHF.h"

#ifndef _FILTERS_H
#define _FILTERS_H

enum FILTER_TYPE
{
    FILTER_TYPE_LOWPASS = 0,
    FILTER_TYPE_HIGHPASS,
};

enum NOTCH_FREQUENCY
{
  NOTCH_FREQ_50HZ = 50,
  NOTCH_FREQ_60HZ = 60
};

enum SAMPLE_FREQUENCY
{
  SAMPLE_FREQ_100HZ = 100,
  SAMPLE_FREQ_250HZ = 250,
  SAMPLE_FREQ_500HZ = 500,
  SAMPLE_FREQ_1000HZ = 1000
};
enum BIO_POTENTIAL_MODE
{
  EMG_MODE = 1,
  EOG_MODE = 2,
  EEG_MODE = 3,
  ECG_MODE = 4
};
class EMGFilters
{
public:
  void init(SAMPLE_FREQUENCY sampleFreq,
            NOTCH_FREQUENCY notchFreq,
            BIO_POTENTIAL_MODE mode,
            int INPUT_PIN,
            bool enableNotchFilter = true,
            bool enableLowpassFilter = true,
            bool enableHighpassFilter = true);

  // \brief Called in the loop, input read analog value to get filtered
  // value
  int update(int inputValue);
  int m_input_pin;

private:
  SAMPLE_FREQUENCY m_sampleFreq;
  NOTCH_FREQUENCY m_notchFreq;
  int m_mode;
  bool m_bypassEnabled;
  bool m_notchFilterEnabled;
  bool m_lowpassFilterEnabled;
  bool m_highpassFilterEnabled;
};

#endif
