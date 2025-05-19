#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <complex>
#include "modQAM.cpp"
#include "demodQAM.cpp"
#include "gaussenoise.cpp"

using namespace std;


vector<int> gen_binary(int num_bits) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1);
    vector<int> binary;
    for (int i = 0; i < num_bits; i++) {
        binary.push_back(dis(gen));
		//cout << binary[i] << " ";
    }
    cout << endl;
    return binary;
}


double BER(const vector<int>& send, const vector<int>& received) {
    if (send.size() != received.size()) {
        cout << "Error: send.size() != received.size()" << endl;
    }
    int errors = 0;
    for (int i = 0; i < send.size(); i++) {
        if (send[i] != received[i]) {
            errors++;
        }
    }
    return static_cast<double>(errors) / send.size();
}

void test_modulation(ModulationType mod_type) {

    int bits_per_symbol;
    string mod_name;

    cout << "\n" << mod_name << endl;
    switch (mod_type) {
    case QPSK: 
        bits_per_symbol = 2;
        cout<< "------------  QPSK ----------";  
        break;

    case QAM16: 
        bits_per_symbol = 4;
        cout << "---------- 16QAM ----------"; 
        break;

    case QAM64:
        bits_per_symbol = 6; 
        cout << "----------- 64QAM ----------"; 
        break;

    default:
        cout << "ERROR: Unknown modulation type\n";
        return;
    }

    int num_symbols = 100;
    int total_bits = num_symbols * bits_per_symbol;
    vector<int> binary = gen_binary(total_bits);

    modQAM modulation;
    modulation.modulateIQ(binary, mod_type);

    gaussnoise noise;

    demodQAM demodulation;

    vector<double> sigma = { 1, 2, 5, 10,15, 20, 25 };
    for (double sigm: sigma) {

        vector<complex<double>> noise_signal = modulation.signal;

        noise.gnoise(noise_signal, 0.0, sigm);

        demodulation.signal = noise_signal;
        demodulation.demod(static_cast<demodQAM::ModulationType>(mod_type));

        double ber = BER(binary, demodulation.binary);
        cout << "sigma = " << sigm << "       BER = " <<  ber << endl;
    }
}


int main() {
    test_modulation(QPSK);
    test_modulation(QAM16);
    test_modulation(QAM64);
    return 0;
}
