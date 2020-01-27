#include <stdexcept>
#include "rational.h"
#include <math.h>

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
  string tempThatAcceptsTheSlash;
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
  return temp;
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
bool Rational::operator==(const Rational a, const Rational b) const
{
  a.reduce();
  a.normalize0();
  b.reduce();
  b.normalize0();
  if (n != b.n)
    return false;
  if (d != b.d)
    return false;
  return true;
}
bool Rational::operator!=(const Rational a, const Rational b) const
{
  return !(a == b);
}
bool Rational::operator<(const Rational a, const Rational b) const
{
  if (a != b)
  {
    a.reduce();
    a.normalize0();
    b.reduce();
    b.normalize0();
    if (a.n/a.d < b.n/b.d)
      return true;
    else
      return false;
  }
}
void Rational::operator+=(Rational &a, const Rational b) //what does the const outside a function do?
{
  a = a + b;
}
void Rational::operator*=(Rational &a, const Rational b)
{
  a = a*b;
}
//used khanacademy's article on The Euclidean Algorithm for finding a GCD
static Rational::int gcd(int a, int b)
{
  if (a != 0 && b != 0)
    return gcd(b, a%b);
  else
    return b;
}
//used stackoverflow answer of how to find the lcm
static Rational::int lcm(int a, int b)
{
  return a*b/gcd(a,b);
}

void Rational::reduce()
{
  divisor = gcd(n, d)
  n /= divisor;
  d /= divisor;
}

void Rational::normalize0()
{
    if (n == 0 && d != 1) d = 1;
}
