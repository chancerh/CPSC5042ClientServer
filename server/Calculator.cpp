#include "Calculator.h"
#include <algorithm> // for sort function
#include "math.h"
#include <stdexcept>

using namespace std;

string Calculator::calculateExpression(const string &inExpr)
{
    return "";
}

float Calculator::mean(vector<float> vec) 
{
    // Check that vector is not empty
    if (vec.size() < 1)
        throw invalid_argument("Dataset in vec must have size > 0.");
  
    float elementSum = 0.0; // this will store the sum of elements

    // Sum the elements of the vector
    for (float elm : vec) 
        elementSum += elm;

    // calculate the mean and return
    float mean = elementSum / ((float) vec.size());

    return mean;

}

float Calculator::median(vector<float> vec)
{
    // Check that vector is not empty
    if (vec.size() < 1)
        throw invalid_argument("Dataset in vec must have size > 0.");   

    // Then median is the .5 quantile
    return quantiles(vec, .5)[0];
}


vector<float> Calculator::quantiles(vector<float> vec, float quantCut)
{
  float initQuantCut = quantCut;
  
    // Check that vector is not empty
  if (vec.size() < 1)
        throw invalid_argument("Dataset in vec must have size > 0.");

    vector<float> quants; // this will hold the calculated quantiles

    // Sort the vector ascending
    sort(vec.begin(), vec.end());
    int n = vec.size();

    while (quantCut < 1.0) {
        if (quantCut > 1.0 | quantCut < 0.0)
            throw invalid_argument("Quantile cuts must be between 0.0 and 1.0");

        // Get the index of the quantile. There may be two if n is even, so 
        float index = (n - 1) * quantCut;

        // If n is odd, indexLow and indexHigh will be the same
        int indexLow = floor(index);
        int indexHigh = ceil(index);

        // Either way, add the elements of indexLow and indexHigh together and
        // divide by 2.
        float quant = (vec[indexHigh] + vec[indexLow]) / 2.0;
        quants.push_back(quant);

        // Move to the next quantile
        quantCut += initQuantCut;
    }

    return quants;
}

float Calculator::var(const vector<float> &vec)
{

    // Check that vector is not empty
    if (vec.size() < 2)
        throw invalid_argument("Dataset in vec must have size > 1.");

    // Meed the mean the calculation
    float vecMean = mean(vec);
    // This will hold the accumulated numerator calculations
    float variance = 0.0;
    // Use a for loop for the sums
    for (auto elm : vec) 
    {
        variance += pow((elm - vecMean), 2);
    }

    // returned the unbiased variance
    return variance / (vec.size() - 1);
}

float Calculator::sd(vector<float> vec)
{
    // Check that vector is not empty
    if (vec.size() < 1)
        throw invalid_argument("Dataset in vec must have size > 1.");

    // unbiased sd is just the square root of the unbiased variance
    return sqrt(var(vec));
}

vector<float> Calculator::summary(const vector<float> &vec)
{
    // Check that vector is not empty
    if (vec.size() < 1)
        throw invalid_argument("Dataset in vec must have size > 0.");
    
    // This will hold the results of the calculations and be returned
    vector<float> summaryOut;
    
    auto min = *min_element(vec.begin(), vec.end());
    auto max = *max_element(vec.begin(), vec.end());
    auto quartiles = quantiles(vec, .25);
    auto avg = mean(vec);
     
    summaryOut.push_back(min);
    summaryOut.push_back(quartiles[0]);
    summaryOut.push_back(quartiles[1]);
    summaryOut.push_back(avg);
    summaryOut.push_back(quartiles[2]);
    summaryOut.push_back(max);


    return summaryOut;

}
