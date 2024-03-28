#include <iostream>

using namespace std;

void cswap(int *x, int *y)
{
  int tmp;

  tmp = *x;
  *x = *y;
  *y = tmp;
}

void cppswap(int &x, int &y)
{
  int tmp;

  tmp = x;
  x = y;
  y = tmp;
}

int main(int argc, char *argv[])
{
  int a, b;

  int &r = a; // r is a reference to a; that is, r and a are aliases

  //  r = a;// a = a;

  a = 0;
  b = 1;

  cout << "a == " << a << ", b == " << b << endl;
  
  cswap(&a, &b);
  
  cout << "a == " << a << ", b == " << b << endl;

  cppswap(a, b);

  cout << "a == " << a << ", b == " << b << endl;
  
  r = b;

  cout << "a == " << a << ", b == " << b << endl;
  
  return 0;
}
