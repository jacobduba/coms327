#include "string327.h"

using namespace std;

int main(int argc, char *argv[])
{
  string327 s;
  // call default constructor, and then call assignment operator
  // THat is not correct.  This is a call to string327(const char *)
  string327 t = " world!";

  cout << s << endl;

  s = "Hello";

  cout << s << t << endl;
  cout << s + t << endl;

  s += t;

  cout << s << endl;

  // >> stops at first whitespace character by default.
  // To change this behavior, use iomanip
  cin >> s;
  cout << s << endl;

  return 0;
}
