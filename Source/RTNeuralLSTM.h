#pragma once

#include <RTNeural/RTNeural.h>

class RT_LSTM
{
public:
    RT_LSTM() = default;

    void reset();
    void load_json(const char* filename);

    void process(const float* inData, float* outData, int numSamples);

private:
    RTNeural::ModelT<float, 1, 1,
        RTNeural::LSTMLayerT<float, 1, 20>,
        RTNeural::DenseT<float, 20, 1>> model;
};
