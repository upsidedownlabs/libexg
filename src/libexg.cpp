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

#define SAMPLE_RATE 500
float sensor_value[4];
float signal[4];
LibEXG::LibEXG(bool displayMsg) {}

// Signals
float LibEXG::getecg(int mode, int INPUT_PIN)
{
  static unsigned long past = 0;
	unsigned long present = micros();
	unsigned long interval = present - past;
	past = present;

	// Run timer
	static long timer = 0;
	timer -= interval;

	// Sample
	if(timer < 0){
		timer += 1000000 / SAMPLE_RATE;
		sensor_value[0] = analogRead(INPUT_PIN);
		signal[0] = EOGFilter(sensor_value);
    sensor_value[1] = analogRead(INPUT_PIN);
		signal[0] = EOGFilter(sensor_value);
    sensor_value[2] = analogRead(INPUT_PIN);
		signal[0] = EOGFilter(sensor_value);
    sensor_value[3] = analogRead(INPUT_PIN);
		signal[0] = EOGFilter(sensor_value);
	}
}

float LibEXG::getemg(int INPUT_PIN)
{

  present = micros();
  interval = present - past;
  past = present;

  // Run timer
  timer -= interval;

  // Sample
  if (timer < 0)
  {
    timer += 1000000 / SAMPLE_RATE;
    float sensor_value = analogRead(INPUT_PIN);
    float signal = ECGFilter(sensor_value);
    Serial.println(signal);
  }
}

float LibEXG::geteog(int INPUT_PIN)
{

  present = micros();
  interval = present - past;
  past = present;

  // Run timer
  timer -= interval;

  // Sample
  if (timer < 0)
  {
    timer += 1000000 / SAMPLE_RATE;
    float sensor_value = analogRead(INPUT_PIN);
    float signal = ECGFilter(sensor_value);
    Serial.println(signal);
  }
}

float LibEXG::geteeg(int INPUT_PIN)
{

  present = micros();
  interval = present - past;
  past = present;

  // Run timer
  timer -= interval;

  // Sample
  if (timer < 0)
  {
    timer += 1000000 / SAMPLE_RATE;
    float sensor_value = analogRead(INPUT_PIN);
    float signal = ECGFilter(sensor_value);
    Serial.println(signal);
  }
}

// Filters

// Band-Pass Butterworth IIR digital filter, generated using filter_gen.py.
// Sampling rate: 500 Hz, frequency: [0.5, 19.5] Hz.
// Filter is order 4, implemented as second-order sections (biquads).
// Reference: https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.butter.html
float LibEXG::EOGFilter(float input)
{
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - -1.60403273 * z1 - 0.64913439 * z2;
    output = 0.00015141 * x + 0.00030282 * z1 + 0.00015141 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.78372179 * z1 - 0.83788559 * z2;
    output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.98799122 * z1 - 0.98803414 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.99533934 * z1 - 0.99537927 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  return output;
}

// Band-Pass Butterworth IIR digital filter, generated using filter_gen.py.
// Sampling rate: 500 Hz, frequency: [74.5, 149.5] Hz.
// Filter is order 4, implemented as second-order sections (biquads).
// Reference: https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.butter.html
float LibEXG::EMGFilter(float input)
{
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - 0.05159732 * z1 - 0.36347401 * z2;
    output = 0.01856301 * x + 0.03712602 * z1 + 0.01856301 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -0.53945795 * z1 - 0.39764934 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - 0.47319594 * z1 - 0.70744137 * z2;
    output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.00211112 * z1 - 0.74520226 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  return output;
}

// Band-Pass Butterworth IIR digital filter, generated using filter_gen.py.
// Sampling rate: 500 Hz, frequency: [0.5, 29.5] Hz.
// Filter is order 4, implemented as second-order sections (biquads).
// Reference: https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.butter.html
float LibEXG::EEGFilter(float input)
{
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - -1.41361551 * z1 - 0.51060681 * z2;
    output = 0.00071374 * x + 0.00142749 * z1 + 0.00071374 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.64480745 * z1 - 0.76352964 * z2;
    output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.98813960 * z1 - 0.98818120 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.99527805 * z1 - 0.99531780 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  return output;
}

// Band-Pass Butterworth IIR digital filter, generated using filter_gen.py.
// Sampling rate: 500 Hz, frequency: [0.5, 44.5] Hz.
// Filter is order 4, implemented as second-order sections (biquads).
// Reference: https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.butter.html
float LibEXG::ECGFilter(float input)
{
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - -1.15206017 * z1 - 0.35073306 * z2;
    output = 0.00311190 * x + 0.00622381 * z1 + 0.00311190 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.41577490 * z1 - 0.66846960 * z2;
    output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.98823710 * z1 - 0.98827786 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.99523783 * z1 - 0.99527746 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  return output;
}
