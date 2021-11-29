# __libexg__
This repository contains Arduino library for using Bio-Potential Amplifiers to record EMG, ECG, EEG, EOG data using Arduino. You can use the filtered data from the Bio-potential Amplifier Modules to create new and awesome Projects!


## Compatibility
This Library can be used with multiple Arduino boards available in the market, for example 
- Arduino Micro
- Arduino Leonardo
- Arduino Mega
- Arduino Due
- Arduino Nano

Also this library can support ESP32 develpment boards.
___
## Usage
This library allows an Arduino board to use it's ADC for getting analog data from Bio-Potential amplifiers. This library has been tested on the Bio-Amp-EXG-Pill module that amplifies the electrcal signals generated in our nerve cells and sends the filtered and amplified electricals signals to the microcontroller.

The __libexg__ library supports only one amplifier module at a time with a particular type of data extraction which maybe anyone from EMG, ECG, EEG, EOG data extraction. 
To use this library:

`#include <libexg.h>`
___
## Circuit

The Basic connection of Bio-Amp-EXG-Pill can de done as shown

![connection](https://github.com/upsidedownlabs/BioAmp-EXG-Pill/blob/main/graphics/circuits/Basic-Circuit.png)

The data pin can be connected to any analog input pin of arduino.

Similarly, ESP32 can also be connected but with a Voltage-Divider as this microcontroller works on 3.3volts.

Any other Bio-Potential Amplifier can be connected in the same way with the analog output of the module connected to the analog input of arduino or any other microcontroller.
___

## Examples
The examples with this library shows the basic implementation of data extraction from Bio-Amp-EXG-Pill Module.
* [ECG](examples/ECG/ECG.ino)
* [EEG](examples/EEG/EEG.ino)
* [EMG](examples/EMG/EMG.ino)
* [EOG](examples/EOG/EOG.ino)

## Methods
___
#### __`getecg()`__ 
* Enable an analog Input pin and get filtered ECG data at a specific sample rate.

* __Syntax__ - `libEXG.getecg(pin)`

__Parameters__
* pin -  the number of the pin that the Bio-Amp_EXG-Pill data pin is attached to. 

__Return__
___
#### __`geteeg()`__
- Enable an analog Input pin and get filtered EEG data at a specific sample rate.

* __Syntax__ - `libEXG.getecg(pin)`

__Parameters__
* pin -  the number of the pin that the Bio-Amp_EXG-Pill data pin is attached to 

___
#### __`getemg()`__
- Enable an analog Input pin and get filtered EMG data at a specific sample rate.


* __Syntax__ - `libEXG.getecg(pin)`

__Parameters__
* pin -  the number of the pin that the Bio-Amp_EXG-Pill data pin is attached to 
___
#### __`geteog()`__
- Enable an analog Input pin and get filtered EOG data at a specific sample rate.

* __Syntax__ - `libEXG.getecg(pin)`

__Parameters__
* pin -  the number of the pin that the Bio-Amp_EXG-Pill data pin is attached to 