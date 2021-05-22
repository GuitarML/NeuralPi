#include <iostream>
#include <nlohmann/json.hpp>
#include "NumCpp.hpp"
#include <string>

class ModelLoader
{

public:
	ModelLoader();
	nc::NdArray<float> vector_to_nc(std::vector<float> in_vec);
	nc::NdArray<float> matrix_to_nc(std::vector<std::vector<float>> in_mat);

	void load_json(const char *filename);

    int hidden_size = 32;

    nc::NdArray<float> lstm_bias_ih_nc;
    nc::NdArray<float> lstm_weights_ih_nc;

    nc::NdArray<float> lstm_bias_hh_nc;
    nc::NdArray<float> lstm_weights_hh_nc;

    nc::NdArray<float> lstm_bias_nc;

    nc::NdArray<float> dense_bias_nc;
    nc::NdArray<float> dense_weights_nc;

private:

};