   #include <iostream>
   #include <cmath>
   
   using namespace std;
   typedef long double ll;
      
   class bbs{
   
       ll p, q, M, seed, actual;
           
       ll gcd(ll a, ll b){
           if(b == 0) return a;
           return gcd(b, fmod(a,b));
       }
       
       public:
       
       bbs(ll p, ll q, ll s){
           this->p = p;
           this->q = q;
           this->seed = s;
           M = p*q;
           actual = s;
       }
       
       ll getrandom(){
       
           ll r = fmod(actual*actual,M);
           actual = r;
           return r;
       }
       
       ll getirandom(int i){
       
           ll g = gcd(p, q);
           ll lcm = p*q/g;
           
           ll exp = 1;
           for(int j = 1; j <= i; ++j) exp = fmod((exp+exp),lcm);
               
           ll x0 = seed*seed;
           ll r = x0;
           
           for(int j = 2; j <= exp; ++j) r = fmod((r*x0),M);
           
           return r;
       }
   };
   
   int main(){
   
       ll p = 11;
       ll q = 19;
       ll s = 3;    
       ll M;
       M = p * q;

       bbs b(p, q, s);
       int n = 1000;
       cout << n << endl;
       for(int i = 0; i < n; ++i) {
           cout << b.getrandom()/M << endl;
       }
   }

