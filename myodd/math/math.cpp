#include "stdafx.h"

#include <tchar.h>
#include "string.h"
#include "math.h"
#include <limits>

namespace myodd{ namespace math{
void Test()
#ifdef _DEBUG
{
  __int64 a = MAKEUI( 10, 20 );
  unsigned int b = LOUI( a );
  unsigned int c = HIUI( a );
  assert( b == 10 && c == 20 );

  __int64 a1 = MAKEUI( 0xffffffff, 0xffffffff );
  unsigned int b1 = LOUI( a1 );
  unsigned int c1 = HIUI( a1 );
  assert( b1 == 0xffffffff && c1 == 0xffffffff );

  unsigned int bb = (unsigned int)random_range( 0, 0xffffffff );
  unsigned int cc = (unsigned int)random_range( 0, 0xffffffff );
  __int64 a2 = MAKEUI( bb, cc );
  unsigned int b2 = LOUI( a2 );
  unsigned int c2 = HIUI( a2 );
  assert( b2 == bb && c2 == cc );

  // make sure that the random number is within range.
  for( int loop = 0; loop < 10; ++loop )
  {
    int r = random_range( -1, 0 );
    assert( r >= -1 && r <= 0 );

    r = random_range( -1, -1 );
    assert( r == -1 );

    r = random_range( 200, 500 );
    assert( r >= 200 && r <= 500 );
    r = random_range( 500, 200 );
    assert( r >= 200 && r <= 500 );
    r = random_range( -500, -200 );
    assert( r >= -500 && r <= -200 );
  }

  // Check the various base conversions.
  assert( ToBase10( _T("A"), 16 ) == 10 );
  assert( ToBase10( _T("0A"), 16 ) == 10 );            // Zeros don't matter.
  assert( ToBase10( _T("0000A"), 16 ) == 10 );
  assert( ToBase10( _T("a"), 16 ) == 10 );             // not case sensitive
  assert( ToBase10( _T("G"), 16 ) == -1 );             // 'G' is not base 16
  assert( ToBase10( _T("AA"), 16 ) == 170 );
  assert( ToBase10( _T("ABCDEF"), 16 ) == 11259375 );
  assert( ToBase10( _T("AbCdEf"), 16 ) == 11259375 );
  assert( ToBase10( _T("101010111100110111101111"), 2) == 11259375 );  //  base 2
  assert( ToBase10( _T("52746757"), 8) == 11259375 );  // base 8
  assert( ToBase10( _T("Z"), 36 ) == 35 );             // base 36
  assert( ToBase10( _T("2S"), 36 ) == 100 );
  assert( ToBase10( _T("100"), 36 ) == 1296 );

  // the other way around.
  STD_TSTRING ret = _T("");                         // base 36
  ToBase( ret, 10, 36 );
  assert( ret == _T("A") );
  ToBase( ret, 100, 36 );
  assert( ret == _T("2S") );

  ToBase( ret, 11259375, 2 );
  assert( ret == _T("101010111100110111101111"));   // base 2

  ToBase( ret, 11259375, 8 );
  assert( ret == _T("52746757"));                   // base 8

  ToBase( ret, 11259375, 16 );
  assert( ret == _T("ABCDEF"));                     // base 16

  // edge cases.
  ToBase( ret, 0, 36 );
  assert( ret == _T("0") );                         // 0- base 36 number
  ToBase( ret, 0, 16 );
  assert( ret == _T("0") );                         // 0- base 16 number
  ToBase( ret, 0, 8 );
  assert( ret == _T("0") );                         // 0- base 8 number
  ToBase( ret, 0, 2 );
  assert( ret == _T("0") );                         // 0- base 2 number

  assert( ToBase10( _T("0"), 36 ) == 0 );
  assert( ToBase10( _T("0"), 16 ) == 0 );
  assert( ToBase10( _T("0"), 8 ) == 0 );
  assert( ToBase10( _T("0"), 2 ) == 0 );

  assert( ToBase10( _T("0"), 40 ) == -1 );          // Invalid bases.
  assert( ToBase10( _T("0"), 1 ) == -1 );
  assert( ToBase10( _T("AAAAA"), 1 ) == -1 );
  assert( ToBase10( _T("AAAAA"), 40 ) == -1 );

  ToBase( ret, (unsigned int)-1, 36 );
  assert( ret == _T("1Z141Z3") );                   // the max base 36 number
  ToBase( ret, (unsigned int)-1, 16 );
  assert( ret == _T("FFFFFFFF") );                  // the max base 16 number
  ToBase( ret, (unsigned int)-1, 8 );
  assert( ret == _T("37777777777") );               // the max base 8 number
  ToBase( ret, (unsigned int)-1, 2 );
  assert( ret == _T("11111111111111111111111111111111") );                
                                                    // the max base 2 number

  assert( max2( 1, 2) == 2 );
  assert( max2( 2, 1) == 2 );

  assert( max3( 1, 2, 3) == 3 );
  assert( max3( 2, 1, 3) == 3 );
  assert( max3( 3, 2, 1) == 3 );
  assert( max3( 3, 1, 2) == 3 );

  assert( min2( 1, 2) == 1 );
  assert( min2( 2, 1) == 1 );

  assert( min3( 1, 2, 3) == 1 );
  assert( min3( 2, 1, 3) == 1 );
  assert( min3( 3, 2, 1) == 1 );
  assert( min3( 3, 1, 2) == 1 );

  assert( inside( 10, -10, 100 ) == 10 );
  assert( inside( -11, -10, 100 ) == -10 );
  assert( inside( 101, -10, 100 ) == 100 );
}
#else
{
  __noop;
}
#endif

/**
 * Calculate the factorial of a number or n!
 * @param unsigned int the number that we want the factorial of.
 * @return unsigned int the factorial of n.
 */
unsigned int factorial( unsigned int n )
{
  unsigned int result = 1;
  for ( unsigned int m = 1; m <= n; ++m )
  {
    result *= m;
  }
  return result;
}

/**
 * Private helper function for Bernstein( .. )
 * @see Bernstein( ... )
 */
double _Ni(unsigned int n, unsigned int i)
{
  ASSERT( n >= i );
  double ni;
  double a1 = static_cast<double>(factorial(n));
  double a2 = static_cast<double>(factorial(i));
  double a3 = static_cast<double>(factorial(n - i));
  ni =  a1/ (a2 * a3);
  return ni;
}

/*
 * Calculate Bernstein basis
 * @return double the bernstein basis. 
 */
double Bernstein(int n, int i, double t)
{
  double basis;
  double ti; /* t^i */
  double tni; /* (1 - t)^i */

  /* Prevent problems with pow */

  if (t == 0.0 && i == 0) 
    ti = 1.0; 
  else 
    ti = pow(t, i);

  if (n == i && t == 1.0) 
    tni = 1.0; 
  else 
    tni = pow((1 - t), (n - i));

  //Bernstein basis
  basis = _Ni(n, i) * ti * tni; 
  return basis;
}

/**
 * Return a Quadratic set of bezier points.
 */
void Bezier2D
( 
  const std::vector<double> &originalPoints, 
  std::vector<double> &returnPoints, 
  const unsigned int maxNumberPoints /* =500*/ 
)
{
  size_t npts = (originalPoints.size() ) / 2;
  int icount, 
      jcount;
  double step, 
         t;

  // Calculate points on curve
  returnPoints.erase( returnPoints.begin(), returnPoints.end() );
  if( maxNumberPoints == 0 || npts == 0)
  {
    return;
  }
  returnPoints.reserve( maxNumberPoints );

  icount = 0;
  t = 0;
  step = (double)1.0 / (maxNumberPoints/2 - 1);

  for (unsigned int i1 = 0; i1 != maxNumberPoints/2; ++i1)
  { 
    if ((1.0 - t) < 5e-6) 
      t = 1.0;

    jcount = 0;
    returnPoints.push_back( 0.0 );
    returnPoints.push_back( 0.0 );
    for (unsigned int i2 = 0; i2 != npts; ++i2)
    {
      double basis = Bernstein( int(npts - 1), i2, t);
      returnPoints[icount] += basis * originalPoints[jcount];
      returnPoints[icount + 1] += basis * originalPoints[jcount + 1];
      jcount = jcount +2;
    }

    icount += 2;
    t += step;
  }
}

/**
 * Get the distance between VECTOR 1 and VECTOR 2
 * @param Vector the first vector
 * @param Vector the second vector
 * @return double the distance between point 1 and 2
 */
double GetDistanceBetweenPoints( Vector v1, Vector v2 )
{
  return GetDistanceBetweenPoints( v1.x_, v1.y_, v2.x_, v2.y_ );
}

/**
 * Get the distance between POINT 1 and POINT 2
 * @param double x value of POINT 1
 * @param double y value of POINT 1
 * @param double x value of POINT 2
 * @param double y value of POINT 2
 * @return double the distance between point 1 and 2
 */
double GetDistanceBetweenPoints
( 
  double x1, 
  double y1, 
  double x2, 
  double y2
)
{
    return sqrt( ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)) ) ;
}

/**
 * Get the shortest distance from a point to a line.
 * The shortest distance is the right angle line from the point to the line
 * Or, if the right angle will never reach the line then it is the end point.
 * @param double x the point x away from the line
 * @param double y the point y away from the line
 * @param double x the point1 x of the line
 * @param double y the point1 y of the line
 * @param double x the point2 x of the line
 * @param double y the point2 y of the line
 * @return double the shortest distance from the point to the line.
 */
double GetShortestDistanceFromLine
( 
  double point_x,
  double point_y,
  double line_x1,
  double line_y1,
  double line_x2,
  double line_y2
)
{
  // get the point on the line the shortest away from the point.
  double xx,yy;
  myodd::math::GetShortestPointOnLineFromLine( xx, yy,
                                  point_x, point_y,
                                  line_x1, line_y1,
                                  line_x2, line_y2 
                                 );

    // return the distance between the two points.
  return GetDistanceBetweenPoints( point_x, point_y, xx, yy );
}

/**
 * Get the shortest distance from a point to a line.
 * The shortest distance is the right angle line from the point to the line
 * Or, if the right angle will never reach the line then it is the end point.
 * @param double& ref x the point x on the line the shortest away from the point.
 * @param double& ref y the point y on the line the shortest away from the point.
 * @param double x the point x away from the line
 * @param double y the point y away from the line
 * @param double x the point1 x of the line
 * @param double y the point1 y of the line
 * @param double x the point2 x of the line
 * @param double y the point2 y of the line
 * @return none
 */
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
)
{
  double A = point_x - line_x1;
  double B = point_y - line_y1;
  double C = line_x2 - line_x1;
  double D = line_y2 - line_y1;

  double dot = A * C + B * D;
  double len_sq = C * C + D * D;
  double param = dot / len_sq;

  if(param < 0)
  {
    point_xOnLine = line_x1;
    point_yOnLine = line_y1;
  }
  else if(param > 1)
  {
    point_xOnLine = line_x2;
    point_yOnLine = line_y2;
  }
  else
  {
    point_xOnLine = line_x1 + param * C;
    point_yOnLine = line_y1 + param * D;
  }
}

/**
 * get the length of a line.
 * @param Vector& the 1st point
 * @param Vector& the 2nd point
 * @return double the length of the line between the two points.
 */
double LengthOfLine( Vector v1, Vector v2 )
{
  return LengthOfLine( v1.x_, v1.y_, v2.x_, v2.y_ );
}

/**
 * get the length of a line.
 * @param double x1 the x,y starting point.
 * @param double y1
 * @param double x2 the x,y ending point.
 * @param double y2
 * @return double the length of the line between the two points.
 */
double LengthOfLine( double x1, double y1, double x2, double y2 )
{
  return sqrt( pow(abs(x1-x2), 2) + pow(abs(y1-y2), 2));
}

/**
 * Given two points on a graph we need to calculate the angle.
 * note that the x1,y1->x2,y2 line is 90 degrees off a 'real' world compass, (as it is on a graph).
 * so we substrac the 90 degree.
 * @param Vector& the 1st point
 * @param Vector& the 2nd point
 * @return double the angle
 */
double AngleOf2Points( const Vector& pt1, const Vector& pt2 )
{

  static const double PI				        =	3.1415926535897932384626433832795;
  static const double radian_to_degree  = (180.0/PI);
  double angle = (radian_to_degree * atan2((pt2.y_ - pt1.y_), (pt2.x_ - pt1.x_)));
  angle -= 90;
  if( angle < 0 ){
    angle+= 360.f;
  }
  return angle;
}

/**
 * Convert a string to an integer.
 * The function returns -1 if there is any problem!
 * 
 * @param const TCHAR* the string we are converting.
 * @param BYTE the base we are converting from.
 * @return int the converted number.
 */
int __fastcall ToBase10(const TCHAR* udata, BYTE fromBase)
{
  //  sanity check
  // 1- We only support base 2 -> 36
  if( fromBase > 36 || fromBase < 2 )
  {
    return -1; // Invalid base.
  }

  // 2- Cannot be NULL
  if( !udata )
  {
    return -1; // Cannot convert NULL value.
  }

  static const TCHAR numdigits[] = TEXT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  size_t udatalen = _tcslen( udata );

  long digitValue = 0;
  int retVal = 0;
  TCHAR digits[ sizeof(numdigits)+1 ];
  TCHAR *dataVal;

  //copy the data to our variable
  TCHAR *data = new TCHAR[udatalen+1];
  _tcscpy_s(data, udatalen+1, udata);
  //convert it to upper case

  _tcsupr_s( data, udatalen+1);
  memset(digits, 0, sizeof(digits));
  //copy the number of digits supported by base in digits

  _tcsncpy_s(digits, numdigits, fromBase);
  for( size_t index = 0; index < udatalen; ++index)
  {
    //is the number there
    dataVal = _tcschr(digits, data[index] );
    if(dataVal != 0 )
    {
      // if it is subtract where to start point.
      digitValue = int(dataVal - digits);

      // increment Retval with digitvalue.
      retVal = retVal * fromBase + digitValue;
    }
    else
    {
      retVal = -1;  // invalid digit for that base.
      break;
    }
  }

  // all done.
  delete [] data;

  //return the result
  return retVal;
}

/**
 * Convert a base 10 number to a given BASE.
 * @param STD_TSTRING& the container to the return value.
 * @param unsigned int the base 10 number.
 * @return NULL|string the converted number.
 */
LPCTSTR __fastcall ToBase( STD_TSTRING& ret, unsigned int base10number, BYTE base)
{
  //  reset the return value.
  ret = _T("");
  //  sanity check
  // 1- We only support base 2 -> 36
  if( base > 36 || base < 2 )
  {
    return NULL;
  }

  //  no need to try to convert 0.
  if( 0 != base10number )
  {
    static const TCHAR numdigits[] = TEXT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    long digitValue = 0;
    
    // copy only the numbers we need, (depending on the base).
    TCHAR digits[sizeof(numdigits) + 1];
    memset(digits, 0, sizeof(digits));
    _tcsncpy_s(digits, numdigits, base);
    while(base10number)
    {
      digitValue = base10number % base;
      base10number = (base10number-digitValue)/base;
      ret = digits[digitValue] + ret;
    }
  }
  else
  {
    ret = _T("0" ); //  zero is zero in all bases.
  }
  return ret.c_str();
}

/**
 * return the modulo of doubles, the remainder on dividing y by x.
 * @param double the number
 * @param double the divider
 * @param double the remainder.
 */
double modulo( double y, double x)
{
  double mod = y - x * floor(y/x);
  ASSERT( 0 <= mod && mod <= x );
  return mod;
}

/**
 * get a random number between 2 numbers.
 * @param int the lowest number in the range
 * @param int the highest number in the range
 * @return int the random number between the two.
 */
int random_range(int lowest_number, int highest_number)
{
  // make sure the numbers are the right way around.
  if(lowest_number > highest_number)
  {
    swap(lowest_number, highest_number);
  }

  size_t range = highest_number - lowest_number + 1;
  return lowest_number + int(range * rand()/(RAND_MAX + 1.0));
}

/**
 * string conversions of STD_TSTRING
 * @see _tstof
 * @param const STD_TSTRING& the number we want to convert.
 * @return double the double conversion of the string
 */
double std_tstof(const STD_TSTRING& s)
{
  return _tstof( s.c_str() );
}

/**
 * string conversions of STD_TSTRING
 * @see _tstol
 * @param const STD_TSTRING& the number we want to convert.
 * @return long the long conversion of the string
 */
long std_tstol(const STD_TSTRING& s)
{
  return _tstol( s.c_str() );
}

/**
 * string conversions of STD_TSTRING
 * @see _tstoi
 * @param const STD_TSTRING& the number we want to convert.
 * @return int the int conversion of the string
 */
int std_tstoi(const STD_TSTRING& s)
{
  return _tstoi( s.c_str() );
}

/**
 * Convert a BCD number to the decimal equivalent.
 * @param int the BCD number been converted.
 * @return int the decimal number.
 */
int BCDToDecimal( int bcdNumber )
{
  int toDecimal = 0;
  int digits = 0;
  for( BYTE b = 0; b < 32; b+= 4 )
  {
    int multiplier = static_cast<int>(pow( 10.f, digits ));
    toDecimal += (((bcdNumber >> b) & 0xf ) * multiplier);
    ++digits;
  }
  return (toDecimal);
}

/// @param	kp	P parameter.
/// @param	ki	I parameter.
/// @param	kd	D parameter.
PIDController::PIDController(double p, double i, double d)
	: m_p( p ), 
    m_i( i ), 
    m_d( d ),
	  m_E0( 0.f ),
	  m_E1( 0.f ),
	  m_E2( 0.f ),
    m_integral( 0.f ),
    m_dTimeDiference( 0.f )
{
  m_lastTime = static_cast<double>(GetTickCount());
}
 
/**
 * @param	double sv	Set value. This is the target value.
 * @param	double pv	Process value. This is the measured (or current) value.
 * @return double the control value.
 */
double PIDController::Update
( 
  double target, 
  double current
)
{
  /// @param	dt	The time elapsed since the last update.
  double newTime = static_cast<double>(GetTickCount());
  m_dTimeDiference = ((newTime - m_lastTime) / 1000.f);

  // save the last time
  m_lastTime = newTime;

  // If the elapsed time is zero (or less) ignore this update.
	if ( GetTimeDiference() <= 0.f )
	{
		return GetIntegralValue();
	}

	// Only the last 3 error values are needed.

	m_E2 = m_E1;
	m_E1 = m_E0;
	m_E0 = target - current;

	double const	e01	= m_E0 - m_E1;
	double const	e12	= m_E1 - m_E2;
	double const	p	= m_p * e01;
	double const	i	= m_i * m_E0 * GetTimeDiference();
	double const	d	= m_d * ( e01 - e12 ) / GetTimeDiference();

	m_integral += p + i + d; 
  return m_integral;
} 

/**
 * Get the low number from an __int64 number
 * @param __int64 the number
 * @return unsigned int the low number in the __int64
 */
unsigned int LOUI(__int64 l)
{
  return ((unsigned int)(((__int64)(l)) & 0xffffffff));
}

/**
 * Get the high number from an __int64 number
 * @param __int64 the number
 * @return unsigned int the low number in the __int64
 */
unsigned int HIUI(__int64 h)
{
  return ((unsigned int)((((__int64)(h)) >> 32) & 0xffffffff));
}

/**
 * 'Join' two unsigned integers together.
 * @param unsigned int the low number
 * @param unsigned int the high number
 * @return __int64 the int 64 number.
 */
__int64  MAKEUI( unsigned int l, unsigned int h)
{
  return ((__int64)(((unsigned int)((l) & 0xffffffff)) | ((__int64)((((h)) & 0xffffffff))) << 32));
}

/**
 * 'Join' two integers together.
 * @param int the low number
 * @param int the high number
 * @return __int64 the int 64 number.
 */
__int64  MAKEI( int l, int h)
{
  return ((__int64)(((int)((l) & 0xffffffff)) | ((__int64)((((h)) & 0xffffffff))) << 32));
}

/**
 * Get the low number from an __int64 number
 * @param __int64 the number
 * @return int the low number in the __int64
 */
int LOI(__int64 l)
{
  return ((int)(((__int64)(l)) & 0xffffffff));
}

/**
 * Get the high number from an __int64 number
 * @param __int64 the number
 * @return int the low number in the __int64
 */
int HII(__int64 h)
{
  return ((int)((((__int64)(h)) >> 32) & 0xffffffff));
}

// --------------------------------------------------------------------
/**
 * int value of the x_ point
 * @param none
 * @return int rounded value of x_
 */
int Vector::ix() const{
  return round(x_);
}

/**
 * int value of the y_ point
 * @param none
 * @return int rounded value of y_
 */
int Vector::iy() const{
  return round(y_);
}

/**
 * Rotate the Vector around a pivot.
 * @param double the turn angle
 * @param Vector the pivot the line will be rotating along. Could be the centre, one end or a point away from it.
 * @return none
 */
void Vector::Rotate( double angle, const Vector& pivot )
{
  static const double geo_pi = 3.1415926535897932384626433832795;
  static const double degree_to_radian = (geo_pi/180.0);
  double radian = degree_to_radian*angle;

  double x = cos(radian)*(x_-pivot.x_) - sin(radian)*(y_-pivot.y_) + pivot.x_;
  double y = sin(radian)*(x_-pivot.x_) + cos(radian)*(y_-pivot.y_) + pivot.y_;

  x_ = x;
  y_ = y;
}

/**
 * Subtract this vector with another
 * @param const Vector& the vector we are subtracting from this one.
 * @return the subtracted vector.
 */
Vector Vector::operator-(const Vector &vect) const
{
  return Vector(x_ - vect.x_, y_ - vect.y_);
}

/**
 * Compare this operator with another
 * @param Vector& the vector we are comparing to
 * @return bool if both vectors are not the same.
 */
bool Vector::operator!=(const Vector& v ) const
{
  return (x_ != v.x_ || y_ != v.y_);
}

/**
 * Compare this operator with another
 * @param Vector& the vector we are comparing to
 * @return bool if both vectors are exactly the same.
 */
bool Vector::operator==(const Vector& v ) const
{
  return (x_ == v.x_ && y_ == v.y_);
}

/**
* Assignment operator
* @param Vector& the vector we assigning the value from.
* @return const Vector& this vector.
*/
const Vector& Vector::operator=(const Vector& v )
{
  if( this != &v )
  {
    x_ = v.x_;
    y_ = v.y_;
  }
  return *this;
}

// --------------------------------------------------------------------
/**
* Find where our line intercept the other line.
* @param const Line the other line
* @param Vector& where it intercepts the line
* @return IntersectResult the interception, as it might not always intercept.
*/
Line::IntersectResult Line::Intersect(const Line& other_line, Vector& intersection) const
{
  double denom = ((other_line.end_.y_ - other_line.begin_.y_)*(end_.x_ - begin_.x_)) -
    ((other_line.end_.x_ - other_line.begin_.x_)*(end_.y_ - begin_.y_));

  double nume_a = ((other_line.end_.x_ - other_line.begin_.x_)*(begin_.y_ - other_line.begin_.y_)) -
    ((other_line.end_.y_ - other_line.begin_.y_)*(begin_.x_ - other_line.begin_.x_));

  double nume_b = ((end_.x_ - begin_.x_)*(begin_.y_ - other_line.begin_.y_)) -
    ((end_.y_ - begin_.y_)*(begin_.x_ - other_line.begin_.x_));

  if(denom == 0.0f)
  {
    if(nume_a == 0.0f && nume_b == 0.0f)
    {
      return COINCIDENT;
    }
    return PARALLEL;
  }

  double ua = nume_a / denom;
  double ub = nume_b / denom;

  if(ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f)
  {
    // Get the intersection point.
    intersection.x_ = begin_.x_ + ua*(end_.x_ - begin_.x_);
    intersection.y_ = begin_.y_ + ua*(end_.y_ - begin_.y_);

    return INTERESECTING;
  }

  return NOT_INTERESECTING;
}

/**
 * The centre of the line
 * @param none
 * @return Vector the centre of the line.
 */
Vector Line::Centre() const
{
  double cx = (begin_.x_ + end_.x_) / 2;
  double cy = (begin_.y_ + end_.y_) / 2;
  return Vector( cx, cy );
}

/**
 * Rotate the line around a pivot.
 * @param double the turn angle
 * @param Vector the pivot the line will be rotating along. Could be the centre, one end or a point away from it.
 * @return none
 */
void Line::Rotate( double angle, const Vector& pivot )
{
  begin_.Rotate( angle, pivot );
  end_.Rotate( angle, pivot );
}

// --------------------------------------------------------------------
/**
 * Check if any part of a line is inside a rectangle.
 * This will only return true if the line is completely inside the rectangle.
 * @param const Line& the line we want to test for.
 * @return bool if the line is inside the rectangle or not.
 */
bool Rectangle::Inside( const Line& ln ) const
{
  return (Inside( ln.begin_ ) && Inside( ln.end_ ));
}

/**
 * Check if a point is inside a rectangle.
 * @param const Vector& the point we want to check is inside the rectangle.
 * @return bool if the vector is inside the rectangle or not.
 */
bool Rectangle::Inside( const Vector& pt) const
{
  //corners of rectangle
  const Vector& p1 = TopLeft();
  const Vector& p2 = TopRight();
  const Vector& p3 = BottomRight();
  const Vector& p4 = BottomLeft();
  
  Vector v1 = p2 - p1; //find edge vectors
  Vector v2 = p3 - p2;
  Vector v3 = p4 - p3;
  Vector v4 = p1 - p4;

  double f1 = DotProduct(v1, pt-p1);
  double f2 = DotProduct(v2, pt-p2);
  if( f1 * f2 < 0 )
  {
    // one is negative and the other is positive
    return false;
  }
  double f3 = DotProduct(v3, pt-p3);
  double f4 = DotProduct(v4, pt-p4);
  if( f3 * f4 < 0 )
  {
    // one is negative and the other is positive
    return false;
  }

  //if all dot products are negative or positive
  if( (f1 < 0 && f2 < 0 && f3 < 0 && f4 < 0) || (f1 > 0 && f2 > 0 && f3 > 0 && f4 > 0) )
  {
    return true;
  }
  return false;
}

/**
 * Set the values of the rectangle given a 2D rectangle.
 * @param double left
 * @param double top
 * @param double right
 * @param double bottom
 * @return none
 */
void Rectangle::SetRect( double l, double t, double r, double b )
{
  // normalise the top/left
  if( l > r ) swap( l, r );
  if( t > b ) swap( t, b );

  // set the values.
  left_.begin_ = Vector(l, t );
  left_.end_ = Vector(l, b );
  top_.begin_ = Vector(l, t );  
  top_.end_ = Vector(r, t );
  right_.begin_ = Vector(r, t );  
  right_.end_ = Vector(r, b );
  bottom_.begin_ = Vector(l, b );  
  bottom_.end_ = Vector(r, b );
}

/**
 * Rotate the rectangle on the centre point.
 * @param double the angle we want to rotate the angle. the angle is in degrees.
 * @return none
 */
void Rectangle::Rotate( double angle )
{
  Vector vC = Centre();

  left_.Rotate( angle, vC );
  top_.Rotate( angle, vC );
  right_.Rotate( angle, vC );
  bottom_.Rotate( angle, vC );
}

/**
 * Get the centre of the rectangle.
 * @param none
 * @return Vector the centre point of the rectangle.
 */
Vector Rectangle::Centre() const
{
  Line diag( TopLeft(), BottomRight() );
  return diag.Centre();
}

/**
 * Get the point where the given line intercept our rectangle.
 * @param const Line& line the line we are testing against our rectangle.
 * @return unsigned int OR IntersectResults to show where the interception was.
 */
unsigned int Rectangle::Intersect(const Line& line ) const
{
  std::map<double, Vector> dummyVector;
  return Intersect( line, dummyVector );
}

/**
 * Get the point where the given line intercept our rectangle.
 * @param const Line& line the line we are testing against our rectangle.
 * @param std::map<double, Vector>& map with the intectept point in order of distance from the begining of the given line. The first point of the map is the distance.
 * @return unsigned int OR IntersectResults to show where the interception was.
 */
unsigned int Rectangle::Intersect(const Line& line, std::map<double, Vector>& vectors ) const
{
  unsigned int ir = NOT_INTERESECTING;
  Vector v;
  if( Line::INTERESECTING == left_.Intersect( line, v ) )
  {
    vectors[GetDistanceBetweenPoints( line.begin_, v )] = v;
    ir |= INTERESECTING_LEFT;
  }
  if( Line::INTERESECTING == top_.Intersect( line, v ) )
  {
    vectors[GetDistanceBetweenPoints( line.begin_, v )] = v;
    ir |= INTERESECTING_TOP;
  }
  if( Line::INTERESECTING == right_.Intersect( line, v ) )
  {
    vectors[GetDistanceBetweenPoints( line.begin_, v )] = v;
    ir |= INTERESECTING_RIGHT;
  }
  if( Line::INTERESECTING == bottom_.Intersect( line, v ) )
  {
    vectors[GetDistanceBetweenPoints( line.begin_, v )] = v;
    ir |= INTERESECTING_BOTTOM;
  }
  assert(vectors.size() <= 2 );  // how can the, (straight), line cross more than twice?

  // the map is ordered by distance form the start to the end
  return ir;
}

/**
 * Get the top left corner of the rectangle.
 * @param none
 * @return const Vector& the top left corner.
 */
const Vector& Rectangle::TopLeft() const
{
  return left_.begin_;
}

/**
 * Get the top right corner of the rectangle.
 * @param none
 * @return const Vector& the top right corner.
 */
const Vector& Rectangle::TopRight() const
{
  return top_.end_;
}

/**
 * Get the bottom left corner of the rectangle.
 * @param none
 * @return const Vector& the bottom left corner.
 */
const Vector& Rectangle::BottomLeft() const
{
  return bottom_.begin_;
}

/**
 * Get the bottom right corner of the rectangle.
 * @param none
 * @return const Vector& the bottom right corner.
 */
const Vector& Rectangle::BottomRight() const
{
  return right_.end_;
}

double DotProduct( const Vector& v1, const Vector& v2 )
{
  return (v1.x_ * v2.x_) + (v1.y_ * v2.y_);
}

double BytesToMegabytes(double bytes)
{
  return bytes / (1024. * 1024.);
}

} //  math
} //  myodd