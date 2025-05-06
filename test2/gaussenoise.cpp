#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <complex>

using namespace std;

class gaussnoise {

private:

    default_random_engine generator;

public:

    gaussnoise() {
        random_device rd;
        generator.seed(rd());
    }
    void gnoise(vector<complex<double>>& data, double mean, double sigm) {
        if (sigm <= 0) return ;
        normal_distribution<double> dist(mean, sigm);
        for (size_t i = 0; i < data.size(); ++i) {
            data[i].real(data[i].real() + dist(generator));
            data[i].imag(data[i].imag() + dist(generator));
        }
    }

};