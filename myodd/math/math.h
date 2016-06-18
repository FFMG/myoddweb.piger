#pragma once
#ifndef __Math_h__
#define __Math_h__

#include "../common/includes.h"
#include <limits>
#include <assert.h>
#include <math.h>
#include <vector>
#include <map>

namespace myodd{ namespace math{
void Test();  //  test data

/**
 * Swap any two similar values
 *
 * @param T& the lhs element.
 * @param T& the rhs element.
 * @return none.
 */
template <typename T>
static void swap(T& a, T& b) 
{
    T temp = a;
    a = b;
    b = temp;
}

/**
 * truncate a variable, (normaly double or float)
 * @return double|float a number to truncate.
 * @return int a truncated number
 */
template <typename T>
static int trunc(const T& x) 
{
  return ( static_cast<int>(x > 0 ? floor(x) : ceil(x) ) ); 
}

/**
 * round a variable, (normaly double or float)
 * @return double|float a number to round.
 * @return int a rounded number
 */
template <typename T>
static int round(const T& x ) 
{
  return ( myodd::math::trunc( x > 0 ? (x+.5) : (x-.5) ) ); 
}

/**
 * round a number to a certain number of decimal places.
 * @param T the number we are rounding
 * @param unsigned short the number of places we are rounding to.
 * @return double the rounded number.
 */
template <typename T>
static T round(const T& x, unsigned short places ) 
{
  double p = static_cast<double>( pow( 10.f, places ) );
  int r = round( x * p );
  return static_cast<T>(r/p);
}

/**
 * Return is two numbers are very, very similar
 / this is useful after divisions for example.
 * @param const T& the left number we are comparing
 * @param const T& the right number we are comparing
 * @return bool if the two numbers are within 1e-10 of each others.
 */
template <typename T>
static bool AreAlmostSame( const T& lhs, const T& rhs )
{
   return ((lhs-rhs) < (1e-10)) && ((lhs-rhs) > -1e-10);
}

double modulo( double y, double x);

/**
 * get a random number between 2 numbers.
 */
int random_range(int lowest_number, int highest_number);

// ----------------------------------------------------------------------
template <typename T, typename U>
U Convert(T src );

// return the maximum of two values
template <typename T>
T min2(const T a, const T b) {
    return  a < b ? a : b;
}

// return the maximum of three values
template <typename T>
T min3 (const T a, const T b, const T c) {
  return (a < b ? min2 (a, c) : min2 (b, c));
}

// return the maximum of two values
template <typename T>
T max2(const T a, const T b) {
    return  a > b ? a : b;
}

// return the maximum of three values
template <typename T>
T max3 (const T a, const T b, const T c) {
  return (a > b ? max2 (a, c) : max2 (b, c));
}

int BCDToDecimal( int bcdNumber );

/**
 * Ensure that a number falls insides a given range.
 * @param const T& the number we want to be within the given range.
 * @param const U& the min value we are comparing to
 * @param const V& the max value we are comparing to.
 * @return T the value within range min/max. 
 */
template <typename T, typename U, typename V>
T inside (const T& x, const U& min, const V& max) 
{
  assert( min <=max );  // what range is that?

  if( x < min ) return static_cast<T>(min);
  if( x > max ) return static_cast<T>(max);
  return x;
}

double Bernstein(int n, int i, double t);

void Bezier2D
(  
  const std::vector<double> &originalPoints, 
  std::vector<double> &returnPoints, 
  const unsigned int maxNumberPoints =500 
);

// The factorial of a number
unsigned int factorial( unsigned int n );

// get the shortest point on a line away from a point.
void GetShortestPointOnLineFromLine
(
  double& point_xOnLine,  
  double& point_yOnLine,
  double point_x,
  double point_y,
  double line_x1,
  double line_y1,
  double line_x2,
  double line_y2
);

// the shortest distance of a point to a line.
double GetShortestDistanceFromLine
(
  double point_x,
  double point_y,
  double line_x1,
  double line_y1,
  double line_x2,
  double line_y2
);

class Vector
{
public:
  double x_, y_;

  Vector(double f = 0.0f)
    : x_(f), y_(f) {}

  Vector(double x, double y)
    : x_(x), y_(y) {}

  Vector(const Vector& v ){
    *this = v;
  }
  
  Vector operator-(const Vector &vect) const;
  bool operator!=(const Vector& v ) const;
  bool operator==(const Vector& v ) const;
  const Vector& operator=(const Vector& v );

  int ix() const;
  int iy() const;

  /**
  * Rotate the line around a pivot.
  * @param double the turn angle
  * @param Vector the pivot the line will be rotating along. Couble be the centre, one end or a point away from it.
  * @return none
  */
  void Rotate( double angle, const Vector& pivot );
};

class Line
{
public:
  Vector begin_;
  Vector end_;

  Line( ) : begin_(0), end_(0)
  {}

  Line(double beginx, double beginy, double endx, double endy)
    : begin_(Vector(beginx, beginy)), end_(Vector(endx, endy)) {}

  Line(const Vector& begin, const Vector& end)
    : begin_(begin), end_(end) {}

  enum IntersectResult { 
    PARALLEL, 
    COINCIDENT, 
    NOT_INTERESECTING, 
    INTERESECTING 
  };

  /**
   * Find where our line intercept the other line.
   * @param const Line the other line
   * @param Vector& where it intercepts the line
   * @return IntersectResult the interception, as it might not always intercept.
   */
  IntersectResult Intersect(const Line& other_line, Vector& intersection) const;

  /**
   * The centre of the line
   * @param none
   * @return Vector the centre of the line.
   */
  Vector Centre() const;

  /**
   * Rotate the line around a pivot.
   * @param double the turn angle
   * @param Vector the pivot the line will be rotating along. Couble be the centre, one end or a point away from it.
   * @return none
   */
  void Rotate( double angle, const Vector& pivot );
};

class Rectangle
{
public:
  Line left_; 
  Line top_;
  Line right_;
  Line bottom_;
  Rectangle()
  {}

  Rectangle( const Line& left, 
             const Line& top,
             const Line& right,
             const Line& bottom ) :
    left_(left ),  top_(top ),
    right_(right ),bottom_(bottom )
  {
  }

  Rectangle( const RECT& rect ) :
    left_( Vector(rect.left, rect.top ),  Vector(rect.left, rect.bottom ) ),
    top_( Vector(rect.left, rect.top ),  Vector(rect.right, rect.top ) ),
    right_( Vector(rect.right, rect.top ),  Vector(rect.right, rect.bottom ) ),
    bottom_( Vector(rect.left, rect.bottom ),  Vector(rect.right, rect.bottom ) )
  {
  }

  void SetRect( double l, double t, double r, double b );
  Vector Centre() const;
  const Vector& TopLeft() const;
  const Vector& TopRight() const;
  const Vector& BottomRight() const;
  const Vector& BottomLeft() const;

  void Rotate( double angle );

  enum IntersectResult { 
    NOT_INTERESECTING = 0x0, 
    INTERESECTING_LEFT = 0x1,
    INTERESECTING_TOP = 0x2,
    INTERESECTING_RIGHT = 0x4,
    INTERESECTING_BOTTOM = 0x8,
  };

  unsigned int Intersect(const Line& line, std::map<double, Vector>& vectors ) const;
  unsigned int Intersect(const Line& line ) const;

  bool Inside( const Vector& pt ) const;
  bool Inside( const Line& ln ) const;
};

// get the distance between 2 points
double GetDistanceBetweenPoints( double x1, double y1, double x2, double y2);
double GetDistanceBetweenPoints( Vector v1, Vector v2 );

// Get the length of a line.
double LengthOfLine( double x1, double y1, double x2, double y2 );
double LengthOfLine( Vector v1, Vector v2 );

double AngleOf2Points( const Vector& p0, const Vector& p1 );

int __fastcall ToBase10(const TCHAR* udata, BYTE fromBase);
LPCTSTR __fastcall ToBase( STD_TSTRING& ret, unsigned int base10number, BYTE base);

int Random( int min, int max );

class PIDController
{
public:

	/// Constructor
	PIDController(double p, double i, double d);

	/// Updates the controller.
	double Update( double target, double current );

	/// Returns the current control value.
	double GetIntegralValue() const	{ return m_integral; }

  double GetTimeDiference() const	{ return m_dTimeDiference; }

private:

	double m_E0, m_E1, m_E2;

  const double m_p, m_i, m_d;
  double m_integral;

  double m_lastTime;

  double m_dTimeDiference;
};

unsigned int LOUI(__int64 l);
unsigned int HIUI(__int64 h);
__int64  MAKEUI( unsigned int l, unsigned int h);

int LOI(__int64 l);
int HII(__int64 h);
__int64  MAKEI( int l, int h);

template <typename T>
class BitExtractor
{
public:
  BitExtractor(T data) : databits_(data)
  {}

  T Extract(size_t lowBit, size_t highBit) const
  {
    assert( lowBit <= sizeof(T)*8 ); 
    assert( highBit <= sizeof(T)*8 );

    int digitCount = highBit - lowBit + 1;
    T mask = (T)pow((double)2, (double)digitCount) - 1;
    return (databits_>>lowBit) & mask;
  }
private:
  T databits_;
};
double DotProduct( const Vector& v1, const Vector& v2 );

double BytesToMegabytes(double bytes);
} //  math
} //  myodd

#endif // __Math_h__