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

enum FILTER_MODE
{
    EMG_FILTER = 1,
    EOG_FILTER = 2,
    EEG_FILTER = 3,
    ECG_FILTER = 4
};
ECG_filter ecg[2];
EMG_filter emg[2];
EEG_filter eeg[2];
EOG_filter eog[2];
AH_filter AHF[4];

void EMGFilters::init(SAMPLE_FREQUENCY sampleFreq,
                      NOTCH_FREQUENCY notchFreq,
                      BIO_POTENTIAL_MODE mode,
                      int INPUT_PIN,
                      bool enableNotchFilter,
                      bool enableLowpassFilter,
                      bool enableHighpassFilter)
{
    m_sampleFreq = sampleFreq;
    m_input_pin = INPUT_PIN;
    m_notchFreq = notchFreq;
    m_mode = mode;
    m_bypassEnabled = true;
    if (((sampleFreq == SAMPLE_FREQ_500HZ) ||
         (sampleFreq == SAMPLE_FREQ_1000HZ)) &&
        ((notchFreq == NOTCH_FREQ_50HZ) || (notchFreq == NOTCH_FREQ_60HZ)))
    {
        m_bypassEnabled = false;
    }
    if (m_mode == EMG_MODE)
    {
        emg[0].init(FILTER_TYPE_LOWPASS, m_sampleFreq);
        emg[1].init(FILTER_TYPE_HIGHPASS, m_sampleFreq);
        AHF[0].init(m_sampleFreq, m_notchFreq);
    }
    else if (m_mode == EOG_MODE)
    {
        eog[0].init(FILTER_TYPE_LOWPASS, m_sampleFreq);
        eog[1].init(FILTER_TYPE_HIGHPASS, m_sampleFreq);
        AHF[1].init(m_sampleFreq, m_notchFreq);
    }
    else if (m_mode == EEG_MODE)
    {
        eeg[0].init(FILTER_TYPE_LOWPASS, m_sampleFreq);
        eeg[1].init(FILTER_TYPE_HIGHPASS, m_sampleFreq);
        AHF[2].init(m_sampleFreq, m_notchFreq);
    }
    else if (m_mode == ECG_MODE)
    {
        ecg[0].init(FILTER_TYPE_LOWPASS, m_sampleFreq);
        ecg[1].init(FILTER_TYPE_HIGHPASS, m_sampleFreq);
        AHF[3].init(m_sampleFreq, m_notchFreq);
    }

    m_notchFilterEnabled = enableNotchFilter;
    m_lowpassFilterEnabled = enableLowpassFilter;
    m_highpassFilterEnabled = enableHighpassFilter;
}

int EMGFilters::update(int inputValue)
{
    int output = 0;
    if (m_bypassEnabled)
    {
        return output = inputValue;
    }

    // first notch filter
    if (m_notchFilterEnabled)
    {
        if (m_mode == EMG_MODE)
        {
            output = AHF[0].update(inputValue);
        }
        else if (m_mode == EOG_MODE)
        {
            output = AHF[1].update(inputValue);
        }
        else if (m_mode == EEG_MODE)
        {
            output = AHF[2].update(inputValue);
        }
        else if (m_mode == ECG_MODE)
        {
            output = AHF[3].update(inputValue);
        }
    }
    else
    {
        // notch filter bypass
        output = inputValue;
    }

    // second low pass filter
    if (m_lowpassFilterEnabled)
    {
        if (m_mode == EMG_MODE)
        {
            output = emg[0].update(output);
        }
        else if (m_mode == EOG_MODE)
        {
            output = eog[0].update(output);
        }
        else if (m_mode == EEG_MODE)
        {
            output = eeg[0].update(output);
        }
        else if (m_mode == ECG_MODE)
        {
            output = ecg[0].update(output);
        }
    }

    // third high pass filter
    if (m_highpassFilterEnabled)
    {
        if (m_mode == EMG_MODE)
        {
            output = emg[1].update(output);
        }
        else if (m_mode == EOG_MODE)
        {
            output = eog[1].update(output);
        }
        else if (m_mode == EEG_MODE)
        {
            output = eeg[1].update(output);
        }
        else if (m_mode == ECG_MODE)
        {
            output = ecg[1].update(output);
        }
    }

    return output;
}
