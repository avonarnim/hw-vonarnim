#include <stdexcept>
#include "rational.h"
#include <math.h>
#include <string>

Rational::Rational()
{
    n = 0;
    d = 1;
}
Rational::Rational(int num, int denom)
{
    if (denom == 0) throw std::invalid_argument("Can't have denom = 0");
    // Continue your implementation below.
    // You should likely call this->reduce() and this->normalize0()
    else
    {
      n = num;
      d = denom;
      this->reduce();
      this->normalize0();
    }

}
//outputs a string of the rational number in format "n/d"
std::ostream& operator<<(std::ostream& ostr, const Rational& r)
{
  ostr << r.n << "/" << r.d << std::endl;
  return ostr;
}
//takes in a rational number from a stringstream input
std::istream& operator>>(std::istream& istr, Rational& r)
{
  char tempThatAcceptsTheSlash = "";
  istr >> r.n;
  istr >> tempThatAcceptsTheSlash;
  istr >> r.d;
  return istr;
}

Rational Rational::operator+(const Rational b) const
{
  Rational temp;
  int denom = lcm(d, b.d);
  temp.n = n*denom/d + b.n*denom/b.d;
  temp.d = denom;
  temp.reduce();
  temp.normalize0();
  return temp; //i think these will go out of bounds.
}
Rational Rational::operator*(const Rational b) const
{
  Rational temp;
  temp.n = n*b.n;
  temp.d = d*b.d;
  temp.reduce();
  temp.normalize0();
  return temp;
}
Rational Rational::operator+(const int a, const Rational b)
{ //could probably do the 4 functions below by just calling each other
  Rational temp;
  temp.n = a*b.d + b.n;
  temp.d = b.d;
  temp.reduce();
  temp.normalize0();
  return temp;
}
Rational Rational::operator+(const Rational b, const int a)
{
  Rational temp;
  temp.n = a*b.d + b.n;
  temp.d = b.d;
  temp.reduce();
  temp.normalize0();
  return temp;
}
Rational Rational::operator*(const int a, const Rational b)
{
  Rational temp;
  temp.n = b.n*a;
  temp.d = b.d;
  temp.reduce();
  temp.normalize0();
  return temp;
}
Rational Rational::operator*(const Rational b, const int a)
{
  Rational temp;
  temp.n = b.n*a;
  temp.d = b.d;
  temp.reduce();
  temp.normalize0();
  return temp;
}
//raises root Rational to a passsed int.
//might need to change from friend to global or
Rational Rational::operator^(int b) const
{
  Rational temp;
  temp.n = pow(n, b);
  temp.d = pow(d, b);
  temp.reduce();
  temp.normalize0();
  return temp;
}
bool Rational::operator==(const Rational b)
{
  reduce();
  normalize0();
  b.reduce();
  b.normalize0();
  if (n != b.n)
    return false;
  if (d != b.d)
    return false;
  return true;
}
bool Rational::operator!=(const Rational b)
{
  return !(*this == b);
}
bool Rational::operator<(const Rational b)
{
  if (this->operator!=(b))
  {
    reduce();
    normalize0();
    b.reduce();
    b.normalize0();
    if (this.n/this.d < b.n/b.d)
      return true;
    else
      return false;
  }
}
Rational& Rational::operator+=(const Rational b)
{
  int denom = lcm(d, b.d);
  n = n*denom/d + b.n*denom/b.d;
  d = denom;
  reduce();
  normalize0();

  // can i do : this = this->operator+(b);
  return *this;
}
Rational& Rational::operator*=(const Rational b)
{
  n = b.n*a;
  d = b.d;
  reduce();
  normalize0();
  // can i do : this = this->operator*(b);
  return *this;
}
//used khanacademy's article on The Euclidean Algorithm for finding a GCD
int Rational::gcd(int a, int b)
{
  if (a != 0 && b != 0)
    return gcd(b, a%b);
  else
    return b;
}
//used stackoverflow answer of how to find the lcm
int Rational::lcm(int a, int b)
{
  return a*b/gcd(a,b);
}

void Rational::reduce()
{
  int divisor = gcd(n, d)
  n /= divisor;
  d /= divisor;
}

void Rational::normalize0()
{
    if (n == 0 && d != 1) d = 1;
}
