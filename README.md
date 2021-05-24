# NeuralPi

NeuralPi is a guitar pedal using neural networks to emulate real amps and pedals on a Raspberry Pi 4. The NeuralPi software is a VST3 plugin built with JUCE, which can be run as a normal audio plugin or cross-compiled to run on the Raspberry Pi 4 with [Elk Audio OS](https://elk.audio/). NeuralPi is intended as a bare-bones plugin to build on. The NeuralPi pedal runs high quality amp/pedal models on an economical DIY setup, costing around $120 for hardware to build yourself.

![app](https://github.com/GuitarML/NeuralPi/blob/main/resources/rpi_pic.jpg)

NeuralPi can sound like an amplifier or distortion/overdrive pedal using the power of neural networks. Machine learning models trained from recordings of real amps and pedals can be loaded into the plugin for endless possiblities on your guitar. Create your own models or use custom tones from GuitarML.

There are four main components to the guitar pedal:

1. Raspberry Pi 4b
2. HiFiBerry DAC + ADC
3. Elk Audio OS
4. Proteus VST3 plugin

![app](https://github.com/GuitarML/NeuralPi/blob/main/resources/neuralpi_pic.jpg)
<br>This is the normal VST3 plugin, which can be compiled for Windows/Mac/Linux using JUCE. It includes a model import feature, dropdown menu, and gain/level controls. The plugin compiled for Elk Audio OS is headless, meaning there is no GUI.

## Installing the plugin

For the cross-compiled Raspberry Pi / Elk Audio OS compatible VST3 plugin, download [here](https://github.com/GuitarML/NeuralPi/releases/tag/v1.0).

WARNING: The audio output of the NeuralPi is at line level. Guitar amplifiers expect a low level electric guitar signal (instrument level). When using the NeuralPi with a guitar amp, start with the master volume at 0 and SLOWLY increase from there. 

## Changing Models

The NeuralPi running on Elk will load one model per instance of the plugin. By default this is the "bj_model_best.json" file. Model select controls will be added in the future, but for now you must overwrite this file to load different models. On Elk Audio OS, the models will be installed here after running the plugin for the first time:

```/home/mind/.config/JUCE/NeuralPi/tones```

Backup the Blues Junior amp model with this:

```cp bj_model_best.json bj_backup.json```

And overwrite the original model to try a new one:

```cp ts9_model_best.json bj_model_best.json```

If you need to revert back to the original state, remove the ```/tones``` folder and restart the plugin. The "bj_model_best.json" is a Fender Blues Jr. amplifier at full gain, and the "ts9_model_best.json" is the Ibanez TS9 overdrive pedal at full drive.

## To Do

Currently, the NeuralPi plugin has no user controls. It runs a single model that can be swapped out before running the plugin. The next step is to add user controls via OSC messages, so that a remote instance of the plugin can control the NeuralPi over Wifi. These controls will include Gain/Volume, EQ, and model selection. 

Elk Audio OS also supports physical controls through [Sensei](https://github.com/elk-audio/sensei). Gain/Volume and EQ knobs can be added, as well as a LCD screen for selecting different models. One could build an actual guitar pedal with NeuralPi and any number of other digital effects and controls.

## Info
The neural network is a re-creation of the LSTM inference model from [Real-Time Guitar Amplifier Emulation with Deep Learning](https://www.mdpi.com/2076-3417/10/3/766/htm)

The [Automated-GuitarAmpModelling](https://github.com/Alec-Wright/Automated-GuitarAmpModelling) project was used to train the .json models. 

The plugin uses [RTNeural](https://github.com/jatinchowdhury18/RTNeural), which is a highly optimized neural net inference engine intended for audio applications. 

## Build Instructions

To build the plugin for use on the Raspberry Pi with Elk Audio OS, see the official [Elk Audio Documentation](https://elk-audio.github.io/elk-docs/html/documents/building_plugins_for_elk.html#vst-plugins-using-juce)

To build for Windows/Mac/Linux:

1. Clone or download this repository.
2. Download and install [JUCE](https://juce.com/) This project uses the "Projucer" application from the JUCE website. 
3. Download the RTNeural submodule (cd into the NeuralPi repo first):
   
   ```git submodule update --remote --recursive```
   
4. Download and extract: [json](https://github.com/nlohmann/json) Json for c++.
5. Open the Chameleon.jucer file and in the appropriate Exporter Header Search Path field, enter the appropriate include paths.
   For example:

```
  <full-path-to>/json-develop/include
  <full-path-to>/NeuralPi/modules/RTNeural
  <full-path-to>/NeuralPi/modules/RTNeural/modules/xsimd/include
```
6. Build Chameleon from the Juce Projucer application for the intended build target. 

Note: Make sure to build in Release mode unless actually debugging. Debug mode will not keep up with real time playing.
