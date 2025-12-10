    #include<iostream>
//    #include<conio.h>
//    #include<stdlib.h>
    #include<cmath>

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
                _seed(0), _a(0), _c(0), _m(2147483648)
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
                _a = 214013;
                _c = 2531011;
            }
            int rnd()
            {
                return mRND::rnd() >> 16;
            }
    };
  

    class BSD_RND: public mRND
    {
        public:
            BSD_RND()
            {
                _a = 1103515245;
                _c = 12345;
            }
            int rnd()
            {
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

	int bsd_prev_rnd_num;
	int bsd_curr_rnd_num;
	int bsd_1_count;

	BSD_RND bsd_rnd;
        MS_RND ms_rnd;

        cout << "MS RAND:" << endl << "========";
	cout << endl << "Please enter a reasonable sample size in int: ";
	cin >> sample_size;
        cout << endl << "Sample Size: " << sample_size;

	msd_prev_rnd_num = ms_rnd.rnd();
	msd_1_count = 0;
        for (int x = 0; x < sample_size; x++)
        {
	   msd_curr_rnd_num = ms_rnd.rnd();
           gcd_result = gcd(msd_prev_rnd_num,msd_curr_rnd_num);
           if (gcd_result == 1) {msd_1_count = msd_1_count + 1;}
           cout << endl << "MS Rand num: " << msd_prev_rnd_num;
	   msd_prev_rnd_num = msd_curr_rnd_num;
	}
	cout << endl << "msd_1_count: " << msd_1_count;

        msd_1_count_probability = (double)msd_1_count/sample_size;
	calc_pi = sqrt(6/(((double)msd_1_count)/sample_size));

	
	cout << endl << "Probability of GCD of two LCG MS RAND generated numbers for above sample size to be 1: " << msd_1_count_probability;
	cout << endl << "Calculation of X(Pi) based on formula sqrt(6/probability): " << calc_pi << endl;

//        cout << endl << "BSD RAND:" << endl << "=========" << endl;
//        for (int x = 0; x < 500; x++)
//            cout << bsd_rnd.rnd() << endl;
 
        return 0;
    }

