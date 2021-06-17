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

    //addAndMakeVisible(modelKnob);
    //ampGainKnob.setLookAndFeel(&ampSilverKnobLAF);
    modelKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    modelKnob.setNumDecimalPlacesToDisplay(1);
    modelKnob.addListener(this);
    //modelKnob.setRange(0, processor.jsonFiles.size() - 1);
    modelKnob.setRange(0.0, 1.0);
    modelKnob.setValue(0.0);
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
    ampGainKnob.setRange(0.0, 1.0);
    ampGainKnob.setValue(0.5);
    ampGainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampGainKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampGainKnob.setNumDecimalPlacesToDisplay(1);
    ampGainKnob.setDoubleClickReturnValue(true, 0.5);

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

    addAndMakeVisible(ampMasterKnob);
    //ampMasterKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampMasterKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampMasterKnob.setNumDecimalPlacesToDisplay(1);
    ampMasterKnob.addListener(this);
    ampMasterKnob.setRange(0.0, 1.0);
    ampMasterKnob.setValue(0.5);
    ampMasterKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampMasterKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20 );
    ampMasterKnob.setNumDecimalPlacesToDisplay(1);
    ampMasterKnob.setDoubleClickReturnValue(true, 0.5);

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


    addAndMakeVisible(ampBassKnob);
    ampBassKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampBassKnob.setNumDecimalPlacesToDisplay(1);
    ampBassKnob.addListener(this);
    ampBassKnob.setRange(0.0, 1.0);
    ampBassKnob.setValue(0.5);
    ampBassKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampBassKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampBassKnob.setNumDecimalPlacesToDisplay(1);
    ampBassKnob.setDoubleClickReturnValue(true, 0.5);

    auto bassValue = getParameterValue(bassName);
    Slider& bassSlider = getBassSlider();
    bassSlider.setValue(bassValue, NotificationType::dontSendNotification);

    ampBassKnob.onValueChange = [this]
    {
        const float sliderValue = static_cast<float> (getBassSlider().getValue());
        const float bassValue = getParameterValue(bassName);

        if (!approximatelyEqual(bassValue, sliderValue))
        {
            setParameterValue(bassName, sliderValue);

            // create and send an OSC message with an address and a float value:
            float value = static_cast<float> (getBassSlider().getValue());

            if (!oscSender.send(bassAddressPattern, value))
            {
                updateOutConnectedLabel(false);
            }
            else
            {
                DBG("Sent value " + String(value) + " to AP " + bassAddressPattern);
            }
        }
    };

    addAndMakeVisible(ampMidKnob);
    ampMidKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampMidKnob.setNumDecimalPlacesToDisplay(1);
    ampMidKnob.addListener(this);
    ampMidKnob.setRange(0.0, 1.0);
    ampMidKnob.setValue(0.5);
    ampMidKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampMidKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampMidKnob.setNumDecimalPlacesToDisplay(1);
    ampMidKnob.setDoubleClickReturnValue(true, 0.5);

    auto midValue = getParameterValue(midName);
    Slider& midSlider = getMidSlider();
    midSlider.setValue(midValue, NotificationType::dontSendNotification);

    ampMidKnob.onValueChange = [this]
    {
        const float sliderValue = static_cast<float> (getMidSlider().getValue());
        const float midValue = getParameterValue(midName);

        if (!approximatelyEqual(midValue, sliderValue))
        {
            setParameterValue(midName, sliderValue);

            // create and send an OSC message with an address and a float value:
            float value = static_cast<float> (getMidSlider().getValue());

            if (!oscSender.send(midAddressPattern, value))
            {
                updateOutConnectedLabel(false);
            }
            else
            {
                DBG("Sent value " + String(value) + " to AP " + midAddressPattern);
            }
        }
    };

    addAndMakeVisible(ampTrebleKnob);
    ampTrebleKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampTrebleKnob.setNumDecimalPlacesToDisplay(1);
    ampTrebleKnob.addListener(this);
    ampTrebleKnob.setRange(0.0, 1.0);
    ampTrebleKnob.setValue(0.5);
    ampTrebleKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampTrebleKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampTrebleKnob.setNumDecimalPlacesToDisplay(1);
    ampTrebleKnob.setDoubleClickReturnValue(true, 0.5);

    auto trebleValue = getParameterValue(trebleName);
    Slider& trebleSlider = getTrebleSlider();
    trebleSlider.setValue(trebleValue, NotificationType::dontSendNotification);

    ampTrebleKnob.onValueChange = [this]
    {
        const float sliderValue = static_cast<float> (getTrebleSlider().getValue());
        const float trebleValue = getParameterValue(trebleName);

        if (!approximatelyEqual(trebleValue, sliderValue))
        {
            setParameterValue(trebleName, sliderValue);

            // create and send an OSC message with an address and a float value:
            float value = static_cast<float> (getTrebleSlider().getValue());

            if (!oscSender.send(trebleAddressPattern, value))
            {
                updateOutConnectedLabel(false);
            }
            else
            {
                DBG("Sent value " + String(value) + " to AP " + trebleAddressPattern);
            }
        }
    };

    addAndMakeVisible(ampPresenceKnob);
    ampPresenceKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampPresenceKnob.setNumDecimalPlacesToDisplay(1);
    ampPresenceKnob.addListener(this);
    ampPresenceKnob.setRange(0.0, 1.0);
    ampPresenceKnob.setValue(0.5);
    ampPresenceKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampPresenceKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampPresenceKnob.setNumDecimalPlacesToDisplay(1);
    ampPresenceKnob.setDoubleClickReturnValue(true, 0.5);

    auto presenceValue = getParameterValue(trebleName);
    Slider& presenceSlider = getPresenceSlider();
    trebleSlider.setValue(presenceValue, NotificationType::dontSendNotification);

    ampPresenceKnob.onValueChange = [this]
    {
        const float sliderValue = static_cast<float> (getPresenceSlider().getValue());
        const float presenceValue = getParameterValue(presenceName);

        if (!approximatelyEqual(presenceValue, sliderValue))
        {
            setParameterValue(presenceName, sliderValue);

            // create and send an OSC message with an address and a float value:
            float value = static_cast<float> (getPresenceSlider().getValue());

            if (!oscSender.send(presenceAddressPattern, value))
            {
                updateOutConnectedLabel(false);
            }
            else
            {
                DBG("Sent value " + String(value) + " to AP " + presenceAddressPattern);
            }
        }
    };

    addAndMakeVisible(GainLabel);
    GainLabel.setText("Gain", juce::NotificationType::dontSendNotification);
    GainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(LevelLabel);
    LevelLabel.setText("Level", juce::NotificationType::dontSendNotification);
    LevelLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(BassLabel);
    BassLabel.setText("Bass", juce::NotificationType::dontSendNotification);
    BassLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(MidLabel);
    MidLabel.setText("Mid", juce::NotificationType::dontSendNotification);
    MidLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(TrebleLabel);
    TrebleLabel.setText("Treble", juce::NotificationType::dontSendNotification);
    TrebleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(PresenceLabel);
    PresenceLabel.setText("Presence", juce::NotificationType::dontSendNotification);
    PresenceLabel.setJustificationType(juce::Justification::centred);

    auto font = GainLabel.getFont();
    float height = font.getHeight();
    font.setHeight(height); // 0.75);
    GainLabel.setFont(font);
    LevelLabel.setFont(font);
    BassLabel.setFont(font);
    MidLabel.setFont(font);
    TrebleLabel.setFont(font);
    PresenceLabel.setFont(font);

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

    oscReceiver.getBassValue().addListener(this);
    oscReceiver.getMidValue().addListener(this);
    oscReceiver.getTrebleValue().addListener(this);
    oscReceiver.getPresenceValue().addListener(this);

    oscReceiver.getModelValue().addListener(this);

    updateInConnectedLabel();

    connectSender();

    // Size of plugin GUI
    setSize(250, 430);

}

NeuralPiAudioProcessorEditor::~NeuralPiAudioProcessorEditor()
{
}

//==============================================================================
void NeuralPiAudioProcessorEditor::paint (Graphics& g)
{
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
    ampGainKnob.setBounds(30, 72, 75, 95);
    ampMasterKnob.setBounds(140, 72, 75, 95);
    ampBassKnob.setBounds(30, 155, 75, 95);
    ampMidKnob.setBounds(140, 155, 75, 95);
    ampTrebleKnob.setBounds(30, 235, 75, 95);
    ampPresenceKnob.setBounds(140, 235, 75, 95);

    GainLabel.setBounds(28, 150, 80, 10);
    LevelLabel.setBounds(138, 150, 80, 10);
    BassLabel.setBounds(28, 233, 80, 10);
    MidLabel.setBounds(138, 233, 80, 10);
    TrebleLabel.setBounds(28, 313, 80, 10);
    PresenceLabel.setBounds(138, 313, 80, 10);

    addAndMakeVisible(ampNameLabel);
    ampNameField.setEditable(true, true, true);
    addAndMakeVisible(ampNameField);

    // IP controls:
    ipField.setBounds(150, 330, 100, 25);
    ipLabel.setBounds(15, 330, 150, 25);

    // Port controls:
    outPortNumberLabel.setBounds(15, 365, 150, 25);
    outPortNumberField.setBounds(160, 365, 75, 25);
    inPortNumberLabel.setBounds(15, 400, 150, 25);
    inPortNumberField.setBounds(160, 400, 75, 25);
}

void NeuralPiAudioProcessorEditor::modelSelectChanged()
{
    const int selectedFileIndex = modelSelect.getSelectedItemIndex();
    if (selectedFileIndex >= 0 && selectedFileIndex < processor.jsonFiles.size()) {
        processor.loadConfig(processor.jsonFiles[selectedFileIndex]);
        processor.current_model_index = modelSelect.getSelectedItemIndex();
    }
    auto newValue = static_cast<float>(processor.current_model_index / (processor.num_models - 1.0));
    modelKnob.setValue(newValue);
    //modelKnob.setValue(processor.current_model_index);
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
                    //processor.num_models += 1;
                }
                // Sort jsonFiles alphabetically
                std::sort(processor.jsonFiles.begin(), processor.jsonFiles.end());
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
    if (slider == &modelKnob)
        if (slider->getValue() >= 0 && slider->getValue() < processor.jsonFiles.size()) {
            modelSelect.setSelectedItemIndex(processor.getModelIndex(slider->getValue()), juce::NotificationType::dontSendNotification);
        }
}
/*
    else if (slider == &ampBassKnob || slider == &ampMidKnob || slider == &ampTrebleKnob) {
        processor.set_ampEQ(ampBassKnob.getValue(), ampMidKnob.getValue(), ampTrebleKnob.getValue(), ampPresenceKnob.getValue());
        // Set knob states for saving positions when closing/reopening GUI
        processor.ampBassKnobState = ampBassKnob.getValue();
        processor.ampMidKnobState = ampMidKnob.getValue();
        processor.ampTrebleKnobState = ampTrebleKnob.getValue();
    }
    else if (slider == &ampPresenceKnob) {
        processor.set_ampEQ(ampBassKnob.getValue(), ampMidKnob.getValue(), ampTrebleKnob.getValue(), ampPresenceKnob.getValue());
    }
}
*/


// OSC Messages
Slider& NeuralPiAudioProcessorEditor::getGainSlider()
{
    return ampGainKnob;
}

Slider& NeuralPiAudioProcessorEditor::getMasterSlider()
{
    return ampMasterKnob;
}

Slider& NeuralPiAudioProcessorEditor::getBassSlider()
{
    return ampBassKnob;
}

Slider& NeuralPiAudioProcessorEditor::getMidSlider()
{
    return ampMidKnob;
}

Slider& NeuralPiAudioProcessorEditor::getTrebleSlider()
{
    return ampTrebleKnob;
}

Slider& NeuralPiAudioProcessorEditor::getPresenceSlider()
{
    return ampPresenceKnob;
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
    bassAddressPattern = "/parameter/" + ampName + "/Bass";
    midAddressPattern = "/parameter/" + ampName + "/Mid";
    trebleAddressPattern = "/parameter/" + ampName + "/Treble";
    presenceAddressPattern = "/parameter/" + ampName + "/Presence";
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
    /*
    else if (labelThatHasChanged == getAmpNameField())
    {
        ampName = getAmpNameField().getTextValue().toString();
        buildAddressPatterns();
        oscReceiver.updateAmpName(getAmpNameField().getTextValue().toString());
    }
    */
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
    if (value.refersToSameSourceAs(oscReceiver.getBassValue()))
    {
        if (!approximatelyEqual(static_cast<double> (value.getValue()), getBassSlider().getValue()))
        {
            getBassSlider().setValue(static_cast<double> (value.getValue()),
                NotificationType::sendNotification);
        }
    }
    else if (value.refersToSameSourceAs(oscReceiver.getMidValue()))
    {
        if (!approximatelyEqual(static_cast<double> (value.getValue()), getMidSlider().getValue()))
        {
            getMidSlider().setValue(static_cast<double> (value.getValue()),
                NotificationType::sendNotification);
        }
    }
    if (value.refersToSameSourceAs(oscReceiver.getTrebleValue()))
    {
        if (!approximatelyEqual(static_cast<double> (value.getValue()), getTrebleSlider().getValue()))
        {
            getTrebleSlider().setValue(static_cast<double> (value.getValue()),
                NotificationType::sendNotification);
        }
    }
    else if (value.refersToSameSourceAs(oscReceiver.getPresenceValue()))
    {
        if (!approximatelyEqual(static_cast<double> (value.getValue()), getPresenceSlider().getValue()))
        {
            getPresenceSlider().setValue(static_cast<double> (value.getValue()),
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
    getBassSlider().setValue(getParameterValue(bassName), NotificationType::dontSendNotification);
    getMidSlider().setValue(getParameterValue(midName), NotificationType::dontSendNotification);
    getTrebleSlider().setValue(getParameterValue(trebleName), NotificationType::dontSendNotification);
    getPresenceSlider().setValue(getParameterValue(presenceName), NotificationType::dontSendNotification);
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