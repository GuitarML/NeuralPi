# NeuralPi

[![CI](https://github.com/GuitarML/NeuralPi/actions/workflows/cmake.yml/badge.svg)](https://github.com/GuitarML/NeuralPi/actions/workflows/cmake.yml) [![CI-Projucer](https://github.com/GuitarML/NeuralPi/actions/workflows/projucer.yml/badge.svg)](https://github.com/GuitarML/NeuralPi/actions/workflows/projucer.yml) [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-brightgreen.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Downloads](https://img.shields.io/github/downloads/GuitarML/NeuralPi/total)](https://somsubhra.github.io/github-release-stats/?username=GuitarML&repository=NeuralPi&page=1&per_page=30)

NeuralPi is a guitar pedal using neural networks to emulate real amps and pedals on a Raspberry Pi 4. The NeuralPi software is a VST3 plugin built with JUCE, which can be run as a normal audio plugin or cross-compiled to run on the Raspberry Pi 4 with [Elk Audio OS](https://elk.audio/). The NeuralPi includes model selection, EQ, and gain/volume controls from a remote instance of the plugin over WiFi. The pedal runs high quality amp/pedal models on an economical DIY setup, costing around $120 for hardware to build yourself. <br>
Check out a video demo on [YouTube](https://www.youtube.com/watch?v=_3zFD6h6Wrc)<br>
Check out the step by step build guide published on [Towards Data Science](https://towardsdatascience.com/neural-networks-for-real-time-audio-raspberry-pi-guitar-pedal-bded4b6b7f31)

![app](https://github.com/GuitarML/NeuralPi/blob/main/resources/rpi_pic.jpg)

NeuralPi can sound like an amplifier or distortion/overdrive pedal using the power of neural networks. Models trained from recordings of real amps and pedals can be loaded into the plugin for endless possiblities on your guitar. Create your own models or use custom tones from GuitarML.

WARNING: The audio output of the HiFiBerry DAC + ADC is at line level. Guitar amplifiers expect a low level electric guitar signal (instrument level). Use NeuralPi only where line level inputs are expected.

There are four main components to the guitar pedal:

1. [Raspberry Pi 4b](https://www.raspberrypi.org/products/raspberry-pi-4-model-b/)
2. [HiFiBerry DAC + ADC](https://www.hifiberry.com/shop/boards/hifiberry-dac-adc/)
3. [Elk Audio OS](https://elk.audio/)
4. NeuralPi VST3 plugin

![app](https://github.com/GuitarML/NeuralPi/blob/main/resources/neuralpi_pic.jpg)
<br>This is the normal plugin (v1.3.0), available for Windows (Standalone, VST3) and Mac (Standalone, AU, VST3). After connencting the Raspberry Pi and remote computer to the same local WiFi network, enter the RaspberryPi's IP address (keep the default ports) to enable control over WiFi. The Win/Mac plugins are fully functional guitar plugins that allow you to try out GuitarML's most advanced amp/pedal models without building the Raspberry Pi pedal.

Note: The plugin must be restarted after using the Import Tone button for changes to take effect.

## Installing the plugin

See the [Release page](https://github.com/GuitarML/NeuralPi/releases) for the cross-compiled Raspberry Pi / Elk Audio OS compatible VST3 plugin and Win/Mac installers.

After running the plugin or standalone for the first time, the two default models will be copied to the following locations. Any imported models will be copied here as well. Model files must be manually removed from these locations to perform model clean-up.
```
Mac/Linux: /home/<username>/Documents/GuitarML/NeuralPi/tones 
Windows: C:/Users/<username>/Documents/GuitarML/NeuralPi/tones
Elk Audio OS: /home/mind/Documents/GuitarML/NeuralPi/tones
```

## Conditioned Models

Starting with version 1.3, NeuralPi can load tones conditioned on the Gain parameter. The three default tones included with NeuralPi are now conditioned models (TS9 pedal, Fender Blues Jr. amp, and Blackstar HT40 amp set to overdrive channel). The conditioned model uses a neural network for the full range of the Gain/Drive parameter, rather than just a snapshot model. When a conditioned model is loaded, the Gain knob will turn red. 

## Adding New Models

Once your NeuralPi is set up, you can add new models from a remote computer using the following steps:

1. From the remote computer, run the plugin and add new models using the "Import Tone" button. Optionally, you can manually add new json files to the ```Documents/GuitarML/Chameleon/tones``` directory.
   Note: The "tones" directory is created the first time you run NeuralPi.
2. Turn on your WiFi enabled NeuralPi (see [Elk documentation](https://elk-audio.github.io/elk-docs/html/documents/working_with_elk_board.html?highlight=wifi#connecting-to-your-board) for connecting the Raspberry Pi to a local WiFi network)
3. Download the ```update_models.bat```(Windows) or ```update_models.sh```(Mac/Linux) to your remote computer. These scripts are located in the "scripts/" directory of this repository. You must change the ```rpi_ip_address``` and ```host_model_path``` to the Raspberry Pi's IP address and path to your json tones (on remote computer). The json files will be first copied from the remote computer to the NeuralPi, and then back from the NeuralPi to the remote computer. This allows updating models from the NeuralPi when you connect a new remote computer.
4. From the remote computer connected to the same local WiFi network as NeuralPi, run the ```update_models.bat```(Windows) or ```update_models.sh```(Mac/Linux) from a cmd terminal. <br><br>
Note: It is important that all models files have unique names with no spaces. <br>
Note: Ensure from the terminal output that you were able to connect over WiFi, and that the model files were copied properly. <br><br>
6. Restart both the NeuralPi and the remote instance of the NeuralPi plugin. From the remote NeuralPi GUI, enter the Raspberry Pi's IP address. As long as both devices are connected to the local WiFi network, you will be able select models from the NeuralPi plugin dropdown list to change models running on the Raspberry Pi.

IMPORTANT: The plugin uses a sort() function to order the models alphabetically. Due to differences in the behaviour of this function on Linux (Elk OS) vs. Win/Mac, you must start json filenames with a capital letter, otherwise the NeuralPi on Elk will sort models starting with a lowercase letter at the end of the list and the controller will be out of sync with the NeuralPi pedal.

## MIDI control of NeuralPi parameters

The “config_neuralpi_MIDI.json” file contains MIDI mapping of NeuralPi parameters.

The names of parameters are: "Gain", "Master", "Bass", "Mid", "Treble", "Presence", "Delay", "Reverb", "Model", "Ir".
In that json file, you can see that those parameters have been asigned to incoming MIDI CC# messages "1", "2", "3", "4, "5", "6", "7", "8", "9" and "10" respectively. But editing the file allows you to chose whatever CC# to whatever parameter, by just changing values in the “cc_number” and “parameter_name” commmands.

Sushi will listen to incoming MIDI CC# messages, will normalize (0, 127)  MIDI values range to (0, 1) Sushi range, and will set that value to correspondent parameter. For instance, if your MIDI controller sends a CC2 message with value "127", Sushi will receive that message and set "Master" parameter (“Master” is assigned to “CC2”) to be "1" (MIDI “127” value normalized to “1”).

You´ll need to copy the config file to the Raspberry, for instance through ssh over Wifi (login as root):

scp -r config_neuralpi_MIDI.json root@<rpi-ip-address>:/home/mind/config_files/

For connecting a MIDI device:

1 - Plug your MIDI device into any Raspberry USB port.

2 - Login as “mind” user, “elk” password, and run Sushi with the MIDI config:

sushi -r --multicore-processing=2 –c ~/config_files/config_neuralpi_MIDI.json &

3 – To list MIDI devices connected to the Raspberry, run:

aconnect –l

4 – You can now connect your MIDI device to Sushi either by their listed ports, or by their names. Run:

aconnect "your-listed-device-name" "Sushi"

NOTE 1: Currentlly, "Model" and "Ir" parameters are a little tricky to control. NeuralPi asigns a value to each file saved in "tones" or "Ir" directory. It divides the (0, 1) range of values by the number of files available, so for instance, if you had just 2 tone files in the directory, one of them would respond to any value in the (0, 0.49) range, and the other would respond to any value in the (0.5, 1) range. 

## To Do

Elk Audio OS also supports physical controls through [Sensei](https://github.com/elk-audio/sensei). Gain/Volume and EQ knobs can be added, as well as a LCD screen for selecting different models. One could build an actual guitar pedal with NeuralPi and any number of other digital effects and controls.

While running PyTorch locally on the Raspberry Pi might be a stretch, it is fully capable of recording high quality audio with the HiFiBerry hat. Implement a capture feature by automating the recording of input/output samples, pushing to remote computer for training, then updating the Pi with the newly trained model.

## Info
The neural network is a re-creation of the LSTM inference model from [Real-Time Guitar Amplifier Emulation with Deep Learning](https://www.mdpi.com/2076-3417/10/3/766/htm)

The [Automated-GuitarAmpModelling](https://github.com/Alec-Wright/Automated-GuitarAmpModelling) project was used to train the .json models.<br>
GuitarML maintains a [fork](https://github.com/GuitarML/Automated-GuitarAmpModelling) with a few extra helpful features, including a Colab training script.
IMPORTANT: When training models for NeuralPi, ensure that a LSTM size of 20 is used. NeuralPi is optimized to run models of this size, and other sizes are not currently compatible.
   
Note: The GuitarML fork of the Automated-GuitarAmpModelling code now contains helper scripts for training conditioned models, which are compatible with NeuralPi v1.3.

The plugin uses [RTNeural](https://github.com/jatinchowdhury18/RTNeural), which is a highly optimized neural net inference engine intended for audio applications. 

The HiFiBerry DAC+ADC card used for this project provides 192kHz/24bit Analog-to-Digital and Digital to Analog, which is industry standard for high quality audio devices. The plugin processes at 44.1kHz (specified in config file) for the neural net DSP. 

## Build Instructions

To build the plugin for use on the Raspberry Pi with Elk Audio OS, see the official [Elk Audio Documentation](https://elk-audio.github.io/elk-docs/html/documents/building_plugins_for_elk.html#vst-plugins-using-juce)

### Build with Cmake

```bash
# Clone the repository
$ git clone https://github.com/GuitarML/NeuralPi.git
$ cd NeuralPi

# initialize and set up submodules
$ git submodule update --init --recursive

# build with CMake
$ cmake -Bbuild
$ cmake --build build --config Release
```
The binaries will be located in `NeuralPi/build/NeuralPi_artefacts/`

### Build with Projucer

1. Clone or download this repository.
2. Download and install [JUCE](https://juce.com/) This project uses the "Projucer" application from the JUCE website. 
3. Download the RTNeural submodule (cd into the NeuralPi repo first):
   
   ```git submodule update --remote --recursive```
   
4. Download and extract: [json](https://github.com/nlohmann/json) Json for c++.
5. Open the NeuralPi.jucer file and in the appropriate Exporter Header Search Path field, enter the appropriate include paths.
   For example:

```
  <full-path-to>/json-develop/include
  <full-path-to>/NeuralPi/modules/RTNeural
  <full-path-to>/NeuralPi/modules/RTNeural/modules/xsimd/include
```
6. Build NeuralPi from the Juce Projucer application for the intended build target. 

Note: Make sure to build in Release mode unless actually debugging. Debug mode will not keep up with real time playing.

