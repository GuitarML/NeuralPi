#pragma once

#include <RTNeural/RTNeural.h>

class RT_LSTM
{
public:
    RT_LSTM() = default;

    void reset();
    void load_json(const char* filename);
    void load_json2(const char* filename);

    void process(const float* inData, float* outData, int numSamples);
    void process(const float* inData, float param, float* outData, int numSamples);

    int input_size = 1;
    
private:
    RTNeural::ModelT<float, 1, 1,
        RTNeural::LSTMLayerT<float, 1, 20>,
        RTNeural::DenseT<float, 20, 1>> model;

    RTNeural::ModelT<float, 2, 1,
        RTNeural::LSTMLayerT<float, 2, 20>,
        RTNeural::DenseT<float, 20, 1>> model_cond1;

    float inArray[2] = { 0.0, 0.0 };
};
