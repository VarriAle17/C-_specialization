
 #include <iostream>

 using namespace std;
 
 class B {
 public:
    virtual void foo(int i){std::cout << "B::foo(int) " << i << "\n";}
    virtual void foo(double x){std::cout << "B::foo(double) " << x << "\n";}
 };

 class D: public B {
 public:
    void foo(int i){std::cout << "D::foo(int) " << i << "\n";}
 };


int main()
{
  D d;
  B b, *pb = &d;
  B& rb = d;
  b.foo(9); //chiama B::foo(int)
  b.foo(9.5);  // chiama B::foo(double)
  d.foo(9);  //chiama D::foo(int)
  d.foo(9.5);  //chiama D::foo(int)   //l'overload non c'è tra funzioni di namespace diversi
  pb -> foo(9.5); //chiama B::foo(double)
  pb->foo(9);   //chiama D::foo(int)
  rb.foo(9.5); //chiama B::foo(double)
  rb.foo(9);   //chiama D::foo(int)
}
