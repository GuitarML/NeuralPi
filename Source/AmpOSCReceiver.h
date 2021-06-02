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

        }
        else if (message.size() == 1 && message[0].isInt32())
        {
            DBG("    value " + String(message[0].getInt32()) + " to AP " + message.getAddressPattern().toString());

            if (message.getAddressPattern().matches(modelAddressPattern))
            {
                //modelValue.setValue(jlimit(0.0f, 1.0f, message[0].getFloat32()));
                modelValue.setValue(message[0].getInt32());
            }
        }
    }

    int defaultPort {25024};

    String ampName {"NeuralPi"};
    String gainAddressPattern {"/parameter/elk_juce_example/Gain"};
    String masterAddressPattern {"/parameter/elk_juce_example/Master"};
    String modelAddressPattern {"/parameter/elk_juce_example/Model"};

    Value gainValue {0.0f};
    Value masterValue {0.0f};

    Value modelValue {0.0f};

    bool connected = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpOSCReceiver)
};
