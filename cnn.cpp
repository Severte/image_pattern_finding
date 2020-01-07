#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define IMG_WIDTH 60
#define IMG_HEIGHT 60
#define FILTER_SIZE 9
#define FILTER_COUNT 12;


float maxValue(float** filtersArray, int x, int y){
    float result = 0.0;
    for(int i = 0; i < FILTER_COUNT; i++){
        result = max(result, filtersArray[i][x * FILTER_SIZE + y]);
    }
    return result;
}


int dotProduct(float* img, float* filter, int x, int y, int c){
    float result = 0.0;
    for(int i = 0; i < FILTER_SIZE; i++){
        for(int j = 0; j < FILTER_SIZE; j++){
            result += img[(x+i) * IMG_WIDTH + (y+j) + c] * filter[i * FILTER_SIZE + (y+j)];
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


void filtersPooling(float** filtersArray, float* result){
    for(int i = 0; i <= IMG_WIDTH - FILTER_SIZE; i++){
        for(int j = 0; j <= IMG_HEIGHT - FILTER_SIZE; j++){
            result[i * FILTER_SIZE + j] = maxValue(filtersArray, i, j);
        }
    }
}

void pooling(float* initial, float* result){
    int new_size = (IMG_WIDTH - FILTER_SIZE + 1 ) / 2
    for(int i = 0; i < new_size; i++ ){
        for(int j = 0; j < new_size; j++){
            result[i * new_size + j] = (result[i * 2 * FILTER_SIZE + j * 2] + result[(i * 2 + 1)* FILTER_SIZE + j * 2] 
                                        + result[i * 2 * FILTER_SIZE + j * 2 + 1] + result[(i * 2 + 1)* FILTER_SIZE + j * 2]) / 4;
        }
    }
}

void hiddenLayer(float* input, float* weights, float* output){
    int result = 0.0;
    int new_size = (IMG_WIDTH - FILTER_SIZE + 1 ) / 2
    int new_size_sq = new_size * new_size;
    for(int i = 0; i < new_size; i++){
        for(int j = 0; i < new_size; j++){
            for(int k = 0; l < new_size; k++){
                for(int l = 0; l < new_size; l++){
                    output[i * new_size + j] = input[k * new_size + l] * weights[(i * new_size + j) * new_size_sq + (k * new_size + l)];
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
    return result;
}

void loadFilters(float** filters){
    ifstream inFile;
    for(int i = 0; i < FILTER_COUNT; i++){
        inFile.open("filter" + to_string(i+1));

    }
}

void main(){
}