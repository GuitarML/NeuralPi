# NeuralPi

NeuralPi is a guitar pedal using neural networks to emulate real amps and pedals on a Raspberry Pi 4. The NeuralPi software is a VST3 plugin built with JUCE, which can be run as a normal audio plugin or cross-compiled to run on the Raspberry Pi 4 with [Elk Audio OS](https://elk.audio/). NeuralPi is intended as a bare-bones plugin to build on.  

![app](https://github.com/GuitarML/NeuralPi/blob/main/resources/rpi_pic.jpg)

NeuralPi can sound like an amplifier or distortion/overdrive pedal using the power of neural networks. Machine learning models trained from recordings of real amps and pedals can be loaded into the plugin for endless possiblities on your guitar. Create your own models or use custom tones from GuitarML.

There are four main components to the guitar pedal:

1. Raspberry Pi 4b
2. HiFiBerry DAC + ADC
3. Elk Audio OS
4. Proteus VST3 plugin

The bare bones pedal can be built for around $120.

![app](https://github.com/GuitarML/NeuralPi/blob/main/resources/neuralpi_pic.jpg)
<br>This is the normal VST3 plugin, which can be compiled for Windows/Mac/Linux using JUCE. It includes a model import feature, dropdown menu, and gain/level controls. The plugin compiled for Elk Audio OS is headless, meaning there is no GUI.

## Installing the plugin

For the cross-compiled Raspberry Pi / Elk Audio OS compatible VST3 plugin, download [here](https://github.com/GuitarML/NeuralPi/releases/tag/v1.0).

WARNING: The audio output of the NeuralPi is at line level. Guitar amplifiers expect a low level electric guitar signal (instrument level). When using the NeuralPi with a guitar amp, start with the master volume at 0 and SLOWLY increase from there. 

## To Do

Currently, the NeuralPi plugin has no user controls. It runs a single model that can be swapped out before running the plugin. The next step is to add user controls via OSC messages, so that a remote instance of the plugin can control the NeuralPi over Wifi. These controls will include Gain/Volume, EQ, and model selection. 

Elk Audio OS also supports physical controls through [Sensei](https://github.com/elk-audio/sensei). Gain/Volume and EQ knobs can be added, as well as a LCD screen for selecting different models. One could build an actual guitar pedal with NeuralPi and any number of other digital effects and controls.

## Info
The neural network is a re-creation of the LSTM inference model from [Real-Time Guitar Amplifier Emulation with Deep Learning](https://www.mdpi.com/2076-3417/10/3/766/htm)

The [Automated-GuitarAmpModelling](https://github.com/Alec-Wright/Automated-GuitarAmpModelling) project was used to train the .json models. 

The plugin uses [RTNeural](https://github.com/jatinchowdhury18/RTNeural), which is a highly optimized neural net inference engine intended for audio applications. 

## Build Instructions
