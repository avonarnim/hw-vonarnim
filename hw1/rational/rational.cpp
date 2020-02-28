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
    else
    {
      n = num;
      d = denom;
      reduce();
      normalize0();
    }

}
//outputs a string of the rational number in format "n/d"
std::ostream& operator<<(std::ostream& ostr, const Rational& r)
{
  Rational temp = r;
  if (r.n > 0 && r.d < 0)
  {
    temp.n *= -1;
    temp.d *= -1;
  }
  ostr << temp.n << "/" << temp.d;
  return ostr;
}
//takes in a rational number from a stringstream input
std::istream& operator>>(std::istream& istr, Rational& r)
{
  char tempThatAcceptsTheSlash;
  istr >> r.n;
  istr >> tempThatAcceptsTheSlash;
  istr >> r.d;
  r.reduce();
  r.normalize0();

  return istr;
}

//adds numbers by multiplying by the other number's denominator essentially
Rational Rational::operator+(const Rational& b)
{
  Rational temp;
  int denom = lcm(d, b.d);
  temp.n = n*denom/d + b.n*denom/b.d;
  temp.d = denom;
  temp.reduce();
  temp.normalize0();
  return temp;
}
//multiplies numbers by multiplying respective numberators and denominators
Rational Rational::operator*(const Rational& b)
{
  Rational temp;
  temp.n = n*b.n;
  temp.d = d*b.d;
  temp.reduce();
  temp.normalize0();
  return temp;
}
//does addition of an int LHS and rational RHS
Rational operator+(const int& a, const Rational& b)
{
  Rational temp;
  temp.n = a*b.d + b.n;
  temp.d = b.d;
  temp.reduce();
  temp.normalize0();
  return temp;
}
//adds an int RHS by multiplying it with the LHS denominator
Rational Rational::operator+(int a) const
{
  Rational temp;
  temp.n = a*d + n;
  temp.d = d;
  temp.reduce();
  temp.normalize0();
  return temp;
}
//multiplies int LHS with rational RHS
Rational operator*(const int& a, const Rational& b)
{
  Rational temp;
  temp.n = b.n*a;
  temp.d = b.d;
  temp.reduce();
  temp.normalize0();
  return temp;
}
//multiplies rational LHS with int RHS
Rational Rational::operator*(int a) const
{
  Rational temp;
  temp.n = n*a;
  temp.d = d;
  temp.reduce();
  temp.normalize0();
  return temp;
}

//raises root Rational to a passsed int.
Rational Rational::operator^(const int b) const
{
  Rational temp;
  if (b > 0)
  {
    temp.n = pow(n, b);
    temp.d = pow(d, b);
  }
  else if (b < 0)
  {
    temp.n = pow(d, -b);
    temp.d = pow(n, -b);
  }
  else if (b == 0)
  {
    temp.n = 1;
    temp.d = 1;
  }
  temp.reduce();
  temp.normalize0();
  return temp;
}
//checks equivalency
bool Rational::operator==(const Rational& b)
{
  Rational bNew = b;
  reduce();
  normalize0();
  bNew.reduce();
  bNew.normalize0();
  if (n != bNew.n)
    return false;
  if (d != bNew.d)
    return false;
  return true;
}
//uses equivalency function to check non equivalency
bool Rational::operator!=(const Rational& b)
{
  return !(*this == b);
}
//divides respective numerators and denominators to evaluate
bool Rational::operator<(const Rational& b)
{
  if (*this != b)
  {
    Rational bNew = b;
    reduce();
    normalize0();
    bNew.reduce();
    bNew.normalize0();
    if (double(this->n)/this->d < double(bNew.n)/bNew.d)
      return true;
    else
      return false;
  }
  else
    return false;
}
//returns *this instead of a temp
Rational& Rational::operator+=(const Rational& b)
{
  int denom = lcm(d, b.d);
  n = n*denom/d + b.n*denom/b.d;
  d = denom;
  reduce();
  normalize0();

  return *this;
}
Rational& Rational::operator+=(int a)
{
  n = n + a*d;
  return *this;
}

Rational& Rational::operator*=(const Rational& b)
{
  n = b.n*n;
  d = b.d*d;
  reduce();
  normalize0();
  // can i do : this = this->operator*(b);
  return *this;
}
Rational& Rational::operator*=(int a)
{
  n = n*a;
  return *this;
}

//used khanacademy's article on The Euclidean Algorithm for finding a GCD
int Rational::gcd(int a, int b)
{
  if (a != 0 && b != 0)
    return gcd(b, a%b);
  if (a == 0)
    return b;
  else
    return a;
}
//used stackoverflow answer of how to find the lcm
int Rational::lcm(int a, int b)
{
  return a*b/gcd(a,b);
}

void Rational::reduce()
{
  int divisor = gcd(n, d);
  n = n/divisor;
  d = d/divisor;
}

void Rational::normalize0()
{
    if (n == 0 && d != 1) d = 1;
}
