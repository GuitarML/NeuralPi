/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "AmpOSCReceiver.h"
//#include "myLookAndFeel.h"
#include <stdlib.h>

//==============================================================================
/**
*/
class NeuralPiAudioProcessorEditor  : public AudioProcessorEditor,
                                      private Button::Listener,
                                      private Slider::Listener,
                                      private Value::Listener,
                                      private Label::Listener,
                                      private Timer
                                
{
public:
    NeuralPiAudioProcessorEditor (NeuralPiAudioProcessor&);
    ~NeuralPiAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    AmpOSCReceiver oscReceiver;
    OSCSender oscSender;

    String outgoingIP{ "127.0.0.1" };
    int outgoingPort{ 24024 };
    int incomingPort{ 25024 };

    String ampName{ "NeuralPi" };
    String gainAddressPattern{ "/parameter/NeuralPi/Gain" };
    String masterAddressPattern{ "/parameter/NeuralPi/Master" };
    String modelAddressPattern{ "/parameter/NeuralPi/Model" };
    String irAddressPattern{ "/parameter/NeuralPi/Ir" };
    String bassAddressPattern{ "/parameter/NeuralPi/Bass" };
    String midAddressPattern{ "/parameter/NeuralPi/Mid" };
    String trebleAddressPattern{ "/parameter/NeuralPi/Treble" };
    String presenceAddressPattern{ "/parameter/NeuralPi/Presence" };
    String delayAddressPattern{ "/parameter/NeuralPi/Delay" };
    String reverbAddressPattern{ "/parameter/NeuralPi/Reverb" };

    const String gainName{ "gain" };
    const String masterName{ "master" };
    const String bassName{ "bass" };
    const String midName{ "mid" };
    const String trebleName{ "treble" };
    const String presenceName{ "presence" };
    const String delayName{ "delay" };
    const String reverbName{ "reverb" };

    const String modelName{ "model" };
    const String irName{ "ir" };

    // For the FileChooser to load json models and IR files
    std::unique_ptr<FileChooser> myChooser;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NeuralPiAudioProcessor& processor;

    Image background = ImageCache::getFromMemory(BinaryData::npi_background_jpg, BinaryData::npi_background_jpgSize);

    // Amp Widgets
    Slider ampGainKnob;
    Slider ampMasterKnob;
    Slider modelKnob;
    Slider irKnob;
    //ImageButton ampOnButton;
    //ImageButton ampLED;
    ComboBox modelSelect;
    ComboBox irSelect;
    Slider ampBassKnob;
    Slider ampMidKnob;
    Slider ampTrebleKnob;
    Slider ampPresenceKnob;
    Slider ampDelayKnob;
    Slider ampReverbKnob;

    Label GainLabel;
    Label LevelLabel;
    Label BassLabel;
    Label MidLabel;
    Label TrebleLabel;
    Label PresenceLabel;
    Label DelayLabel;
    Label ReverbLabel;
    Label toneDropDownLabel;
    Label irDropDownLabel;
    Label versionLabel;

    File test_file;
    File model_folder;

    TextButton loadButton;
    TextButton loadIR;
    ToggleButton irButton;
    ToggleButton lstmButton;

    juce::LookAndFeel_V4 blueLookAndFeel;
    juce::LookAndFeel_V4 redLookAndFeel;
    
    juce::String fname;
    virtual void buttonClicked(Button* button) override;
    void modelSelectChanged();
    void loadButtonClicked();
    void updateToggleState(juce::Button* button, juce::String name);
    void irSelectChanged();
    void loadIRClicked();
    virtual void sliderValueChanged(Slider* slider) override;


    Label ampNameLabel{ {}, "Amp Name (no spaces): " };
    Label ampNameField{ {}, "NeuralPi" };

    Label ipLabel{ {}, "Target IP Address: " };
    Label ipField{ {}, "192.168.1.97" };

    Label outPortNumberLabel{ {}, "Outgoing OSC Port: " };
    Label outPortNumberField{ {}, "24024" };

    Label inPortNumberLabel{ {}, "Incoming OSC Port: " };
    Label inPortNumberField{ {}, "25024" };

    Label gainLabel{ {}, "Gain" };
    Label masterLabel{ {}, "Master" };

    Label modelLabel{ {}, "Model" };

    Label inConnectedLabel{ "(connected)" };
    Label outConnectedLabel{ "(connected)" };

    // OSC Messages
    Slider& getGainSlider();
    Slider& getMasterSlider();
    Slider& getModelSlider();
    Slider& getIrSlider();
    Slider& getBassSlider();
    Slider& getMidSlider();
    Slider& getTrebleSlider();
    Slider& getPresenceSlider();
    Slider& getDelaySlider();
    Slider& getReverbSlider();

    Label& getOutPortNumberField();
    Label& getInPortNumberField();
    Label& getIPField();
    Label& getAmpNameField();
    Label& getOutConnectedLabel();
    Label& getInConnectedLabel();
    void buildAddressPatterns();
    void connectSender();
    void updateOutgoingIP(String ip);
    void updateOutgoingPort(int port);
    void labelTextChanged(Label* labelThatHasChanged) override;
    void updateInConnectedLabel();
    void updateOutConnectedLabel(bool connected);
    // This callback is invoked if an OSC message has been received setting either value.
    void valueChanged(Value& value) override;
    void timerCallback() override;

    AudioProcessorParameter* getParameter(const String& paramId);

    float getParameterValue(const String& paramId);
    void setParameterValue(const String& paramId, float value);
    void setParamKnobColor();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralPiAudioProcessorEditor)
};
