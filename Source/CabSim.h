/*
  ==============================================================================

  CabSim

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"

#pragma once


//==============================================================================
//template <typename Type>
class CabSim
{
public:
    //==============================================================================
    CabSim()
    {
        auto dir = juce::File::getCurrentWorkingDirectory();
        //auto dir = juce::File("C:\\Users\\rache\\Desktop\\dev\\NeuralPi\\resources");
        int numTries = 0;

        while (! dir.getChildFile ("resources").exists() && numTries++ < 15)
            dir = dir.getParentDirectory();

        auto& convolution = processorChain.template get<convolutionIndex>();
        //convolution.loadImpulseResponse(dir.getChildFile("cassette_recorder.wav"),
        convolution.loadImpulseResponse(dir.getChildFile("resources").getChildFile("guitar_amp.wav"),
            juce::dsp::Convolution::Stereo::yes,
            juce::dsp::Convolution::Trim::no,
            1024);
    }

    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        //juce::ignoreUnused (spec);
        processorChain.prepare(spec);
    }

    //==============================================================================
    template <typename ProcessContext>
    void process(const ProcessContext& context) noexcept
    {
        processorChain.process(context);
    }

    //==============================================================================
    void reset() noexcept 
    {
        processorChain.reset();
    }

private:
    //==============================================================================
    enum
    {
        convolutionIndex
    };

    juce::dsp::ProcessorChain<juce::dsp::Convolution> processorChain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CabSim)
};