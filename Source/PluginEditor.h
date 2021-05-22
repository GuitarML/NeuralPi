/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
//#include "myLookAndFeel.h"
#include <stdlib.h>

//==============================================================================
/**
*/
class NeuralPiAudioProcessorEditor  : public AudioProcessorEditor,
                                       private Button::Listener,
                                       private Slider::Listener
                                
{
public:
    NeuralPiAudioProcessorEditor (NeuralPiAudioProcessor&);
    ~NeuralPiAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NeuralPiAudioProcessor& processor;

    // Amp Widgets
    Slider ampGainKnob;
    Slider ampMasterKnob;
    //ImageButton ampOnButton;
    //ImageButton ampLED;
    ComboBox modelSelect;
    Label GainLabel;
    Label LevelLabel;

    File test_file;
    File model_folder;

    TextButton loadButton;
    juce::String fname;
    virtual void buttonClicked(Button* button) override;
    void modelSelectChanged();
    void loadButtonClicked();
    virtual void sliderValueChanged(Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralPiAudioProcessorEditor)
};
