//Code to test for good random numbers
//Reference: https://rosettacode.org/wiki/Linear_congruential_generator#bc
//Reference: https://www.sanfoundry.com/cpp-program-implement-linear-congruential-generator-pseudo-random-number-generation/

    #include <iostream>
    #include <cmath>
    #include <openssl/rand.h>
    #include <stdio.h>
    #include <string.h>

    using namespace std;
    class mRND
    {
        public:
            void seed(unsigned int s)
            {
                _seed = s;
            }
     
        protected:
            mRND() :
//                _seed(0), _a(0), _c(0), _m(2147483648)
                  _seed(0), _a(0), _c(0), _m(3648)
            {
            }
            int rnd()
            {
                return (_seed = (_a * _seed + _c) % _m);
            }
            int _a, _c;
            unsigned int _m, _seed;
    };

    class MS_RND: public mRND
    {
        public:
            MS_RND()
            {
//                _a = 214013;
//                _c = 2531011;
                _a = 363;
                _c = 431;
            }
            int rnd()
            {
//                return mRND::rnd() >> 16;
                  return mRND::rnd();
        }
    };
  

    int gcd(int u, int v)
    {
           return (v != 0) ? gcd(v, u % v) : u;
    };

    int main(int argc, char* argv[])
    {
    int gcd_result;
    int msd_prev_rnd_num;
    int msd_curr_rnd_num;
    int msd_1_count;
    int sample_size=500;
    double msd_1_count_probability;
    double calc_pi;

        unsigned char uc1[10];
        unsigned char uc2[10];


        MS_RND ms_rnd;

    cout << endl << "Please enter a reasonable sample size in int: ";
    cin >> sample_size;
        cout << endl << "Sample Size: " << sample_size;

//      LCG RNG Code
//
//
        cout << endl << "LCG RAND:" << endl << "=========";

    msd_prev_rnd_num = ms_rnd.rnd();
    msd_1_count = 0;
        for (int x = 0; x < sample_size; x++)
        {
       msd_curr_rnd_num = ms_rnd.rnd();
           gcd_result = gcd(msd_prev_rnd_num,msd_curr_rnd_num);
           if (gcd_result == 1) {msd_1_count = msd_1_count + 1;}
//           cout << endl << "LCG Rand num: " << msd_prev_rnd_num;
       msd_prev_rnd_num = msd_curr_rnd_num;
    }
    cout << endl << "LCG_1_count: " << msd_1_count;

        msd_1_count_probability = (double)msd_1_count/sample_size;
    calc_pi = sqrt(6/(((double)msd_1_count)/sample_size));

    
    cout << endl << "Probability of GCD of two LCG RAND generated numbers for above sample size to be 1: " << msd_1_count_probability;
    cout << endl << "Calculation of Pi Value based on formula sqrt(6/probability): " << calc_pi << endl;

//    OpenSSL Rand_Bytes() Code
//
//
        cout << "OpenSSL Rand" << endl << "============ " << endl;
    cout << "OSSL Rand status: " << RAND_status() <<endl;
        RAND_bytes(uc1,10);
    msd_prev_rnd_num = (int)uc1[1];
    msd_1_count = 0;
        for (int x = 0; x < sample_size; x++)
        {
       RAND_bytes(uc2,10);
       msd_curr_rnd_num = (int)uc2[1];
           gcd_result = gcd(msd_prev_rnd_num,msd_curr_rnd_num);
           if (gcd_result == 1) {msd_1_count = msd_1_count + 1;}
//           cout << endl << "OSSL Rand num: " << msd_prev_rnd_num;
       msd_prev_rnd_num = msd_curr_rnd_num;
    }
    cout << "OSSL_1_count: " << msd_1_count << endl;

        msd_1_count_probability = (double)msd_1_count/sample_size;
    calc_pi = sqrt(6/(((double)msd_1_count)/sample_size));

    
    cout << "Probability of GCD of two OSSL RAND generated numbers for above sample size to be 1: " << msd_1_count_probability;
    cout << endl << "Calculation of Pi Value based on formula sqrt(6/probability): " << calc_pi << endl;

//      C++ RAND() code
//
//
        cout << "C++ Rand" << endl << "======== " <<endl;
        msd_prev_rnd_num = rand();
        msd_1_count = 0;
        for (int x = 0; x < sample_size; x++)
        {
           msd_curr_rnd_num = rand();
           gcd_result = gcd(msd_prev_rnd_num,msd_curr_rnd_num);
           if (gcd_result == 1) {msd_1_count = msd_1_count + 1;}
//           cout << endl << "C ++ Rand num: " << msd_prev_rnd_num;
           msd_prev_rnd_num = msd_curr_rnd_num;
        }
        cout << "C++_1_count: " << msd_1_count;

        msd_1_count_probability = (double)msd_1_count/sample_size;
        calc_pi = sqrt(6/(((double)msd_1_count)/sample_size));


        cout << endl << "Probability of GCD of two C++ RAND generated numbers for above sample size to be 1: " << msd_1_count_probability;
        cout << endl << "Calculation of Pi Value based on formula sqrt(6/probability): " << calc_pi << endl;


        return 0;
    }
