/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>
#include <fstream>

//==============================================================================
NeuralPiAudioProcessor::NeuralPiAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    )//,
    //treeState(*this, nullptr, "PARAMETER", { std::make_unique<AudioParameterFloat>(GAIN_ID, GAIN_NAME, NormalisableRange<float>(-10.0f, 10.0f, 0.01f), 0.0f),
    //                std::make_unique<AudioParameterFloat>(MODEL_ID, MODEL_NAME, NormalisableRange<float>(0, 1, 1), 0),
    //                std::make_unique<AudioParameterFloat>(MASTER_ID, MASTER_NAME, NormalisableRange<float>(-36.0f, 0.0f, 0.01f), 0.0f) })

#endif
{
    setupDataDirectories();
    installTones();
    resetDirectory(userAppDataDirectory_tones);
    if (jsonFiles.size() > 0) {
        loadConfig(jsonFiles[current_model_index]);
    }
    // initialize parameters:
    addParameter(gainParam = new AudioParameterFloat(GAIN_ID, GAIN_NAME, NormalisableRange<float>(-0.0f, 1.0f, 0.01f), 0.5f));
    addParameter(masterParam = new AudioParameterFloat(MASTER_ID, MASTER_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    addParameter(modelParam = new AudioParameterFloat(MODEL_ID, MODEL_NAME, NormalisableRange<float>(0, jsonFiles.size()-1, 1), 0));

    //treeState.createAndAddParameter(std::make_unique<AudioParameterFloat>(MODEL_ID, MODEL_NAME, NormalisableRange<float>(0, jsonFiles.size() - 1, 1), 0));

}


NeuralPiAudioProcessor::~NeuralPiAudioProcessor()
{
}

//==============================================================================
const String NeuralPiAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NeuralPiAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NeuralPiAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NeuralPiAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NeuralPiAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NeuralPiAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NeuralPiAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NeuralPiAudioProcessor::setCurrentProgram (int index)
{
}

const String NeuralPiAudioProcessor::getProgramName (int index)
{
    return {};
}

void NeuralPiAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void NeuralPiAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    LSTM.reset();
}

void NeuralPiAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NeuralPiAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


void NeuralPiAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    // Setup Audio Data
    const int numSamples = buffer.getNumSamples();
    const int numInputChannels = getTotalNumInputChannels();

    // Amp =============================================================================
    if (amp_state == 1) {
        auto gain = static_cast<float> (gainParam->get());
        auto master = static_cast<float> (masterParam->get());
        auto model = static_cast<float> (modelParam->get());
        int model_index = static_cast<int>(model);
        //buffer.applyGain(ampDrive);
        //buffer.applyGain(decibelToLinear(gain));
        buffer.applyGain(gain);

		// Apply LSTM model
        if (model_loaded == 1) {
            if (current_model_index != model_index) {
                loadConfig(jsonFiles[model_index]);
                current_model_index = model_index;
                //current_model_index = modelSelect.getSelectedItemIndex();
                //setSelectedItemIndex(slider->getValue(), juce::NotificationType::dontSendNotification);
            }
            LSTM.process(buffer.getReadPointer(0), buffer.getWritePointer(0), numSamples);
        }

        //    Master Volume 
        //buffer.applyGain(ampMaster);
        buffer.applyGain(master);
        //buffer.applyGain(decibelToLinear(master));
    }
    
    for (int ch = 1; ch < buffer.getNumChannels(); ++ch)
        buffer.copyFrom(ch, 0, buffer, 0, 0, buffer.getNumSamples());
}

//==============================================================================
bool NeuralPiAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* NeuralPiAudioProcessor::createEditor()
{
    return new NeuralPiAudioProcessorEditor (*this);
}

//==============================================================================
void NeuralPiAudioProcessor::getStateInformation(MemoryBlock& destData)
{
    MemoryOutputStream stream(destData, true);

    stream.writeFloat(*gainParam);
    stream.writeFloat(*masterParam);
    stream.writeFloat(*modelParam);
}

void NeuralPiAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    MemoryInputStream stream(data, static_cast<size_t> (sizeInBytes), false);

    gainParam->setValueNotifyingHost(stream.readFloat());
    masterParam->setValueNotifyingHost(stream.readFloat());
    modelParam->setValueNotifyingHost(stream.readFloat());
}

void NeuralPiAudioProcessor::loadConfig(File configFile)
{
    this->suspendProcessing(true);
    model_loaded = 1;
    String path = configFile.getFullPathName();
    char_filename = path.toUTF8();

    LSTM.load_json(char_filename);

    this->suspendProcessing(false);
}

void NeuralPiAudioProcessor::resetDirectory(const File& file)
{
    jsonFiles.clear();
    if (file.isDirectory())
    {
        juce::Array<juce::File> results;
        file.findChildFiles(results, juce::File::findFiles, false, "*.json");
        for (int i = results.size(); --i >= 0;)
            jsonFiles.push_back(File(results.getReference(i).getFullPathName()));
    }
}

void NeuralPiAudioProcessor::addDirectory(const File& file)
{
    if (file.isDirectory())
    {
        juce::Array<juce::File> results;
        file.findChildFiles(results, juce::File::findFiles, false, "*.json");
        for (int i = results.size(); --i >= 0;)
            jsonFiles.push_back(File(results.getReference(i).getFullPathName()));
    }
}

void NeuralPiAudioProcessor::setupDataDirectories()
{
    // User app data directory
    File userAppDataTempFile = userAppDataDirectory.getChildFile("tmp.pdl");

    File userAppDataTempFile_tones = userAppDataDirectory_tones.getChildFile("tmp.pdl");


    // Create (and delete) temp file if necessary, so that user doesn't have
    // to manually create directories
    if (!userAppDataDirectory.exists()) {
        userAppDataTempFile.create();
    }
    if (userAppDataTempFile.existsAsFile()) {
        userAppDataTempFile.deleteFile();
    }

    if (!userAppDataDirectory_tones.exists()) {
        userAppDataTempFile_tones.create();
    }
    if (userAppDataTempFile_tones.existsAsFile()) {
        userAppDataTempFile_tones.deleteFile();
    }


    // Add the tones directory and update tone list
    addDirectory(userAppDataDirectory_tones);
}

void NeuralPiAudioProcessor::installTones()
//====================================================================
// Description: Checks that the default tones
//  are installed to the NeuralPi directory, and if not, 
//  copy them from the binary data in the plugin to that directory.
//
//====================================================================
{

    // Default tones
    File ts9_tone = userAppDataDirectory_tones.getFullPathName() + "/ts9_model_best.json";
    File bjdirty_tone = userAppDataDirectory_tones.getFullPathName() + "/bj_model_best.json";


    if (ts9_tone.existsAsFile() == false) {
        std::string string_command = ts9_tone.getFullPathName().toStdString();
        const char* char_ts9_tone = &string_command[0];

        std::ofstream myfile;
        myfile.open(char_ts9_tone);
        myfile << BinaryData::ts9_model_best_json;

        myfile.close();
    }

    if (bjdirty_tone.existsAsFile() == false) {
        std::string string_command = bjdirty_tone.getFullPathName().toStdString();
        const char* char_bjdirty = &string_command[0];

        std::ofstream myfile;
        myfile.open(char_bjdirty);
        myfile << BinaryData::bj_model_best_json;

        myfile.close();
    }
    
}

float NeuralPiAudioProcessor::convertLogScale(float in_value, float x_min, float x_max, float y_min, float y_max)
{
    float b = log(y_max / y_min) / (x_max - x_min);
    float a = y_max / exp(b * x_max);
    float converted_value = a * exp(b * in_value);
    return converted_value;
}

/*
void NeuralPiAudioProcessor::set_ampDrive(float db_ampDrive)
{
    gainParam = decibelToLinear(db_ampDrive);
    ampGainKnobState = db_ampDrive;
}

void NeuralPiAudioProcessor::set_ampMaster(float db_ampMaster)
{
    ampMasterKnobState = db_ampMaster;
    if (db_ampMaster == -48.0) {
        ampMaster = decibelToLinear(-100.0);
    } else {
        ampMaster = decibelToLinear(db_ampMaster);
    }
}
*/

float NeuralPiAudioProcessor::decibelToLinear(float dbValue)
{
    return powf(10.0, dbValue/20.0);
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NeuralPiAudioProcessor();
}
