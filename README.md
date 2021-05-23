# NeuralPi

NeuralPi is a guitar pedal using neural networks to emulate real amps and pedals on a Raspberry Pi 4. The NeuralPi software is a VST3 plugin built with JUCE, which can be run as a normal audio plugin or cross-compiled to run on the Raspberry Pi 4 with [Elk Audio OS](https://elk.audio/). Simple gain and level knobs are included, but have not yet been implemented as controls while running on the Pi, only with normal plugin operation. NeuralPi is intended as a bare-bones plugin to build on.  

![app](https://github.com/GuitarML/NeuralPi/blob/main/resources/rpi_pic.jpg)

NeuralPi can sound like an amplifier or distortion/overdrive pedal using the power of neural networks. Machine learning models trained from recordings of real amps and pedals can be loaded into the plugin for endless possiblities on your guitar. Create your own models or use custom tones from GuitarML.

There are four main components to the guitar pedal:

1. Raspberry Pi 4b
2. HiFiBerry DAC + ADC
3. Elk Audio OS
4. Proteus VST3 plugin

The bare bones pedal can be built for around $120. For a complete guide to building the NeuralPi pedal yourself, see this article.

![app](https://github.com/GuitarML/NeuralPi/blob/main/resources/neuralpi_pic.jpg)

## Installing the plugin

For the cross-compiled Raspberry Pi / Elk Audio OS compatible VST3 plugin, download here.

For the NeuralPi plugin for use in a normal DAW:
1. Download the appropriate plugin installer (Windows 10, Mac, Ubuntu Linux)
2. Run the installer and follow the instructions. May need to reboot to allow your DAW to recognize the new plugin.

WARNING: The audio output of the NeuralPi is at line level. Guitar amplifiers expect a low level electric guitar signal (instrument level). When using the NeuralPi with a guitar amp, start with the master volume at 0 and SLOWLY increase from there. 

## Info
The neural network is a re-creation of the LSTM inference model from [Real-Time Guitar Amplifier Emulation with Deep Learning](https://www.mdpi.com/2076-3417/10/3/766/htm)

The [Automated-GuitarAmpModelling](https://github.com/Alec-Wright/Automated-GuitarAmpModelling) project was used to train the .json models. 

The plugin uses [RTNeural](https://github.com/jatinchowdhury18/RTNeural), which is a highly optimized neural net inference engine intended for audio applications. 

## Build Instructions
