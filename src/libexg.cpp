// LibEXG
// https://github.com/upsidedownlabs/libexg

// Upside Down Labs invests time and resources providing this open source code,
// please support Upside Down Labs and open-source hardware by purchasing
// products from Upside Down Labs!

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


#include "libexg.h"

#define SAMPLE_RATE_ECG 125
#define SAMPLE_RATE_EEG 256
#define SAMPLE_RATE_EMG 500
#define SAMPLE_RATE_EOG 75

static unsigned long past = 0;
static long timer = 0;  
unsigned long interval = 0;
unsigned long present = 0; 
static float z1 = 0, z2 = 0, z3 = 0, z4 = 0, z5 = 0, z6 = 0, z7 = 0, z8 = 0;

LibEXG::LibEXG(bool displayMsg) {}

// Signals
float LibEXG::getecg(int INPUT_PIN){

  present = micros();
  interval = present - past;
  past = present;

  // Run timer

  timer -= interval;

  // Sample
  if (timer < 0){
    timer += 1000000 / SAMPLE_RATE_ECG;
    float sensor_value = analogRead(INPUT_PIN);
    float signal = ECGFilter(sensor_value);
    Serial.println(signal);
  }
}

float LibEXG::getemg(int INPUT_PIN){

  present = micros();
  interval = present - past;
  past = present;

  // Run timer
  timer -= interval;

  // Sample
  if (timer < 0)
  {
    timer += 1000000 / SAMPLE_RATE_EMG;
    float sensor_value = analogRead(INPUT_PIN);
    float signal = ECGFilter(sensor_value);
    Serial.println(signal);
  }
}

float LibEXG::geteog(int INPUT_PIN){
  
  present = micros();
  interval = present - past;
  past = present;

  // Run timer
  timer -= interval;

  // Sample
  if (timer < 0){
    timer += 1000000 / SAMPLE_RATE_EOG;
    float sensor_value = analogRead(INPUT_PIN);
    float signal = ECGFilter(sensor_value);
    Serial.println(signal);
  }
}

float LibEXG::geteeg(int INPUT_PIN){

  present = micros();
  interval = present - past;
  past = present;

  // Run timer
  timer -= interval;

  // Sample
  if (timer < 0){
    timer += 1000000 / SAMPLE_RATE_EEG;
    float sensor_value = analogRead(INPUT_PIN);
    float signal = ECGFilter(sensor_value);
    Serial.println(signal);
  }
}

// Filters

float LibEXG::EOGFilter(float input){
  float output = input;
  {
    // static float z1, z2; // filter section state
    float x = output - 0.02977423 * z1 - 0.04296318 * z2;
    output = 0.09797471 * x + 0.19594942 * z1 + 0.09797471 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    // static float z1, z2; // filter section state
    float x = output - 0.08383952 * z3 - 0.46067709 * z4;
    output = 1.00000000 * x + 2.00000000 * z3 + 1.00000000 * z4;
    z4 = z3;
    z3 = x;
  }
  {
    // static float z1, z2; // filter section state
    float x = output - -1.92167271 * z5 - 0.92347975 * z6;
    output = 1.00000000 * x + -2.00000000 * z5 + 1.00000000 * z6;
    z6 = z5;
    z5 = x;
  }
  {
    // static float z1, z2; // filter section state
    float x = output - -1.96758891 * z7 - 0.96933514 * z8;
    output = 1.00000000 * x + -2.00000000 * z7 + 1.00000000 * z8;
    z8 = z7;
    z7 = x;
  }
  return output;
}

float LibEXG::EMGFilter(float input){
  float output = input;
  {
    // static float z1, z2; // filter section state
    float x = output - 0.05159732 * z1 - 0.36347401 * z2;
    output = 0.01856301 * x + 0.03712602 * z1 + 0.01856301 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    // static float z1, z2; // filter section state
    float x = output - -0.53945795 * z3 - 0.39764934 * z4;
    output = 1.00000000 * x + -2.00000000 * z3 + 1.00000000 * z4;
    z4 = z3;
    z3 = x;
  }
  {
    // static float z1, z2; // filter section state
    float x = output - 0.47319594 * z5 - 0.70744137 * z6;
    output = 1.00000000 * x + 2.00000000 * z5 + 1.00000000 * z6;
    z6 = z5;
    z5 = x;
  }
  {
    // static float z1, z2; // filter section state
    float x = output - -1.00211112 * z7 - 0.74520226 * z8;
    output = 1.00000000 * x + -2.00000000 * z7 + 1.00000000 * z8;
    z8 = z7;
    z7 = x;
  }
  return output;
}

float LibEXG::EEGFilter(float input){

  float output = input;
  {
    // static float z1, z2; // filter section state
    float x = output - -0.95391350 * z1 - 0.25311356 * z2;
    output = 0.00735282 * x + 0.01470564 * z1 + 0.00735282 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    // static float z1, z2; // filter section state
    float x = output - -1.20596630 * z3 - 0.60558332 * z4;
    output = 1.00000000 * x + 2.00000000 * z3 + 1.00000000 * z4;
    z4 = z3;
    z3 = x;
  }
  {
    // static float z1, z2; // filter section state
    float x = output - -1.97690645 * z5 - 0.97706395 * z6;
    output = 1.00000000 * x + -2.00000000 * z5 + 1.00000000 * z6;
    z6 = z5;
    z5 = x;
  }
  {
    // static float z1, z2; // filter section state
    float x = output - -1.99071687 * z7 - 0.99086813 * z8;
    output = 1.00000000 * x + -2.00000000 * z7 + 1.00000000 * z8;
    z8 = z7;
    z7 = x;
  }
  return output;
}

float LibEXG::ECGFilter(float input){
  float output = input;
  {
    // static float z1, z2; // filter section state
    float x = output - 0.70682283 * z1 - 0.15621030 * z2;
    output = 0.28064917 * x + 0.56129834 * z1 + 0.28064917 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    // static float z1, z2; // filter section state
    float x = output - 0.95028224 * z3 - 0.54073140 * z4;
    output = 1.00000000 * x + 2.00000000 * z3 + 1.00000000 * z4;
    z4 = z3;
    z3 = x;
  }
  {
    // static float z1, z2; // filter section state
    float x = output - -1.95360385 * z5 - 0.95423412 * z6;
    output = 1.00000000 * x + -2.00000000 * z5 + 1.00000000 * z6;
    z6 = z5;
    z5 = x;
  }
  {
    // static float z1, z2; // filter section state
    float x = output - -1.98048558 * z7 - 0.98111344 * z8;
    output = 1.00000000 * x + -2.00000000 * z7 + 1.00000000 * z8;
    z8 = z7;
    z7 = x;
  }
  return output;
}

