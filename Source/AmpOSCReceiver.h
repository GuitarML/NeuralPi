#include "../JuceLibraryCode/JuceHeader.h"

#pragma once

class AmpOSCReceiver :
        private OSCReceiver,
        private OSCReceiver::Listener <OSCReceiver::MessageLoopCallback>
{
public:
    AmpOSCReceiver()
    {
        changePort (defaultPort);

        addListener (this);
    }

    Value& getGainValue()
    {
        return gainValue;
    }

    Value& getMasterValue()
    {
        return masterValue;
    }

    Value& getBassValue()
    {
        return bassValue;
    }

    Value& getMidValue()
    {
        return midValue;
    }

    Value& getTrebleValue()
    {
        return trebleValue;
    }

    Value& getPresenceValue()
    {
        return presenceValue;
    }

    Value& getModelValue()
    {
        return modelValue;
    }

    void changePort (int port)
    {
        if (! connect (port))
        {
            connected = false;
            DBG ("Connection Failed");
        }
        else
        {
            connected = true;
            DBG("Connection Succeeded");
        }
    }

    void updateAmpName (String name)
    {
        ampName = name;
        buildAddressPatterns();
    }

    bool isConnected()
    {
        return connected;
    }

private:
    void buildAddressPatterns()
    {
        gainAddressPattern = "/parameter/" + ampName + "/Gain";
        masterAddressPattern = "/parameter/" + ampName + "/Master";
        bassAddressPattern = "/parameter/" + ampName + "/Bass";
        midAddressPattern = "/parameter/" + ampName + "/Mid";
        trebleAddressPattern = "/parameter/" + ampName + "/Treble";
        presenceAddressPattern = "/parameter/" + ampName + "/Presence";
        modelAddressPattern = "/parameter/" + ampName + "/Model";
    }

    void oscMessageReceived(const OSCMessage& message) override
    {
        DBG("Message Received: ");

        if (message.size() == 1 && message[0].isFloat32())
        {
            DBG("    value " + String(message[0].getFloat32()) + " to AP " + message.getAddressPattern().toString());

            if (message.getAddressPattern().matches(gainAddressPattern))
            {
                gainValue.setValue(jlimit(0.0f, 1.0f, message[0].getFloat32()));
            }
            else if (message.getAddressPattern().matches(masterAddressPattern))
            {
                masterValue.setValue(jlimit(0.0f, 1.0f, message[0].getFloat32()));
            }

            if (message.getAddressPattern().matches(bassAddressPattern))
            {
                bassValue.setValue(jlimit(0.0f, 1.0f, message[0].getFloat32()));
            }
            else if (message.getAddressPattern().matches(midAddressPattern))
            {
                midValue.setValue(jlimit(0.0f, 1.0f, message[0].getFloat32()));
            }

            if (message.getAddressPattern().matches(trebleAddressPattern))
            {
                trebleValue.setValue(jlimit(0.0f, 1.0f, message[0].getFloat32()));
            }
            else if (message.getAddressPattern().matches(presenceAddressPattern))
            {
                presenceValue.setValue(jlimit(0.0f, 1.0f, message[0].getFloat32()));
            }
            else if (message.getAddressPattern().matches(modelAddressPattern))
            {
                modelValue.setValue(jlimit(0.0f, 1.0f, message[0].getFloat32()));
            }

        }
    }

    int defaultPort {25024};

    String ampName {"NeuralPi"};
    String gainAddressPattern {"/parameter/elk_juce_example/Gain"};
    String masterAddressPattern {"/parameter/elk_juce_example/Master"};
    String bassAddressPattern {"/parameter/elk_juce_example/Bass"};
    String midAddressPattern {"/parameter/elk_juce_example/Mid"};
    String trebleAddressPattern {"/parameter/elk_juce_example/Treble"};
    String presenceAddressPattern {"/parameter/elk_juce_example/Presence"};
    String modelAddressPattern {"/parameter/elk_juce_example/Model"};

    Value gainValue {0.5f};
    Value masterValue {0.5f};
    Value bassValue {0.5f};
    Value midValue {0.5f};
    Value trebleValue {0.5f};
    Value presenceValue {0.5f};

    Value modelValue {0.0f};

    bool connected = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpOSCReceiver)
};
