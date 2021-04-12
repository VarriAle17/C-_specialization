//   Convert this program to C++

//  change to C++ io

//  change to one line comments

//  change defines of constants to const

//  change array to vector<>

//  inline any short function


//#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std; //useful to avoid writing every time std::vector and similar

//#define N 40
const int N = 40;

//inline function with generics definition
template <class T>
inline void sum(T& p, int n, const vector<T>& d)
{
  //int i; not necessary here
  p = 0;

  for(int i = 0; i < n; ++i)

    p = p + d[i];

}

int main(void)

{

   //int i; not necessary here

   int accum = 0;

   vector<int> data;

   for(int i = 0; i < N; ++i)
    
      data.push_back(i);

    sum(accum, N, data);

    //printf("sum is %d\n", accum); 
    cout << "sum is " << accum << endl;
    return 0;

}