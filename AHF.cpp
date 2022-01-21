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


#include "AHF.h"

static float ahf_numerator_coef_50Hz[2][6] = {
    {0.9522, -1.5407, 0.9522, 0.8158, -0.8045, 0.0855},
    {0.5869, -1.1146, 0.5869, 1.0499, -2.0000, 1.0499}};
static float ahf_denominator_coef_50Hz[2][6] = {
    {1.0000, -1.5395, 0.9056, 1.0000 - 1.1187, 0.3129},
    {1.0000, -1.8844, 0.9893, 1.0000, -1.8991, 0.9892}};
static float ahf_output_gain_coef_50Hz[2] = {1.3422, 1.4399};
// coef[sampleFreqInd][order] for 60Hz
static float ahf_numerator_coef_60Hz[2][6] = {
    {0.9528, -1.3891, 0.9528, 0.8272, -0.7225, 0.0264},
    {0.5824, -1.0810, 0.5824, 1.0736, -2.0000, 1.0736}};
static float ahf_denominator_coef_60Hz[2][6] = {
    {1.0000, -1.3880, 0.9066, 1.0000, -0.9739, 0.2371},
    {1.0000, -1.8407, 0.9894, 1.0000, -1.8584, 0.9891}};
static float ahf_output_gain_coef_60Hz[2] = {1.3430, 1.4206};


void AH_filter::init(int sampleFreq, int humFreq)
{
    gain = 0;
    for (int i = 0; i < 4; i++)
    {
        states[i] = 0;
    }
    if (humFreq == 50)
    {
        if (sampleFreq == 500)
        {
            for (int i = 0; i < 6; i++)
            {
                num[i] = ahf_numerator_coef_50Hz[0][i];
                den[i] = ahf_denominator_coef_50Hz[0][i];
            }
            gain = ahf_output_gain_coef_50Hz[0];
        }
        else if (sampleFreq == 1000)
        {
            for (int i = 0; i < 6; i++)
            {
                num[i] = ahf_numerator_coef_50Hz[1][i];
                den[i] = ahf_denominator_coef_50Hz[1][i];
            }
            gain = ahf_output_gain_coef_50Hz[1];
        }
    }
    else if (humFreq == 60)
    {
        if (sampleFreq == 500)
        {
            for (int i = 0; i < 6; i++)
            {
                num[i] = ahf_numerator_coef_60Hz[0][i];
                den[i] = ahf_denominator_coef_60Hz[0][i];
            }
            gain = ahf_output_gain_coef_60Hz[0];
        }
        else if (sampleFreq == 1000)
        {
            for (int i = 0; i < 6; i++)
            {
                num[i] = ahf_numerator_coef_60Hz[1][i];
                den[i] = ahf_denominator_coef_60Hz[1][i];
            }
            gain = ahf_output_gain_coef_60Hz[1];
        }
    }
}

float AH_filter::update(float input)
{
    float output;
    float stageIn;
    float stageOut;

    stageOut = num[0] * input + states[0];
    states[0] = (num[1] * input + states[1]) - den[1] * stageOut;
    states[1] = num[2] * input - den[2] * stageOut;
    stageIn = stageOut;
    stageOut = num[3] * stageOut + states[2];
    states[2] = (num[4] * stageIn + states[3]) - den[4] * stageOut;
    states[3] = num[5] * stageIn - den[5] * stageOut;

    output = gain * stageOut;

    return output;
}