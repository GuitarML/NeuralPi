/*
  ==============================================================================

  CabSim

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"

#pragma once

//==============================================================================
class CabSim
{
public:
    //==============================================================================
    CabSim() = default;

    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
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

    void load(File irFile) noexcept
    {
        auto& convolution = processorChain.template get<convolutionIndex>();
        convolution.loadImpulseResponse(irFile,
            juce::dsp::Convolution::Stereo::yes,
            juce::dsp::Convolution::Trim::no,
            1024);
    }

private:
    enum
    {
        convolutionIndex
    };

    juce::dsp::ProcessorChain<juce::dsp::Convolution> processorChain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CabSim)
};