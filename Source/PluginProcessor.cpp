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
    )

#endif
{
    setupDataDirectories();
    installTones();
    resetDirectory(userAppDataDirectory_tones);
    // Sort jsonFiles alphabetically
    std::sort(jsonFiles.begin(), jsonFiles.end());
    if (jsonFiles.size() > 0) {
        loadConfig(jsonFiles[current_model_index]);
    }

    resetDirectoryIR(userAppDataDirectory_irs);
    // Sort irFiles alphabetically
    std::sort(irFiles.begin(), irFiles.end());
    if (irFiles.size() > 0) {
        loadIR(irFiles[current_ir_index]);
    }

    // initialize parameters:
    addParameter(gainParam = new AudioParameterFloat(GAIN_ID, GAIN_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    addParameter(masterParam = new AudioParameterFloat(MASTER_ID, MASTER_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    addParameter(bassParam = new AudioParameterFloat(BASS_ID, BASS_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    addParameter(midParam = new AudioParameterFloat(MID_ID, MID_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    addParameter(trebleParam = new AudioParameterFloat(TREBLE_ID, TREBLE_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    addParameter(presenceParam = new AudioParameterFloat(PRESENCE_ID, PRESENCE_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    addParameter(modelParam = new AudioParameterFloat(MODEL_ID, MODEL_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.0f));
    addParameter(irParam = new AudioParameterFloat(IR_ID, IR_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.0f));
    addParameter(delayParam = new AudioParameterFloat(DELAY_ID, DELAY_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.0f));
    addParameter(reverbParam = new AudioParameterFloat(REVERB_ID, REVERB_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.0f));
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

    // set up DC blocker
    dcBlocker.coefficients = dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 35.0f);
    dsp::ProcessSpec spec{ sampleRate, static_cast<uint32> (samplesPerBlock), 2 };
    dcBlocker.prepare(spec);

    // Set up IR
    cabSimIR.prepare(spec);

    // fx chain
    fxChain.prepare(spec);    
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
    const int sampleRate = getSampleRate();

    auto block = dsp::AudioBlock<float>(buffer).getSingleChannelBlock(0);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);

    // Amp =============================================================================
    if (amp_state == 1) {
        auto gain = static_cast<float> (gainParam->get());
        auto master = static_cast<float> (masterParam->get());
        // Note: Default 0.0 -> 1.0 param range is converted to +-12.0 here
        auto bass = (static_cast<float> (bassParam->get() - 0.5) * 24.0);
        auto mid = (static_cast<float> (midParam->get() - 0.5) * 24.0);
        auto treble = (static_cast<float> (trebleParam->get() - 0.5) * 24.0);
        auto presence = (static_cast<float> (presenceParam->get() - 0.5) * 24.0);

        auto delay = (static_cast<float> (delayParam->get()));
        auto reverb = (static_cast<float> (reverbParam->get()));

        auto model = static_cast<float> (modelParam->get());
        model_index = getModelIndex(model);

        auto ir = static_cast<float> (irParam->get());
        ir_index = getIrIndex(ir);

        // Applying gain adjustment for snapshot models
        if (LSTM.input_size == 1) {
            buffer.applyGain(gain * 2.0);
        } 

        // Process EQ
        eq4band.setParameters(bass, mid, treble, presence);// Better to move this somewhere else? Only need to set when value changes
        eq4band.process(buffer.getReadPointer(0), buffer.getWritePointer(0), midiMessages, numSamples, numInputChannels, sampleRate);

        // Apply LSTM model
        if (model_loaded == 1 && lstm_state == true) {
            if (current_model_index != model_index) {
                loadConfig(jsonFiles[model_index]);
                current_model_index = model_index;
            }

            // Process LSTM based on input_size (snapshot model or conditioned model)
            if (LSTM.input_size == 1) {
                LSTM.process(buffer.getReadPointer(0), buffer.getWritePointer(0), numSamples);
            }  
            else if (LSTM.input_size == 2) {
                LSTM.process(buffer.getReadPointer(0), gain, buffer.getWritePointer(0), numSamples);
            }
            else if (LSTM.input_size == 3) {
                LSTM.process(buffer.getReadPointer(0), gain, master, buffer.getWritePointer(0), numSamples);
            }
        }

        // Process IR
        if (ir_state == true && num_irs > 0) {
            if (current_ir_index != ir_index) {
                loadIR(irFiles[ir_index]);
                current_ir_index = ir_index;
            }
            auto block = dsp::AudioBlock<float>(buffer).getSingleChannelBlock(0);
            auto context = juce::dsp::ProcessContextReplacing<float>(block);
            cabSimIR.process(context);

            // IR generally makes output quieter, add volume here to make ir on/off volume more even
            buffer.applyGain(2.0);
        }

        //    Master Volume 
		if (LSTM.input_size == 1 || LSTM.input_size == 2) {
			buffer.applyGain(master * 2.0); // Adding volume range (2x) mainly for clean models
		}

        // Process Delay, and Reverb
        set_delayParams(delay);
        set_reverbParams(reverb);
        fxChain.process(context);
    }

    // process DC blocker
    auto monoBlock = dsp::AudioBlock<float>(buffer).getSingleChannelBlock(0);
    dcBlocker.process(dsp::ProcessContextReplacing<float>(monoBlock));
    
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
    stream.writeFloat(*bassParam);
    stream.writeFloat(*midParam);
    stream.writeFloat(*trebleParam);
    stream.writeFloat(*presenceParam);
    stream.writeFloat(*modelParam);
    stream.writeFloat(*irParam);
    stream.writeFloat(*delayParam);
    stream.writeFloat(*reverbParam);
}

void NeuralPiAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    MemoryInputStream stream(data, static_cast<size_t> (sizeInBytes), false);

    gainParam->setValueNotifyingHost(stream.readFloat());
    masterParam->setValueNotifyingHost(stream.readFloat());
    bassParam->setValueNotifyingHost(stream.readFloat());
    midParam->setValueNotifyingHost(stream.readFloat());
    trebleParam->setValueNotifyingHost(stream.readFloat());
    presenceParam->setValueNotifyingHost(stream.readFloat());
    modelParam->setValueNotifyingHost(stream.readFloat());
    irParam->setValueNotifyingHost(stream.readFloat());
    delayParam->setValueNotifyingHost(stream.readFloat());
    reverbParam->setValueNotifyingHost(stream.readFloat());
}

int NeuralPiAudioProcessor::getModelIndex(float model_param)
{
    int a = static_cast<int>(round(model_param * (num_models - 1.0)));
    if (a > num_models - 1) {
        a = num_models - 1;
    }
    else if (a < 0) {
        a = 0;
    }
    return a;
}

int NeuralPiAudioProcessor::getIrIndex(float ir_param)
{
    int a = static_cast<int>(round(ir_param * (num_irs - 1.0)));
    if (a > num_irs - 1) {
        a = num_irs - 1;
    }
    else if (a < 0) {
        a = 0;
    }
    return a;
}

void NeuralPiAudioProcessor::loadConfig(File configFile)
{
    this->suspendProcessing(true);
    String path = configFile.getFullPathName();
    char_filename = path.toUTF8();

    try {
        // Load the JSON file into the correct model
        LSTM.load_json(char_filename);
    
        // Check what the input size is and then update the GUI appropirately
        if (LSTM.input_size == 1) {
            params = 0;
        }
        else if (LSTM.input_size == 2) {
            params = 1;
        }
        else if (LSTM.input_size == 3) {
            params = 2;
        }
        
        // If we are good: let's say so
        model_loaded = 1;
    }
    catch (const std::exception& e) {
        DBG("Unable to load json file: " + configFile.getFullPathName());
        std::cout << e.what();
    }

    this->suspendProcessing(false);
}

void NeuralPiAudioProcessor::loadIR(File irFile)
{
    this->suspendProcessing(true);

    try {
        cabSimIR.load(irFile);
        ir_loaded = 1;
    }
    catch (const std::exception& e) {
        DBG("Unable to load IR file: " + irFile.getFullPathName());
        std::cout << e.what();
    }
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

void NeuralPiAudioProcessor::resetDirectoryIR(const File& file)
{
    irFiles.clear();
    if (file.isDirectory())
    {
        juce::Array<juce::File> results;
        file.findChildFiles(results, juce::File::findFiles, false, "*.wav");
        for (int i = results.size(); --i >= 0;)
            irFiles.push_back(File(results.getReference(i).getFullPathName()));
    }
}

void NeuralPiAudioProcessor::addDirectory(const File& file)
{
    if (file.isDirectory())
    {
        juce::Array<juce::File> results;
        file.findChildFiles(results, juce::File::findFiles, false, "*.json");
        for (int i = results.size(); --i >= 0;)
        {
            jsonFiles.push_back(File(results.getReference(i).getFullPathName()));
            num_models = num_models + 1.0;
        }
    }
}

void NeuralPiAudioProcessor::addDirectoryIR(const File& file)
{
    if (file.isDirectory())
    {
        juce::Array<juce::File> results;
        file.findChildFiles(results, juce::File::findFiles, false, "*.wav");
        for (int i = results.size(); --i >= 0;)
        {
            irFiles.push_back(File(results.getReference(i).getFullPathName()));
            num_irs = num_irs + 1.0;
        }
    }
}

void NeuralPiAudioProcessor::setupDataDirectories()
{
    // User app data directory
    File userAppDataTempFile = userAppDataDirectory.getChildFile("tmp.pdl");

    File userAppDataTempFile_tones = userAppDataDirectory_tones.getChildFile("tmp.pdl");

    File userAppDataTempFile_irs = userAppDataDirectory_irs.getChildFile("tmp.pdl");

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

    if (!userAppDataDirectory_irs.exists()) {
        userAppDataTempFile_irs.create();
    }
    if (userAppDataTempFile_irs.existsAsFile()) {
        userAppDataTempFile_irs.deleteFile();
    }


    // Add the tones directory and update tone list
    addDirectory(userAppDataDirectory_tones);
    addDirectoryIR(userAppDataDirectory_irs);
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
    File ts9_tone = userAppDataDirectory_tones.getFullPathName() + "/TS9.json";
    File bjdirty_tone = userAppDataDirectory_tones.getFullPathName() + "/BluesJR.json";
    File ht40od_tone = userAppDataDirectory_tones.getFullPathName() + "/HT40_Overdrive.json";

    if (ts9_tone.existsAsFile() == false) {
        std::string string_command = ts9_tone.getFullPathName().toStdString();
        const char* char_ts9_tone = &string_command[0];

        std::ofstream myfile;
        myfile.open(char_ts9_tone);
        myfile << BinaryData::TS9_json;

        myfile.close();
    }

    if (bjdirty_tone.existsAsFile() == false) {
        std::string string_command = bjdirty_tone.getFullPathName().toStdString();
        const char* char_bjdirty = &string_command[0];

        std::ofstream myfile;
        myfile.open(char_bjdirty);
        myfile << BinaryData::BluesJr_json;

        myfile.close();
    }

    if (ht40od_tone.existsAsFile() == false) {
        std::string string_command = ht40od_tone.getFullPathName().toStdString();
        const char* char_ht40od = &string_command[0];

        std::ofstream myfile;
        myfile.open(char_ht40od);
        myfile << BinaryData::HT40_Overdrive_json;

        myfile.close();
    }
    
}

void NeuralPiAudioProcessor::set_ampEQ(float bass_slider, float mid_slider, float treble_slider, float presence_slider)
{
    eq4band.setParameters(bass_slider, mid_slider, treble_slider, presence_slider);
}

void NeuralPiAudioProcessor::set_delayParams(float paramValue)
{
    auto& del = fxChain.template get<delayIndex>();
    del.setWetLevel(paramValue);
    // Setting delay time as larger steps to minimize clicking, and to start delay time at a reasonable value
    if (paramValue < 0.25) {
        del.setDelayTime(0, 0.25);
    } else if (paramValue < 0.5) {
        del.setDelayTime(0, 0.5);
    } else if (paramValue < 0.75) {
        del.setDelayTime(0, 0.75);
    } else {
        del.setDelayTime(0, 1.0);
    }
    del.setFeedback(0.8-paramValue/2);
}


void NeuralPiAudioProcessor::set_reverbParams(float paramValue)
{
    auto& rev = fxChain.template get<reverbIndex>();
    rev_params = rev.getParameters();

    // Sets reverb params as a function of a single reverb param value ( 0.0 to 1.0)
    rev_params.wetLevel = paramValue;
    rev_params.damping = 0.6 - paramValue/2; // decay is inverse of damping
    rev_params.roomSize = 0.8 - paramValue/2;
    //rev_params.width = paramValue;
    rev.setParameters(rev_params);
}

float NeuralPiAudioProcessor::convertLogScale(float in_value, float x_min, float x_max, float y_min, float y_max)
{
    float b = log(y_max / y_min) / (x_max - x_min);
    float a = y_max / exp(b * x_max);
    float converted_value = a * exp(b * in_value);
    return converted_value;
}


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
