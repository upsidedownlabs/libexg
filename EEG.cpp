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


#include "EEG.h"


static float eeg_lpf_coef_100[2][3] = {
    {0.13110644,0.26221288,0.13110644}, {1.00000000,-0.74778918,0.27221494}};
static float eeg_lpf_coef_250[2][3] = {
    {0.02785977,0.05571953,0.02785977}, {1.00000000,-1.47548044,0.58691951}};
static float eeg_lpf_coef_500[2][3] = {
    {0.00782021,0.01564042,0.00782021}, {1.00000000,-1.73472577,0.76600660}};
static float eeg_lpf_coef_1000[2][3] = {
    {0.00208057,0.00416113,0.00208057}, {1.00000000,-1.86689228,0.87521455}};
static float eeg_hpf_coef_100[2][3] = {
    {0.50500103,-1.01000206,0.50500103}, {1.00000000,-0.74778918,0.27221494}};
static float eeg_hpf_coef_250[2][3] = {
    {0.76559999,-1.53119998,0.76559999}, {1.00000000,-1.47548044,0.58691951}};
static float eeg_hpf_coef_500[2][3] = {
    {0.87518309,-1.75036618,0.87518309}, {1.00000000,-1.73472577,0.76600660}};
static float eeg_hpf_coef_1000[2][3] = {
    {0.93552671,-1.87105341,0.93552671}, {1.00000000,-1.86689228,0.87521455}};

void EEG_filter ::init(FILTER_TYPE ftype, int sampleFreq)
{
    states[0] = 0;
    states[1] = 0;
    if (ftype == FILTER_TYPE_LOWPASS)
    {
        // 2th order butterworth lowpass filter
        // cutoff frequency 150Hz
        if (sampleFreq == 100)
        {
            for (int i = 0; i < 3; i++)
            {
                num[i] = eeg_lpf_coef_100[0][i];
                den[i] = eeg_lpf_coef_100[1][i];
            }
        }
        else if (sampleFreq == 250)
        {
            for (int i = 0; i < 3; i++)
            {
                num[i] = eeg_lpf_coef_250[0][i];
                den[i] = eeg_lpf_coef_250[1][i];
            }
        }
        else if (sampleFreq == 500)
        {
            for (int i = 0; i < 3; i++)
            {
                num[i] = eeg_lpf_coef_500[0][i];
                den[i] = eeg_lpf_coef_500[1][i];
            }
        }
        else if (sampleFreq == 1000)
        {
            for (int i = 0; i < 3; i++)
            {
                num[i] = eeg_lpf_coef_1000[0][i];
                den[i] = eeg_lpf_coef_1000[1][i];
            }
        }
    }
    else if (ftype == FILTER_TYPE_HIGHPASS)
    {
        // 2th order butterworth
        // cutoff frequency 20Hz
         if (sampleFreq == 100)
        {
            for (int i = 0; i < 3; i++)
            {
                num[i] = eeg_hpf_coef_100[0][i];
                den[i] = eeg_hpf_coef_100[1][i];
            }
        }
        else if (sampleFreq == 250)
        {
            for (int i = 0; i < 3; i++)
            {
                num[i] = eeg_hpf_coef_250[0][i];
                den[i] = eeg_hpf_coef_250[1][i];
            }
        }
        else if (sampleFreq == 500)
        {
            for (int i = 0; i < 3; i++)
            {
                num[i] = eeg_hpf_coef_500[0][i];
                den[i] = eeg_hpf_coef_500[1][i];
            }
        }
        else if (sampleFreq == 1000)
        {
            for (int i = 0; i < 3; i++)
            {
                num[i] = eeg_hpf_coef_1000[0][i];
                den[i] = eeg_hpf_coef_1000[1][i];
            }
        }
    }
}

float EEG_filter::update(float input)
{
    float tmp = (input - den[1] * states[0] - den[2] * states[1]) / den[0];
    float output = num[0] * tmp + num[1] * states[0] + num[2] * states[1];
    // save last states
    states[1] = states[0];
    states[0] = tmp;
    return output;
}
