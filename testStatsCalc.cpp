#include "server/Calculator.cpp"
#include <iostream>
#include <vector>
#include "assert.h"
#include <stdexcept>
#include <cmath>

using namespace std;

// Test compare floats - because precision could be an 
// use an epsilon to set precision boundary for passing
const float EPSILON = 0.001;


int main() {

    // make a vector or test data
    vector<float> testVecEven = 
        {5.1, 4.9, 4.7, 4.6, 5.0, 5.4};

    vector<float> testVecOdd = 
        {3.5, 3.0, 3.2, 3.1, 3.6, 3.9, 3.4};
    
    // make a 1 element vector
    vector<float> oneElmVec = {5.6};

    // make an empty vector
    vector<float> noElmVec;

    // Instantiate a new calculator
    Calculator* calc = new Calculator();

    // run quantiles
    calc->quantiles(testVecEven, .25);

    // Test mean
    // Test with an even vector
    cout << calc->mean(testVecEven) << endl;
    assert(fabs((calc->mean(testVecEven) - 4.95)) <= EPSILON);

    // Test with an odd vector
    assert(fabs((calc->mean(testVecOdd) - 3.385714)) <= EPSILON);

    assert(calc->mean(oneElmVec) == oneElmVec[0]);
    try 
    {
        calc->mean(noElmVec);
    }
    catch (invalid_argument)
    {
        cout << "Mean correctly threw invalid argument with empty vector" << 
            endl;
    }

    // Test Median
    // Test with an even vector
    cout << calc->median(testVecEven) << endl;
    assert(fabs((calc->median(testVecEven) - 4.95)) <= EPSILON);

    // Test with an odd vector
    assert(fabs((calc->median(testVecOdd) - 3.4)) <= EPSILON);

    // Test with N of 1
    assert(calc->median(oneElmVec) == oneElmVec[0]);

    // Test with empty vector
    try 
    {
        calc->median(noElmVec);
    }
    catch (invalid_argument)
    {
        cout << "Median correctly threw invalid argument with empty vector" << 
            endl;
    }

    // Test Variance
    // Test with an even vector
    cout << calc->var(testVecEven) << endl;
    assert(fabs((calc->var(testVecEven) - 0.083)) <= EPSILON);

    // Test with an odd vector
    assert(fabs((calc->var(testVecOdd) - 0.09809524)) <= EPSILON);

    // Test with N of 1
    try 
    {
      calc->var(oneElmVec);
    }
    catch (invalid_argument)
    {
        cout << "Variance correctly threw invalid argument with n=1 vector" << 
            endl;
    }

    // Test with empty vector
    try 
    {
        calc->var(noElmVec);
    }
    catch (invalid_argument)
    {
        cout << "Variance correctly threw invalid argument with empty vector" << 
            endl;
    }


    // Test SD
    // Test with an even vector
    cout << calc->sd(testVecEven) << endl;
    assert(fabs((calc->sd(testVecEven) - 0.2880972)) <= EPSILON);

    // Test with an odd vector
    assert(fabs((calc->sd(testVecOdd) - 0.3132016)) <= EPSILON);

    // Test with N of 1
    try 
    {
      calc->sd(oneElmVec);
    }
    catch (invalid_argument)
    {
        cout << "Standard deviation correctly threw invalid argument with n=1 vector" << 
            endl;
    }

    // Test with empty vector
    try 
    {
        calc->sd(noElmVec);
    }
    catch (invalid_argument)
    {
        cout << "Standard deviation correctly threw invalid argument with empty vector" << 
            endl;
    }

    // Test percentile
    assert(fabs((calc->percentile(testVecEven, 0.33) - 4.83)) <= EPSILON);
    assert(fabs((calc->percentile(testVecEven, 0.48) - 4.94)) <= EPSILON);
    assert(fabs((calc->percentile(testVecEven, 0.89) - 5.235)) <= EPSILON);

    assert(fabs((calc->percentile(testVecOdd, 0.33) - 3.198)) <= EPSILON);
    assert(fabs((calc->percentile(testVecOdd, 0.48) - 3.376)) <= EPSILON);
    assert(fabs((calc->percentile(testVecOdd, 0.89) - 3.702)) <= EPSILON);

    // Test with empty vector
    try 
    {
        calc->percentile(noElmVec, .32);
    }
    catch (invalid_argument)
    {
        cout << "Percentile correctly threw invalid argument with empty vector" << 
            endl;
    }
    
    
    
    // Test Summary
    vector<float> evenVecSummary = calc->summary(testVecEven);
    vector<float> evenVecCheck = {4.600, 4.750, 4.950, 4.950, 5.075, 5.400};
    for (int i = 0; i < evenVecSummary.size(); i++)
    {
        assert(fabs(evenVecSummary[i] - evenVecCheck[i]) <= EPSILON);
    }
        
    vector<float> oddVecSummary = calc->summary(testVecOdd);
    vector<float> oddVecCheck = {3.0, 3.15, 3.4, 3.386, 3.55, 3.9};
    for (unsigned int i = 0; i < oddVecSummary.size(); i++)
    {
	assert(fabs(oddVecSummary[i] - oddVecCheck[i]) <= EPSILON);
    }


}
