/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "AmpOSCReceiver.h"
#include <stdio.h>
#include <fstream>
#include <iostream>


//==============================================================================
NeuralPiAudioProcessorEditor::NeuralPiAudioProcessorEditor (NeuralPiAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to

    //modelSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, MODEL_ID, modelKnob);
    addAndMakeVisible(modelKnob);
    //ampGainKnob.setLookAndFeel(&ampSilverKnobLAF);
    modelKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    modelKnob.setNumDecimalPlacesToDisplay(1);
    modelKnob.addListener(this);
    modelKnob.setRange(0, processor.jsonFiles.size() - 1);
    modelKnob.setValue(processor.current_model_index);
    modelKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    modelKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    modelKnob.setNumDecimalPlacesToDisplay(1);
    modelKnob.setDoubleClickReturnValue(true, 0.0);

    auto modelValue = getParameterValue(modelName);
    Slider& modelSlider = getModelSlider();
    modelSlider.setValue(modelValue, NotificationType::dontSendNotification);

    modelKnob.onValueChange = [this]
    {
        const float sliderValue = static_cast<float> (getModelSlider().getValue());
        const float modelValue = getParameterValue(modelName);

        if (!approximatelyEqual(modelValue, sliderValue))
        {
            setParameterValue(modelName, sliderValue);

            // create and send an OSC message with an address and a float value:
            float value = static_cast<float> (getModelSlider().getValue());

            if (!oscSender.send(modelAddressPattern, value))
            {
                updateOutConnectedLabel(false);
            }
            else
            {
                DBG("Sent value " + String(value) + " to AP " + modelAddressPattern);
            }
        }
    };
    
    
    addAndMakeVisible(modelSelect);
    modelSelect.setColour(juce::Label::textColourId, juce::Colours::black);
    int c = 1;
    for (const auto& jsonFile : processor.jsonFiles) {
        modelSelect.addItem(jsonFile.getFileNameWithoutExtension(), c);
        c += 1;
    }
    modelSelect.onChange = [this] {modelSelectChanged(); };
    modelSelect.setSelectedItemIndex(processor.current_model_index, juce::NotificationType::dontSendNotification);
    modelSelect.setScrollWheelEnabled(true);

    addAndMakeVisible(loadButton);
    loadButton.setButtonText("Import Tone");
    loadButton.setColour(juce::Label::textColourId, juce::Colours::black);
    loadButton.addListener(this);

    //gainSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, GAIN_ID, ampGainKnob);
    addAndMakeVisible(ampGainKnob);
    //ampGainKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampGainKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampGainKnob.setNumDecimalPlacesToDisplay(1);
    ampGainKnob.addListener(this);
    ampGainKnob.setRange(-12.0, 12.0);
    ampGainKnob.setValue(processor.ampGainKnobState);
    ampGainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampGainKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampGainKnob.setNumDecimalPlacesToDisplay(1);
    ampGainKnob.setDoubleClickReturnValue(true, 0.0);

    auto gainValue = getParameterValue(gainName);
    Slider& gainSlider = getGainSlider();
    gainSlider.setValue(gainValue, NotificationType::dontSendNotification);

    ampGainKnob.onValueChange = [this]
    {
        const float sliderValue = static_cast<float> (getGainSlider().getValue());
        const float gainValue = getParameterValue(gainName);

        if (!approximatelyEqual(gainValue, sliderValue))
        {
            setParameterValue(gainName, sliderValue);

            // create and send an OSC message with an address and a float value:
            float value = static_cast<float> (getGainSlider().getValue());

            if (!oscSender.send(gainAddressPattern, value))
            {
                updateOutConnectedLabel(false);
            }
            else
            {
                DBG("Sent value " + String(value) + " to AP " + gainAddressPattern);
            }
        }
    };

    //masterSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, MASTER_ID, ampMasterKnob);
    addAndMakeVisible(ampMasterKnob);
    //ampMasterKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampMasterKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampMasterKnob.setNumDecimalPlacesToDisplay(1);
    ampMasterKnob.addListener(this);
    ampMasterKnob.setRange(-48.0, 0.0);
    ampMasterKnob.setValue(processor.ampMasterKnobState);
    ampMasterKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampMasterKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20 );
    ampMasterKnob.setNumDecimalPlacesToDisplay(1);
    ampMasterKnob.setDoubleClickReturnValue(true, -24.0);

    auto masterValue = getParameterValue(masterName);
    Slider& masterSlider = getMasterSlider();
    masterSlider.setValue(masterValue, NotificationType::dontSendNotification);

    ampMasterKnob.onValueChange = [this]
    {
        const float sliderValue = static_cast<float> (getMasterSlider().getValue());
        const float masterValue = getParameterValue(masterName);

        if (!approximatelyEqual(masterValue, sliderValue))
        {
            setParameterValue(masterName, sliderValue);

            // create and send an OSC message with an address and a float value:
            float value = static_cast<float> (getMasterSlider().getValue());

            if (!oscSender.send(masterAddressPattern, value))
            {
                updateOutConnectedLabel(false);
            }
            else
            {
                DBG("Sent value " + String(value) + " to AP " + masterAddressPattern);
            }
        }
    };

    addAndMakeVisible(GainLabel);
    GainLabel.setText("Gain", juce::NotificationType::dontSendNotification);
    GainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(LevelLabel);
    LevelLabel.setText("Level", juce::NotificationType::dontSendNotification);
    LevelLabel.setJustificationType(juce::Justification::centred);
    auto font = GainLabel.getFont();
    float height = font.getHeight();
    font.setHeight(height); // 0.75);
    GainLabel.setFont(font);
    LevelLabel.setFont(font);


    // Name controls:
    addAndMakeVisible(ampNameLabel);
    ampNameField.setEditable(true, true, true);
    addAndMakeVisible(ampNameField);

    // IP controls:
    ipField.setEditable(true, true, true);
    addAndMakeVisible(ipLabel);
    addAndMakeVisible(ipField);

    // Port controls:
    addAndMakeVisible(outPortNumberLabel);
    outPortNumberField.setEditable(true, true, true);
    addAndMakeVisible(outPortNumberField);
    addAndMakeVisible(outConnectedLabel);

    addAndMakeVisible(inPortNumberLabel);
    inPortNumberField.setEditable(true, true, true);
    addAndMakeVisible(inPortNumberField);
    addAndMakeVisible(inConnectedLabel);


    // OSC messaging

    getInPortNumberField().addListener(this);
    getAmpNameField().addListener(this);
    getOutPortNumberField().addListener(this);
    getIPField().addListener(this);

    oscReceiver.getGainValue().addListener(this);
    oscReceiver.getMasterValue().addListener(this);

    oscReceiver.getModelValue().addListener(this);

    updateInConnectedLabel();

    connectSender();


    // Size of plugin GUI
    setSize(250, 350);

}

NeuralPiAudioProcessorEditor::~NeuralPiAudioProcessorEditor()
{
}

//==============================================================================
void NeuralPiAudioProcessorEditor::paint (Graphics& g)
{
    //background = ImageCache::getFromMemory(BinaryData::pedal_blank_png, BinaryData::pedal_blank_pngSize);
   
    //g.drawImageAt(background, 0, 0);

    g.setColour (Colours::white);
    g.setFont (15.0f);

}

void NeuralPiAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    modelSelect.setBounds(15, 10, 210, 25);
    loadButton.setBounds(15, 42, 100, 25);
    modelKnob.setBounds(140, 40, 75, 95);

    // Amp Widgets
    ampGainKnob.setBounds(30, 85, 75, 95);
    ampMasterKnob.setBounds(140, 120, 75, 95);
    GainLabel.setBounds(28, 163, 80, 10);
    LevelLabel.setBounds(138, 200, 80, 10);

    addAndMakeVisible(ampNameLabel);
    ampNameField.setEditable(true, true, true);
    addAndMakeVisible(ampNameField);

    // IP controls:
    ipField.setBounds(130, 220, 100, 25);
    ipLabel.setBounds(15, 220, 100, 25);

    // Port controls:
    outPortNumberLabel.setBounds(15, 260, 90, 25);
    outPortNumberField.setBounds(130, 260, 100, 25);
    inPortNumberLabel.setBounds(15, 300, 90, 25);
    inPortNumberField.setBounds(130, 300, 100, 25);
}

void NeuralPiAudioProcessorEditor::modelSelectChanged()
{
    const int selectedFileIndex = modelSelect.getSelectedItemIndex();
    if (selectedFileIndex >= 0 && selectedFileIndex < processor.jsonFiles.size()) {
        processor.loadConfig(processor.jsonFiles[selectedFileIndex]);
        processor.current_model_index = modelSelect.getSelectedItemIndex();
    }
    modelKnob.setValue(processor.current_model_index);
}

void NeuralPiAudioProcessorEditor::loadButtonClicked()
{
    FileChooser chooser("Select one or more .json tone files to import",
        {},
        "*.json");
    if (chooser.browseForMultipleFilesToOpen())
    {
        int import_fail = 1;
        Array<File> files = chooser.getResults();
        for (auto file : files) {
            File fullpath = processor.userAppDataDirectory_tones.getFullPathName() + "/" + file.getFileName();
            bool b = fullpath.existsAsFile();
            if (b == false) {

                processor.loadConfig(file);
                fname = file.getFileName();
                processor.loaded_tone = file;
                processor.loaded_tone_name = fname;
                processor.custom_tone = 1;

                // Copy selected file to model directory and load into dropdown menu
                bool a = file.copyFileTo(fullpath);
                if (a == true) {
                    modelSelect.addItem(file.getFileNameWithoutExtension(), processor.jsonFiles.size() + 1);
                    modelSelect.setSelectedItemIndex(processor.jsonFiles.size(), juce::NotificationType::dontSendNotification);
                    processor.jsonFiles.push_back(file);
                }
            }
        }
    }
}


void NeuralPiAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &loadButton) {
        loadButtonClicked();
    }
}


void NeuralPiAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    // Amp
    if (slider == &ampGainKnob)
        processor.set_ampDrive(slider->getValue());
    else if (slider == &ampMasterKnob)
        processor.set_ampMaster(slider->getValue());
    else if (slider == &modelKnob)
        if (slider->getValue() >= 0 && slider->getValue() < processor.jsonFiles.size()) {
            processor.loadConfig(processor.jsonFiles[slider->getValue()]);
            processor.current_model_index = modelSelect.getSelectedItemIndex();
            modelSelect.setSelectedItemIndex(slider->getValue(), juce::NotificationType::dontSendNotification);
        }
        
}

// OSC Messages

Slider& NeuralPiAudioProcessorEditor::getGainSlider()
{
    return ampGainKnob;
}

Slider& NeuralPiAudioProcessorEditor::getMasterSlider()
{
    return ampMasterKnob;
}

Slider& NeuralPiAudioProcessorEditor::getModelSlider()
{
    return modelKnob;
}


Label& NeuralPiAudioProcessorEditor::getOutPortNumberField()
{
    return outPortNumberField;
}

Label& NeuralPiAudioProcessorEditor::getInPortNumberField()
{
    return inPortNumberField;
}

Label& NeuralPiAudioProcessorEditor::getIPField()
{
    return ipField;
}

Label& NeuralPiAudioProcessorEditor::getAmpNameField()
{
    return ampNameField;
}

Label& NeuralPiAudioProcessorEditor::getOutConnectedLabel()
{
    return outConnectedLabel;
}

Label& NeuralPiAudioProcessorEditor::getInConnectedLabel()
{
    return inConnectedLabel;
}

void NeuralPiAudioProcessorEditor::buildAddressPatterns()
{
    gainAddressPattern = "/parameter/" + ampName + "/Gain";
    masterAddressPattern = "/parameter/" + ampName + "/Master";
    modelAddressPattern = "/parameter/" + ampName + "/Model";
}

void NeuralPiAudioProcessorEditor::connectSender()
{
    // specify here where to send OSC messages to: host URL and UDP port number
    if (!oscSender.connect(outgoingIP, outgoingPort))
    {
        updateOutConnectedLabel(false);
    }
    else
    {
        updateOutConnectedLabel(true);
    }
}

void NeuralPiAudioProcessorEditor::updateOutgoingIP(String ip)
{
    outgoingIP = ip;
    connectSender();
}

void NeuralPiAudioProcessorEditor::updateOutgoingPort(int port)
{
    outgoingPort = port;
    connectSender();
}

void NeuralPiAudioProcessorEditor::labelTextChanged(Label* labelThatHasChanged)
{
    if (labelThatHasChanged == &getInPortNumberField())
    {
        const int newPort = getInPortNumberField().getTextValue().toString().getIntValue();
        oscReceiver.changePort(newPort);
        updateInConnectedLabel();
    }
    else if (labelThatHasChanged == &getOutPortNumberField())
    {
        const int newPort = getOutPortNumberField().getTextValue().toString().getIntValue();
        updateOutgoingPort(newPort);
    }
    else if (labelThatHasChanged == &getIPField())
    {
        const String newIP = getIPField().getTextValue().toString();
        updateOutgoingIP(newIP);
    }
    //else if (labelThatHasChanged == getAmpNameField())
    //{
    //    ampName = getAmpNameField().getTextValue().toString();
    //    buildAddressPatterns();
    //    oscReceiver.updateAmpName(getAmpNameField().getTextValue().toString());
    //}
}

void NeuralPiAudioProcessorEditor::updateInConnectedLabel()
{
    const bool connected = oscReceiver.isConnected();
    if (connected)
    {
        getInConnectedLabel().setText("(Connected)", dontSendNotification);
    }
    else
    {
        getInConnectedLabel().setText("(Disconnected!)", dontSendNotification);
    }
}

void NeuralPiAudioProcessorEditor::updateOutConnectedLabel(bool connected)
{
    if (connected)
    {
        getOutConnectedLabel().setText("(Connected)", dontSendNotification);
    }
    else
    {
        getOutConnectedLabel().setText("(Disconnected!)", dontSendNotification);
    }
}

// This callback is invoked if an OSC message has been received setting either value.
void NeuralPiAudioProcessorEditor::valueChanged(Value& value)
{
    if (value.refersToSameSourceAs(oscReceiver.getGainValue()))
    {
        if (!approximatelyEqual(static_cast<double> (value.getValue()), getGainSlider().getValue()))
        {
            getGainSlider().setValue(static_cast<double> (value.getValue()),
                NotificationType::sendNotification);
        }
    }
    else if (value.refersToSameSourceAs(oscReceiver.getMasterValue()))
    {
        if (!approximatelyEqual(static_cast<double> (value.getValue()), getMasterSlider().getValue()))
        {
            getMasterSlider().setValue(static_cast<double> (value.getValue()),
                NotificationType::sendNotification);
        }
    }
    else if (value.refersToSameSourceAs(oscReceiver.getModelValue()))
    {
        if (!approximatelyEqual(static_cast<double> (value.getValue()), getModelSlider().getValue()))
        {
            getModelSlider().setValue(static_cast<double> (value.getValue()),
                NotificationType::sendNotification);
        }
    }
}

void NeuralPiAudioProcessorEditor::timerCallback()
{
    getGainSlider().setValue(getParameterValue(gainName), NotificationType::dontSendNotification);
    getMasterSlider().setValue(getParameterValue(masterName), NotificationType::dontSendNotification);
    getModelSlider().setValue(getParameterValue(modelName), NotificationType::dontSendNotification);
}

AudioProcessorParameter* NeuralPiAudioProcessorEditor::getParameter(const String& paramId)
{
    if (auto* proc = getAudioProcessor())
    {
        auto& params = proc->getParameters();

        for (auto p : params)
        {
            if (auto* param = dynamic_cast<AudioProcessorParameterWithID*> (p))
            {
                if (param->paramID == paramId)
                    return param;
            }
        }
    }

    return nullptr;
}

float NeuralPiAudioProcessorEditor::getParameterValue(const String& paramId)
{
    if (auto* param = getParameter(paramId))
        return param->getValue();

    return 0.0f;
}

void NeuralPiAudioProcessorEditor::setParameterValue(const String& paramId, float value)
{
    if (auto* param = getParameter(paramId))
        param->setValueNotifyingHost(value);
}