/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <stdio.h>
#include <fstream>
#include <iostream>


//==============================================================================
NeuralPiAudioProcessorEditor::NeuralPiAudioProcessorEditor (NeuralPiAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to

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

    // Size of plugin GUI
    setSize(250, 200);

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

    // Amp Widgets
    ampGainKnob.setBounds(30, 85, 75, 95);
    ampMasterKnob.setBounds(140, 85, 75, 95);
    GainLabel.setBounds(28, 163, 80, 10);
    LevelLabel.setBounds(138, 163, 80, 10);
}

void NeuralPiAudioProcessorEditor::modelSelectChanged()
{
    const int selectedFileIndex = modelSelect.getSelectedItemIndex();
    if (selectedFileIndex >= 0 && selectedFileIndex < processor.jsonFiles.size()) {
        processor.loadConfig(processor.jsonFiles[selectedFileIndex]);
        processor.current_model_index = modelSelect.getSelectedItemIndex();
    }
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
}