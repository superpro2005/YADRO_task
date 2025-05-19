#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <complex>

using namespace std;

class demodQAM {
private:
    float fcar = 100;
    float fs = 1000;
public:
    enum ModulationType { QPSK, QAM16, QAM64 };
    vector<complex<double>> signal;
    vector<int> binary;


    void demod(ModulationType modTYPE) {
        binary.clear();

        int numSamples = 1000;

        int total_symbols = signal.size() / numSamples;

        for (int symbols = 0; symbols < total_symbols; symbols++) {
            double sum_I = 0;
            double sum_Q = 0;
            double avg_I, avg_Q;
         
            for (int i = 0; i < numSamples; i++) {
                int idx = symbols * numSamples + i;
                double t = static_cast<double>(idx) / fs;

                double COS = cos(2 * M_PI * fcar * t);
                double SIN = sin(2 * M_PI * fcar * t);

                sum_I += (signal[idx] * COS).real();
                sum_Q += (signal[idx] * SIN).imag();
            }

            avg_I = sum_I / numSamples;
            avg_Q = sum_Q / numSamples;

            if (modTYPE == QPSK) {
                int bit1, bit2;

                if      (avg_I >= 0 && avg_Q >= 0)      { bit1 = 1; bit2 = 1; }
                else if (avg_I < 0 && avg_Q >= 0)       { bit1 = 0; bit2 = 1; }
                else if (avg_I < 0 && avg_Q < 0)        { bit1 = 0; bit2 = 0; }
                else                                    { bit1 = 1; bit2 = 0; }

                binary.push_back(bit1);
                binary.push_back(bit2);
            }        

            else if (modTYPE == QAM16) {

                int bit_I0, bit_I1, bit_Q0, bit_Q1;

                if      (avg_I < -1)         { bit_I1 = 0; bit_I0 = 0; }
                else if (avg_I < 0)          { bit_I1 = 0; bit_I0 = 1; }
                else if (avg_I < 1)           { bit_I1 = 1; bit_I0 = 0; }
                else                         { bit_I1 = 1; bit_I0 = 1; }

                if      (avg_Q < -1)         { bit_Q1 = 0; bit_Q0 = 0; }
                else if (avg_Q < 0)          { bit_Q1 = 0; bit_Q0 = 1; }
                else if (avg_Q < 1)          { bit_Q1 = 1; bit_Q0 = 0; }
                else                         { bit_Q1 = 1; bit_Q0 = 1; }

                
                binary.push_back(bit_I1);
                binary.push_back(bit_I0);
                binary.push_back(bit_Q1);
                binary.push_back(bit_Q0);

                /*
                cout << "bit_I1: " << bit_I1 << endl;
                cout << "bit_I0: " << bit_I0 << endl;
                cout << "bit_Q0: " << bit_Q1 << endl;
                cout << "bit_Q1: " << bit_Q1 << endl;
                */

            }

            else if (modTYPE == QAM64) {
				avg_I = avg_I ;   
				avg_Q = avg_Q ;

                //cout << "avgIqwe:" << avg_I << "  avgQ: " << avg_Q << endl;

                int bit_I0, bit_I1, bit_I2, bit_Q0, bit_Q1, bit_Q2;


                if      (avg_I < -3)        { bit_I2 = 0; bit_I1 = 0; bit_I0 = 0; }
                else if (avg_I < -2)        { bit_I2 = 0; bit_I1 = 0; bit_I0 = 1; }
                else if (avg_I < -1)        { bit_I2 = 0; bit_I1 = 1; bit_I0 = 0; }
                else if (avg_I < 0)         { bit_I2 = 0; bit_I1 = 1; bit_I0 = 1; }
                else if (avg_I < 1)         { bit_I2 = 1; bit_I1 = 0; bit_I0 = 0; }
                else if (avg_I < 2)         { bit_I2 = 1; bit_I1 = 0; bit_I0 = 1; }
                else if (avg_I < 3)         { bit_I2 = 1; bit_I1 = 1; bit_I0 = 0; }
                else                        { bit_I2 = 1; bit_I1 = 1; bit_I0 = 1; }


                if      (avg_Q < -3)        { bit_Q2 = 0; bit_Q1 = 0; bit_Q0 = 0; }
                else if (avg_Q < -2)        { bit_Q2 = 0; bit_Q1 = 0; bit_Q0 = 1; }
                else if (avg_Q < -1)        { bit_Q2 = 0; bit_Q1 = 1; bit_Q0 = 0; }
                else if (avg_Q < 0)         { bit_Q2 = 0; bit_Q1 = 1; bit_Q0 = 1; }
                else if (avg_Q < 1)         { bit_Q2 = 1; bit_Q1 = 0; bit_Q0 = 0; }
                else if (avg_Q < 2)         { bit_Q2 = 1; bit_Q1 = 0; bit_Q0 = 1; }
                else if (avg_Q < 3)         { bit_Q2 = 1; bit_Q1 = 1; bit_Q0 = 0; }
                else                        { bit_Q2 = 1; bit_Q1 = 1; bit_Q0 = 1; }

                binary.push_back(bit_I2);
                binary.push_back(bit_I1);
                binary.push_back(bit_I0);
                binary.push_back(bit_Q2);
                binary.push_back(bit_Q1);
                binary.push_back(bit_Q0);

                /*
                cout << "bit_I2: " << bit_I2 << endl;
                cout << "bit_I1: " << bit_I1 << endl;
                cout << "bit_I0: " << bit_I0 << endl;
                cout << "bit_Q2: " << bit_Q2 << endl;
                cout << "bit_Q1: " << bit_Q1 << endl;
                cout << "bit_Q0: " << bit_Q0 << endl;
                */
            }
        }

    }
};
