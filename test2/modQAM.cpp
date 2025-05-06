#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <complex>

using namespace std;
enum ModulationType { QPSK, QAM16, QAM64 };
class modQAM {
private:
    float fs = 1000;
    float fcar = 100;
    double I, Q;
    int symbol;
    int symbolIndex;

public:
    int numSamples = 10;
    vector<complex<double>> signal;


    void modulatesymbol(vector<int>& bits, ModulationType modTYPE) {
        I = 0;
        Q = 0;

        if (modTYPE == QPSK && bits.size() == 2) {
            I = (bits[0] == 0) ? -1 : 1;
            Q = (bits[1] == 0) ? -1 : 1;
        }

        else if (modTYPE == QAM16 && bits.size() == 4) {
            int I_bits = bits[0] * 2 + bits[1];
            int Q_bits = bits[2] * 2 + bits[3];
            int QAM16_value[4] = { -3, -1, 1, 3 };
            I = QAM16_value[I_bits];
            Q = QAM16_value[Q_bits];
        }

        else if (modTYPE == QAM64 && bits.size() == 6) {
            int I_bits = bits[0] * 4 + bits[1] * 2 + bits[2];
            int Q_bits = bits[3] * 4 + bits[4] * 2 + bits[5];
            int QAM64_value[8] = { -7, -5, -3, -1, 1, 3, 5, 7 };
            I = QAM64_value[I_bits];
            Q = QAM64_value[Q_bits];
           // cout << "I:" << I << " Q: " << Q << endl;
          //  cout << bits[0] << " "<< bits[1] << "  " << bits[2]<<endl;
        }
        else {
            cout << "ERROR: invalid symbols or modTYPE" << endl;
            return;
        }
    }

    void genAnalog() {
        for (int i = 0; i < numSamples; i++) {
            double t = static_cast<double>(symbolIndex * numSamples + i) / fs;

            double COS = cos(2 * M_PI * fcar * t);
            double SIN = sin(2 * M_PI * fcar * t);
            double I_part = I * COS;
            double Q_part = Q * SIN;

            complex<double> sample(I_part, Q_part);
            signal.push_back(sample);


        }
        symbolIndex++;
    }

    void modulateIQ(vector<int>& bits, ModulationType modTYPE) {
        symbolIndex = 0;
        signal.clear();

        if (modTYPE == QPSK) {
            symbol = 2;
        }
        else if (modTYPE == QAM16) {
            symbol = 4;
        }
        else if (modTYPE == QAM64) {
            symbol = 6;
        }
        else {
            cout << "ERROR modTYPE" << endl;
            return;
        }


        for (size_t i = 0; i + symbol <= bits.size(); i += symbol) {
            vector<int> symbols(bits.begin() + i, bits.begin() + i + symbol);
            modulatesymbol(symbols, modTYPE);
            genAnalog();

        }
    }
};
