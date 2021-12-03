/* LibEXG
https://github.com/upsidedownlabs/libexg

Upside Down Labs invests time and resources providing this open source code,
please support Upside Down Labs and open-source hardware by purchasing
products from Upside Down Labs!

Copyright (c) 2021 Moteen Shah moteenshah.02@gmail.com
Copyright (c) 2021 Upside Down Labs - contact@upsidedownlabs.tech

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "libexg.h"

LibEXG::LibEXG(bool displayMsg) {}

// Signals
float LibEXG::getData(int mode, uint8_t INPUT_PIN, int Channel)
{
  static unsigned long past = 0;
  unsigned long present = micros();
  unsigned long interval = present - past;
  past = present;

  // Run timer
  static long timer = 0;
  timer -= interval;

  // Sample
  if (timer < 0)
  {
    timer += 1000000 / SAMPLE_RATE;
    switch (mode)
    {
    case 1:
      signal[1] = EXGFilter(analogRead(INPUT_PIN),EMG_MODE, Channel);
      return signal[1];
      break;
    case 2:
      signal[2] = EXGFilter(analogRead(INPUT_PIN),EEG_MODE,Channel);
      return signal[2];
      break;
    case 3:
      signal[0] = EXGFilter(analogRead(INPUT_PIN),EOG_MODE,Channel);
      return signal[0];
      break;
    case 4:
      signal[3] = EXGFilter(analogRead(INPUT_PIN),ECG_MODE,Channel);
      return signal[3];
      break;
    default:
      break;
    }
  }
}

// Filters
float LibEXG::EXGFilter(float input, int mode, int channel)
{
  switch (mode)
  {
  case 1:
    output[channel][0] = input;
    {
      x[channel][0] = output[channel][0] - 0.05159732 * z_filter[channel][0][0] - 0.36347401 * z_filter[channel][0][1];
      output[channel][0] = 0.01856301 * x[channel][0] + 0.03712602 * z_filter[channel][0][0] + 0.01856301 * z_filter[channel][0][1];
      z_filter[channel][0][1] = z_filter[channel][0][0];
      z_filter[channel][0][0] = x[channel][0];
    }
    {
      x[channel][1] = output[channel][0] - -0.53945795 * z_filter[channel][1][0] - 0.39764934 * z_filter[channel][1][1];
      output[channel][0] = 1.00000000 * x[channel][1] + -2.00000000 * z_filter[channel][1][0] + 1.00000000 * z_filter[channel][1][1];
      z_filter[channel][1][1] = z_filter[channel][1][0];
      z_filter[channel][1][0] = x[channel][1];
    }
    {
      x[channel][2] = output[channel][0] - 0.47319594 * z_filter[channel][2][0] - 0.70744137 * z_filter[channel][2][1];
      output[channel][0] = 1.00000000 * x[channel][0] + 2.00000000 * z_filter[channel][2][0] + 1.00000000 * z_filter[channel][2][1];
      z_filter[channel][2][1] = z_filter[channel][2][0];
      z_filter[channel][2][0] = x[channel][2];
    }
    {
      x[channel][3] = output[channel][0] - -1.00211112 * z_filter[channel][3][0] - 0.74520226 * z_filter[channel][3][1];
      output[channel][0] = 1.00000000 * x[channel][3] + -2.00000000 * z_filter[channel][3][0] + 1.00000000 * z_filter[channel][3][1];
      z_filter[channel][3][1] = z_filter[channel][3][0];
      z_filter[channel][3][0] = x[channel][3];
    }
    return output[channel][0];
    break;
  case 2:
    output[channel][1] = input;
    {
      x[channel][0] = output[channel][1] - -1.41361551 * z_filter[channel][0][0] - 0.51060681 * z_filter[channel][0][1];
      output[channel][1] = 0.00071374 * x[channel][0] + 0.00142749 * z_filter[channel][0][0] + 0.00071374 * z_filter[channel][0][1];
      z_filter[channel][0][1] = z_filter[channel][0][0];
      z_filter[channel][0][0] = x[channel][0];
    }
    {
      x[channel][1] = output[channel][1] - -1.64480745 * z_filter[channel][1][0] - 0.76352964 * z_filter[channel][1][1];
      output[channel][1] = 1.00000000 * x[channel][1] + 2.00000000 * z_filter[channel][1][0] + 1.00000000 * z_filter[channel][1][1];
      z_filter[channel][1][1] = z_filter[channel][1][0];
      z_filter[channel][1][0] = x[channel][1];
    }
    {
      x[channel][2] = output[channel][1] - -1.98813960 * z_filter[channel][2][0] - 0.98818120 * z_filter[channel][2][1];
      output[channel][1] = 1.00000000 * x[channel][2] + -2.00000000 * z_filter[channel][0][0] + 1.00000000 * z_filter[channel][2][1];
      z_filter[channel][2][1] = z_filter[channel][2][0];
      z_filter[channel][2][0] = x[channel][2];
    }
    {
      x[channel][3] = output[channel][1] - -1.99527805 * z_filter[channel][3][0] - 0.99531780 * z_filter[channel][3][1];
      output[channel][1] = 1.00000000 * x[channel][3] + -2.00000000 * z_filter[channel][0][0] + 1.00000000 * z_filter[channel][3][1];
      z_filter[channel][3][1] = z_filter[channel][3][0];
      z_filter[channel][3][0] = x[channel][3];
    }
    return output[channel][0];
    break;
  case 3:
    output[channel][3] = input;
    {
      x[channel][0] = output[channel][2] - -1.60403273 * z_filter[channel][0][0] - 0.64913439 * z_filter[channel][0][1];
      output[channel][2] = 0.00015141 * x[channel][0] + 0.00030282 * z_filter[channel][0][0] + 0.00015141 * z_filter[channel][0][1];
      z_filter[channel][0][1] = z_filter[channel][0][0];
      z_filter[channel][0][0] = x[channel][0];
    }
    {
      x[channel][1] = output[channel][2] - -1.78372179 * z_filter[channel][1][0] - 0.83788559 * z_filter[channel][1][1];
      output[channel][2] = 1.00000000 * x[channel][1] + 2.00000000 * z_filter[channel][1][0] + 1.00000000 * z_filter[channel][1][1];
      z_filter[channel][1][1] = z_filter[channel][1][0];
      z_filter[channel][1][0] = x[channel][1];
    }
    {
      x[channel][2] = output[channel][2] - -1.98799122 * z_filter[channel][2][0] - 0.98803414 * z_filter[channel][2][1];
      output[channel][2] = 1.00000000 * x[channel][2] + -2.00000000 * z_filter[channel][2][0] + 1.00000000 * z_filter[channel][2][1];
      z_filter[channel][2][1] = z_filter[channel][2][0];
      z_filter[channel][2][0] = x[channel][2];
    }
    {
      x[channel][3] = output[channel][2] - -1.99533934 * z_filter[channel][3][0] - 0.99537927 * z_filter[channel][3][1];
      output[channel][2] = 1.00000000 * x[channel][3] + -2.00000000 * z_filter[channel][3][0] + 1.00000000 * z_filter[channel][3][1];
      z_filter[channel][3][1] = z_filter[channel][3][0];
      z_filter[channel][3][0] = x[channel][3];
    }
    return output[channel][0];
    break;
  case 4:
    output[channel][0] = input;
    {
      x[channel][0] = output[channel][3] - -1.15206017 * z_filter[channel][0][0] - 0.35073306 * z_filter[channel][0][1];
      output[channel][3] = 0.00311190 * x[channel][0] + 0.00622381 * z_filter[channel][0][0] + 0.00311190 * z_filter[channel][0][1];
      z_filter[channel][0][1] = z_filter[channel][0][0];
      z_filter[channel][0][0] = x[channel][0];
    }
    {
      x[channel][1] = output[channel][3] - -1.41577490 * z_filter[channel][1][0] - 0.66846960 * z_filter[channel][1][1];
      output[channel][3] = 1.00000000 * x[channel][1] + 2.00000000 * z_filter[channel][1][0] + 1.00000000 * z_filter[channel][1][1];
      z_filter[channel][1][1] = z_filter[channel][1][0];
      z_filter[channel][1][0] = x[channel][1];
    }
    {
      x[channel][2] = output[channel][3] - -1.98823710 * z_filter[channel][2][0] - 0.98827786 * z_filter[channel][2][1];
      output[channel][3] = 1.00000000 * x[channel][2] + -2.00000000 * z_filter[channel][2][0] + 1.00000000 * z_filter[channel][2][1];
      z_filter[channel][2][1] = z_filter[channel][2][0];
      z_filter[channel][2][0] = x[channel][2];
    }
    {
      x[channel][3] = output[channel][3] - -1.99523783 * z_filter[channel][3][0] - 0.99527746 * z_filter[channel][3][1];
      output[channel][3] = 1.00000000 * x[channel][3] + -2.00000000 * z_filter[channel][3][0] + 1.00000000 * z_filter[channel][3][1];
      z_filter[channel][3][1] = z_filter[channel][3][0];
      z_filter[channel][3][0] = x[channel][3];
    }
    return output[channel][0];
  default:
    break;
  }
}