/* calculate area in meters*/
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>

#define PI 3.16
using namespace std;
int& prova1(int& a){
    return ++a;
}

void mc_count(double a, void f(double))
{ cout <<"here we are " << endl; f(a);}

void is_ok(double a){ cout << "value of a: " << a << endl;};

void prova(int (*a) [10]){
    std::cout << a[6]+1;
}

class Just_a_double {
public:
    double a;

    Just_a_double(double x) : a(x) { }
    Just_a_double() : Just_a_double(0) { }
};

int main(void)
{
    mc_count(5, is_ok);
    vector<char[256]>v;
    int array[10][10];

   // vector<int> ale;
   // ale[0] = 7;
  //  for (auto k : ale) cout << k<< endl;
//prova(array);
    int a =5;
    int& b = a;
    cout << b<< endl;
    int c = prova1(b);
    cout << b << endl;
    cout << c << endl;
    cout << ++b << endl;
    cout << c << endl;
    int& d = prova1(b);
    cout << ++d << endl;
    cout << b << endl;
    cout << c << endl;
    
    
    const int& p = 9;
   // int && t =p;
   // t++;
    cout << p << endl;
   // cout << t << endl;
    //int && t = move(p); //doesn't work
    
   // cout << ale;
   double pippo = 2.8;
   int pippo2 = pippo;
   cout <<pippo2;
    return 0;
}
