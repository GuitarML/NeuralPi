#include "RTNeuralGRU.h"

using Vec2d = std::vector<std::vector<float>>;

Vec2d transpose(const Vec2d& x)
{
    auto outer_size = x.size();
    auto inner_size = x[0].size();
    Vec2d y(inner_size, std::vector<float>(outer_size, 0.0f));

    for (size_t i = 0; i < outer_size; ++i)
    {
        for (size_t j = 0; j < inner_size; ++j)
            y[j][i] = x[i][j];
    }

    return y;
}

template <typename T1>
void RT_GRU::set_weights(T1 model, const char* filename)
{
    // Initialize the correct model
    auto& gru = (*model).template get<0>();
    auto& dense = (*model).template get<1>();

    // read a JSON file
    std::ifstream i2(filename);
    nlohmann::json weights_json;
    i2 >> weights_json;

    Vec2d gru_weights_ih = weights_json["/state_dict/rec.weight_ih_l0"_json_pointer];
    gru.setWVals(transpose(gru_weights_ih));

    Vec2d gru_weights_hh = weights_json["/state_dict/rec.weight_hh_l0"_json_pointer];
    gru.setUVals(transpose(gru_weights_hh));

    Vec2d gru_bias_hh;
    gru_bias_hh[0] = weights_json["/state_dict/rec.bias_ih_l0"_json_pointer];
    gru_bias_hh[1] = weights_json["/state_dict/rec.bias_hh_l0"_json_pointer];

    //std::vector<float> gru_bias_ih = weights_json["/state_dict/rec.bias_ih_l0"_json_pointer];
    //std::vector<float> gru_bias_hh = weights_json["/state_dict/rec.bias_hh_l0"_json_pointer];
    //for (int i = 0; i < 60; ++i)  // TODO verify 60 here for hidden size 20 (i.e. 3 * hidden_size)
    //    gru_bias_hh[i] += gru_bias_ih[i];
    gru.setBVals(gru_bias_hh);

    Vec2d dense_weights = weights_json["/state_dict/lin.weight"_json_pointer];
    dense.setWeights(dense_weights);

    std::vector<float> dense_bias = weights_json["/state_dict/lin.bias"_json_pointer];
    dense.setBias(dense_bias.data());
   
}
void RT_GRU::load_json(const char* filename)
{
    // Read in the JSON file
    std::ifstream i2(filename);
	nlohmann::json weights_json;
	i2 >> weights_json;

    // Get the input size of the JSON file
	int input_size_json = weights_json["/model_data/input_size"_json_pointer];
	input_size = input_size_json;

    // Load the appropriate model
    if (input_size == 1) {
		set_weights(&model, filename);
    }
    else if (input_size == 2) {
		set_weights(&model_cond1, filename);
    } 
    else if (input_size == 3) {
		set_weights(&model_cond2, filename);
    } 
}


void RT_GRU::reset()
{
    if (input_size == 1) {
        model.reset();
    } else {
        model_cond1.reset();
    }
}

void RT_GRU::process(const float* inData, float* outData, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
        outData[i] = model.forward(inData + i) + inData[i];
}

void RT_GRU::process(const float* inData, float param, float* outData, int numSamples)
{
    for (int i = 0; i < numSamples; ++i) {
        inArray1[0] = inData[i];
        inArray1[1] = param;
        outData[i] = model_cond1.forward(inArray1) + inData[i];
    }
}

void RT_GRU::process(const float* inData, float param1, float param2, float* outData, int numSamples)
{
    for (int i = 0; i < numSamples; ++i) {
        inArray2[0] = inData[i];
        inArray2[1] = param1;
        inArray2[2] = param2;
        outData[i] = model_cond2.forward(inArray2) + inData[i];
    }
}

