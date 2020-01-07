#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cmath>
using namespace std;

#define IMG_WIDTH 60
#define IMG_HEIGHT 60
#define FILTER_SIZE 7
#define FILTER_SIZE_SQ 49
#define FEATURE_MAP_SIZE 54
#define FEATURE_MAP_SIZE_SQ 2916
#define NEW_SIZE 27
#define NEW_SIZE_SQ 729
#define FILTER_COUNT 48 // must be divisible by 3 to account for 3 RGB levels


float maxValue(float** filtersArray, int x, int y){
    float result = 0.0;
    for(int i = 0; i < FILTER_COUNT; i++){
        result = max(result, filtersArray[i][x * FILTER_SIZE + y]);
    }
    return result;
}

float sigmoid(float x){
    return (1 / ( 1 + exp(-x)));
}

int dotProduct(float* img, float* filter, int x, int y, int c){
    float result = 0.0;
    for(int i = 0; i < FILTER_SIZE; i++){
        for(int j = 0; j < FILTER_SIZE; j++){
            result += img[(x+i) * IMG_WIDTH + (y+j)*3 + c] * filter[i * FILTER_SIZE + (y+j)];
        }
    }
    return max(0.0, result);
}

void applyFilter(float* img, float* filter, int c, float* result){
    for(int i = 0; i <= IMG_WIDTH - FILTER_SIZE; i++){
        for(int j = 0; j <= IMG_HEIGHT - FILTER_SIZE; j++){
            result[i * FILTER_SIZE + j] = dotProduct(img, filter, i, j, c);
        }
    }
}


/*void filtersPooling(float** filtersArray, float* result){
    for(int i = 0; i <= IMG_WIDTH - FILTER_SIZE; i++){
        for(int j = 0; j <= IMG_HEIGHT - FILTER_SIZE; j++){
            result[i * FILTER_SIZE + j] = maxValue(filtersArray, i, j);
        }
    }
}*/

void pooling(float* initial, float* result){
    int new_size = (IMG_WIDTH - FILTER_SIZE + 1 ) / 2
    for(int i = 0; i < new_size; i++ ){
        for(int j = 0; j < new_size; j++){
            result[i * new_size + j] = (result[i * 2 * FILTER_SIZE + j * 2] + result[(i * 2 + 1)* FILTER_SIZE + j * 2] 
                                        + result[i * 2 * FILTER_SIZE + j * 2 + 1] + result[(i * 2 + 1)* FILTER_SIZE + j * 2]) / 4;
        }
    }
}

void hiddenLayer(float** input, float** weights, float* output){
    int result = 0.0;
    int new_size = (IMG_WIDTH - FILTER_SIZE + 1 ) / 2
    int new_size_sq = new_size * new_size;
    for(int i = 0; i < new_size; i++){
        for(int j = 0; i < new_size; j++){
            for(int k = 0; l < new_size; k++){
                for(int l = 0; l < new_size; l++){
                    for(int m = 0; m < FILTER_COUNT/3; m++){
                        output[i * new_size + j] += input[m][k * new_size + l] * weights[m][(i * new_size + j) * new_size_sq + (k * new_size + l)];
                    }
                }
            }
        }
    }
    for(int i = 0; i < new_size; i++){
        for(int j = 0; i < new_size; j++){
            for(int k = 0; l < new_size; k++){
                for(int l = 0; l < new_size; l++){
                    output[i * new_size + j] = sigmoid(output[i * new_size + j]);
                }
            }
        }
    }
}

float outputLayer(float* input, float* weights){
    int result = 0.0;
    int new_size = (IMG_WIDTH - FILTER_SIZE + 1 ) / 2
    for(int i = 0; i < new_size; i++){
        for(int j = 0; j < new_size; j++){
            result += input[i * new_size + j] * weights [i * new_size + j];
        }
    }
    return sigmoid(result);
}

void loadFilters(float** filters){
    ifstream inFile;
    for(int i = 0; i < FILTER_COUNT; i++){
        inFile.open("filter" + to_string(i+1) + ".txt");
        int j = 0;
        float v = 0.0;
        while( inFile >> v){
            filters[i][j] = v;
            j++;
        }
        inFile.close();
    }
}

void loadWeights(float* weights, int n){
    ifstream inFile;
    
    inFile.open("weights" + to_string(n+1) + ".txt");
    int j = 0;
    float v = 0.0;
    while( inFile >> v ){
        weights[j] = v;
        j++;
    }
    inFile.close();
}

bool fileExists(string filename){
    ifstream inFile(filename);
    return inFile;
}

float random(float min, float max){
    srand(time(0));
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}

void saveWeights(float* weights, string filename){
    ofstream outFile(filename);
    for(int i = 0; i < NEW_SIZE; i++){
        for(int j = 0; j < NEW_SIZE; j++){
            outFile << weights[i * NEW_SIZE + j] << " ";
        }
        outFile << "\n";
    }
    outFile.close();
}

void saveFilters(float** filters){
    for(int i = 0; i < FILTER_COUNT; i++){
        ofstream outFile("filter" + to_string(i+1) + ".txt");
            for (int j = 0; j < FILTER_SIZE_SQ; j++){
                outFile << filters[i][j] << " ";
            }
            outFile.close();
        }
}

void initializeValues(float** filters, float** weights_hidden, float* weights_output){
    for(int i = 0; i < FILTER_COUNT; i++){
            for (int j = 0; j < FILTER_SIZE_SQ; j++){
                filters[i][j] = random(-1.0,1.0);
            }
        }
    for (int k = 0; k < NEW_SIZE_SQ; k++){
        for (int l = 0; l < FILTER_COUNT/3; l++){
            weights_hidden_layer[l][k] = random(-1.0,1.0);
            }
         weights_output_layer[k] = random(-1.0,1.0);
        }
}

float performCNN(float* img, float** filters, float* weights_h, float* weights_o){
    float feature_maps[FILTER_COUNT][FEATURE_MAP_SIZE_SQ];
    for(int i = 0; i < FILTER_COUNT; i++){
        applyFilter(img, filters[i], i%3, feature_maps[i]);
    }
    float feature_maps_sum[FILTER_COUNT/3][FEATURE_MAP_SIZE_SQ];
    for(int i = 0; i < FILTER_COUNT/3; i++){
        for(j = 0; j < FEATURE_MAP_SIZE_SQ; j++){
            feature_maps_sum[i][j] = feature_maps[3*i][FEATURE_MAP_SIZE_SQ] + feature_maps[3*i+1][FEATURE_MAP_SIZE_SQ] + feature_maps[3*i+2][FEATURE_MAP_SIZE_SQ];
        }
        
    }
    /*float filter_pooled[FEATURE_MAP_SIZE_SQ];
    filtersPooling(feature_maps, filter_pooled);*/
    float final_pooled[FILTER_COUNT/3][NEW_SIZE_SQ];
    for(int i = 0; i < FILTER_COUNT/3; i++){
        pooling(feature_maps_sum[i],final_pooled[i]);
    }
    float hidden_layer_output[NEW_SIZE_SQ];
    hiddenLayer(final_pooled, weights_h, hidden_layer_output);
    return outputLayer(hidden_layer_output, weights_o);
}

void main(){
    float filters[FILTER_COUNT][FILTER_SIZE_SQ];
    float weights_hidden_layer[FILTER_COUNT/3][NEW_SIZE_SQ];
    float weights_output_layer[NEW_SIZE_SQ];
    if(!fileExists("filter1.txt")){
        initializeValues(filters, weights_hidden_layer, weights_output_layer);
    }
    else{
        loadFilters(filters);
        loadWeights(weights_hidden_layer, 1);
        loadWeights(weights_output_layer, 2);
    }

}