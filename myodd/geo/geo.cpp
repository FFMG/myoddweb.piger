#include "stdafx.h"

#ifdef __AFXWIN_H__
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif // _DEBUG
#endif // __AFXWIN_H__

#include "geo.h"
#include "../math/math.h"
#include "../string/string.h"

#include <time.h>

namespace myodd{ namespace geo{

void Test()
#ifdef _DEBUG
{
  double d1, d2, d3;
  d1 = positionToDecimal( _T("N05º 13' 01.3" ));
  d2 = positionToDecimal( _T("05º 13' 01.3N" ));
  assert( d1 == d2 );

  d3 = positionToDecimal( _T("N05* 13.02166666666668"));
  assert( d2 == d3 );

  d1 = positionToDecimal( _T("S29° 59&apos;13.60&quot;" )); //  XML format
  d2 = positionToDecimal( _T("S29:59'13.60\"" ));
  assert( d1 == d2 );

  d1 = positionToDecimal( _T("E30° 56&apos;58.10&quot;" ));
  d2 = positionToDecimal( _T("E30° 56'58.10\"" ));
  assert( d1 == d2 );

  int _d1, _m1, _d2, _m2;
  double _s1, _s2;
  decimalToDegree( d1, _d1, _m1, _s1 );
  decimalToDegree( d2, _d2, _m2, _s2 );

  assert( _d1 == _d2 && _d1 == 30);
  assert( _m1 == _m2 && _m1 == 56 );
  assert( _s1 == _s2 && (_s1 > 58.1 && _s1 < 58.2 ));

  double a = myodd::math::round( radianToDegree( 1.f ), 5);
  double b = myodd::math::round(57.2957795, 5);
  assert( abs(a - b ) < 0.005 );

  a = myodd::math::round(degreeToRadian( 1.f ), 5);
  b = myodd::math::round(0.0174532925, 5);
  assert( abs(a - b ) < 0.005 );
}
#else
{
  __noop;
}
#endif

static const double a = 6378.137;       /* semi-major axis (equatorial radius) of WGS84 ellipsoid */
static const double b = 6356.7523142;   /* semi-minor axis referenced to the WGS84 ellipsoid */
static const double r_0 = 6371.2;	/* standard Earth magnetic reference radius  */

static double gnm_wmm2005[13][13] =
{
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {-29556.8, -1671.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {-2340.6, 3046.9, 1657.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {1335.4, -2305.1, 1246.7, 674.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {919.8, 798.1, 211.3, -379.4, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {-227.4, 354.6, 208.7, -136.5, -168.3, -14.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {73.2, 69.7, 76.7, -151.2, -14.9, 14.6, -86.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {80.1, -74.5, -1.4, 38.5, 12.4, 9.5, 5.7, 1.8, 0.0, 0.0, 0.0, 0.0, 0.0},
    {24.9, 7.7, -11.6, -6.9, -18.2, 10.0, 9.2, -11.6, -5.2, 0.0, 0.0, 0.0, 0.0},
    {5.6, 9.9, 3.5, -7.0, 5.1, -10.8, -1.3, 8.8, -6.7, -9.1, 0.0, 0.0, 0.0},
    {-2.3, -6.3, 1.6, -2.6, 0.0, 3.1, 0.4, 2.1, 3.9, -0.1, -2.3, 0.0, 0.0},
    {2.8, -1.6, -1.7, 1.7, -0.1, 0.1, -0.7, 0.7, 1.8, 0.0, 1.1, 4.1, 0.0},
    {-2.4, -0.4, 0.2, 0.8, -0.3, 1.1, -0.5, 0.4, -0.3, -0.3, -0.1, -0.3, -0.1},
};

static double hnm_wmm2005[13][13]=
{
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 5079.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, -2594.7, -516.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, -199.9, 269.3, -524.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 281.5, -226.0, 145.8, -304.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 42.4, 179.8, -123.0, -19.5, 103.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, -20.3, 54.7, 63.6, -63.4, -0.1, 50.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, -61.5, -22.4, 7.2, 25.4, 11.0, -26.4, -5.1, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 11.2, -21.0, 9.6, -19.8, 16.1, 7.7, -12.9, -0.2, 0.0, 0.0, 0.0, 0.0},
    {0.0, -20.1, 12.9, 12.6, -6.7, -8.1, 8.0, 2.9, -7.9, 6.0, 0.0, 0.0, 0.0},
    {0.0, 2.4, 0.2, 4.4, 4.8, -6.5, -1.1, -3.4, -0.8, -2.3, -7.9, 0.0, 0.0},
    {0.0, 0.3, 1.2, -0.8, -2.5, 0.9, -0.6, -2.7, -0.9, -1.3, -2.0, -1.2, 0.0},
    {0.0, -0.4, 0.3, 2.4, -2.6, 0.6, 0.3, 0.0, 0.0, 0.3, -0.9, -0.4, 0.8},
};

static double gtnm_wmm2005[13][13]=
{
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {8.0, 10.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {-15.1, -7.8, -0.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.4, -2.6, -1.2, -6.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {-2.5, 2.8, -7.0, 6.2, -3.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {-2.8, 0.7, -3.2, -1.1, 0.1, -0.8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {-0.7, 0.4, -0.3, 2.3, -2.1, -0.6, 1.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.2, -0.1, -0.3, 1.1, 0.6, 0.5, -0.4, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.1, 0.3, -0.4, 0.3, -0.3, 0.2, 0.4, -0.7, 0.4, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
};

static double htnm_wmm2005[13][13]=
{
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, -20.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, -23.2, -14.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 5.0, -7.0, -0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 2.2, 1.6, 5.8, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 1.7, 2.1, 4.8, -1.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, -0.6, -1.9, -0.4, -0.5, -0.3, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.6, 0.4, 0.2, 0.3, -0.8, -0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, -0.2, 0.1, 0.3, 0.4, 0.1, -0.2, 0.4, 0.4, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
};

static const int nmax = 12;

static double P[13][13];
static double DP[13][13];
static double gnm[13][13];
static double hnm[13][13];
static double sm[13];
static double cm[13];

static double root[13];
static double roots[13][13][2];

/**
 * return variation (in degrees) given geodetic latitude (degree),
 * longitude(degree), height (km) and (Julian) date
 * N and E lat and long are positive, S and W negative
 *
 * @param double lat in degrees 
 * @param double lon in degrees 
 * @param double h in KMs
 * @param unsigned long int Julian date
 * @param double* field NULL or double[6] , (Output Bx (N) By (E) Bz (down) (in nTesla) dip (degrees down positive) and variation (degrees E positive))
 * @return double the mag var in degree. 
 */
double degree_magvar
( 
  double lat, 
  double lon, 
  double h, 
  unsigned long int dat, 
  double* field 
)
{
  return radianToDegree(  
            radians_magvar( degreeToRadian( lat ),
                            degreeToRadian( lon ),
                            h,
                            dat,
                            field
                           )
                        );
}

/*
 * return variation (in radians) given geodetic latitude (radians),
 * longitude(radians), height (km) and (Julian) date
 * N and E lat and long are positive, S and W negative
 * 
 * @param double lat in radians
 * @param double lon in radians
 * @param double h in KM, 
 * @param unsigned long int dat Julian date 
 * @param double* field NULL or double[6] , (Output Bx (N) By (E) Bz (down) (in nTesla) dip (degrees down positive) and variation (degrees E positive))
 * @return double the mag var in degree. 
 */
double radians_magvar
( 
  double lat_radian, 
  double lon_radian, 
  double h, 
  unsigned long int dat, 
  double* field 
)
{
  if( 0 == dat )
  {
    dat = today_to_julian_days( );
  }
  /* output field B_r,B_th,B_phi,B_x,B_y,B_z */
  int n,m;
  /* reference date for current model is 1 januari 2005 */
  static long date0_wmm2005 = yymmdd_to_julian_days(5,1,1);

  double yearfrac,sr,r,theta,c,s,psi,fn,fn_0,B_r,B_theta,B_phi,X,Y,Z;
  double sinpsi, cospsi, inv_s;

  static int been_here = 0;

  double sinlat = sin(lat_radian);
  double coslat = cos(lat_radian);

  /* convert to geocentric coords: */
  // sr = sqrt(pow(a*coslat,2.0)+pow(b*sinlat,2.0));
  sr = sqrt(a*a*coslat*coslat + b*b*sinlat*sinlat);
  /* sr is effective radius */
  theta = atan2(coslat * (h*sr + a*a),
    sinlat * (h*sr + b*b));
  /* theta is geocentric co-latitude */

  r = h*h + 2.0*h * sr +
    (a*a*a*a - ( a*a*a*a - b*b*b*b ) * sinlat*sinlat ) / 
    (a*a - (a*a - b*b) * sinlat*sinlat );

  r = sqrt(r);

  /* r is geocentric radial distance */
  c = cos(theta);
  s = sin(theta);
  /* protect against zero divide at geographic poles */
  inv_s =  1.0 / (s + (s == 0.)*1.0e-8); 

  /* zero out arrays */
  for ( n = 0; n <= nmax; n++ ) {
    for ( m = 0; m <= n; m++ ) {
      P[n][m] = 0;
      DP[n][m] = 0;
    }
  }

  /* diagonal elements */
  P[0][0] = 1;
  P[1][1] = s;
  DP[0][0] = 0;
  DP[1][1] = c;
  P[1][0] = c ;
  DP[1][0] = -s;

  // these values will not change for subsequent function calls
  if( !been_here ) {
    for ( n = 2; n <= nmax; n++ ) {
      root[n] = sqrt((2.*n-1) / (2.*n));
    }

    for ( m = 0; m <= nmax; m++ ) {
      double mm = m*m;
      for ( n = myodd::math::max2(m + 1, 2); n <= nmax; n++ ) {
        roots[m][n][0] = sqrt((n-1)*(n-1) - mm);
        roots[m][n][1] = 1.0 / sqrt( n*n - mm);
      }
    }
    been_here = 1;
  }

  for ( n=2; n <= nmax; n++ ) {
    // double root = sqrt((2.0*n-1) / (2.0*n));
    P[n][n] = P[n-1][n-1] * s * root[n];
    DP[n][n] = (DP[n-1][n-1] * s + P[n-1][n-1] * c) *
      root[n];
  }

  /* lower triangle */
  for ( m = 0; m <= nmax; m++ ) {
    // double mm = m*m;
    for ( n = myodd::math::max2(m + 1, 2); n <= nmax; n++ ) {
      // double root1 = sqrt((n-1)*(n-1) - mm);
      // double root2 = 1.0 / sqrt( n*n - mm);
      P[n][m] = (P[n-1][m] * c * (2.0*n-1) -
        P[n-2][m] * roots[m][n][0]) *
        roots[m][n][1];

      DP[n][m] = ((DP[n-1][m] * c - P[n-1][m] * s) *
        (2.0*n-1) - DP[n-2][m] * roots[m][n][0]) *
        roots[m][n][1];
    }
  }

  /* compute Gauss coefficients gnm and hnm of degree n and order m for the desired time
  achieved by adjusting the coefficients at time t0 for linear secular variation */
  /* WMM2005 */
  yearfrac = (dat - date0_wmm2005) / 365.25;
  for ( n = 1; n <= nmax; n++ ) {
    for ( m = 0; m <= nmax; m++ ) {
      gnm[n][m] = gnm_wmm2005[n][m] + yearfrac * gtnm_wmm2005[n][m];
      hnm[n][m] = hnm_wmm2005[n][m] + yearfrac * htnm_wmm2005[n][m];
    }
  }

  /* compute sm (sin(m lon) and cm (cos(m lon)) */
  for ( m = 0; m <= nmax; m++ ) {
    sm[m] = sin(m * lon_radian);
    cm[m] = cos(m * lon_radian);
  }

  /* compute B fields */
  B_r = 0.0;
  B_theta = 0.0;
  B_phi = 0.0;
  fn_0 = r_0/r;
  fn = fn_0 * fn_0;

  for ( n = 1; n <= nmax; n++ ) {
    double c1_n=0;
    double c2_n=0;
    double c3_n=0;
    for ( m = 0; m <= n; m++ ) {
      double tmp = (gnm[n][m] * cm[m] + hnm[n][m] * sm[m]); 
      c1_n=c1_n + tmp * P[n][m];
      c2_n=c2_n + tmp * DP[n][m];
      c3_n=c3_n + m * (gnm[n][m] * sm[m] - hnm[n][m] * cm[m]) * P[n][m];
    }
    // fn=pow(r_0/r,n+2.0);
    fn *= fn_0;
    B_r = B_r + (n + 1) * c1_n * fn;
    B_theta = B_theta - c2_n * fn;
    B_phi = B_phi + c3_n * fn * inv_s;
  }

  /* Find geodetic field components: */
  psi = theta - ((geo_pi / 2.0) - lat_radian);
  sinpsi = sin(psi);
  cospsi = cos(psi);
  X = -B_theta * cospsi - B_r * sinpsi;
  Y = B_phi;
  Z = B_theta * sinpsi - B_r * cospsi;

  if( field )
  {
    field[0]=B_r;
    field[1]=B_theta;
    field[2]=B_phi;
    field[3]=X;
    field[4]=Y;
    field[5]=Z;   /* output fields */
  }
  /* find variation in radians */
  /* return zero variation at magnetic pole X=Y=0. */
  /* E is positive */
  return (X != 0. || Y != 0.) ? atan2(Y, X) : (double) 0.;  
}

//  -------------------------------------------------------------------
/* Convert date to Julian day    1950-2049 */
unsigned long int yymmdd_to_julian_days(int yy,int mm,int dd)
{
  unsigned long jd;

  //  in case we are passed the full year we don't need to convert it.
  if( yy < 1000 )
  {
    yy = (yy < 50) ? (2000 + yy) : (1900 + yy);
  }
  assert( yy >=1900 && yy <= 2049 );
  
  jd = dd - 32075L + 1461L * (yy + 4800L + (mm - 14) / 12 ) / 4;
  jd = jd + 367L * (mm - 2 - (mm - 14) / 12*12) / 12;
  jd = jd - 3 * ((yy + 4900L + (mm - 14) / 12) / 100) / 4;

  return(jd);
}

/**
 * Get todays date to Julian.
 * @return unsigned long int the current number of days in the julian calendar
 */
unsigned long int today_to_julian_days()
{
  // the main problem with this function
  // is in case the user is running the application for more than one day
  // but realisticaly the diferences will be minimal even over a few days or months.
  static unsigned long int uliToday = 0;
  if( 0 == uliToday )
  {
    __time64_t long_time;
    _time64( &long_time ); 

    struct tm today;
    _localtime64_s( &today, &long_time );
    uliToday = (yymmdd_to_julian_days( 1900 + today.tm_year, 
                                          1 + today.tm_mon, 
                                              today.tm_mday 
                                     ));
  }
  return uliToday;
}

/**
 * Convert a decimal number to the equivalent degree/minutes/seconds.
 * @param double the number we are converting.
 * @param int& the degrees.
 * @param int& the minutes.
 * @param double& the seconds.
 */
void decimalToDegree
( 
  double n, 
  int &degree, 
  int &minute, 
  double &second
)
{
  degree	=	myodd::math::trunc( n );
	minute	=	myodd::math::trunc((n - degree) * 60);

	second = (((n - degree) * 60) - minute)*60;

	while( second >= 60 ){
		second-=60;
		minute+=1;
	}
	while( minute >= 60 ){
		minute-=60;
		degree+=1;
	}
}

/**
 * Convert a string to a Decimal lat/lon 
 * @see http://williams.best.vwh.net/avform.htm
 * @see http://en.wikipedia.org/wiki/ISO_6709
 * @param const MYODD_CHAR* the string representing the lat/lon
 * @return double the converted latitude/longitude.
 */
double positionToDecimal(const MYODD_CHAR* lpString )
{
  std::wstring sPos( lpString );
  sPos = myodd::strings::Replace( sPos, _T("&apos;"), _T("'") );
  sPos = myodd::strings::Replace( sPos, _T("&quot;"), _T("\"") );

  const MYODD_CHAR* lpPos = sPos.c_str();

  // 05º 13' 01.3" S = -(5 + 13/60 + 1/3600)
  // S05º 13' 01.3"
  double dPos = 0;

  // positive or negative number.
  short direction = 1;
  
  std::vector<double> nums;
  size_t l = _tcslen(lpPos );
  // go around looking for all the numbers
  // normally the string will have a format with a bunch of numbers separated
  // by other characters.
  //
  // N51° 43' 21.80"
  //
  for( size_t s = 0; s < l; ++s )
  {
    int n = 0;
    for(;;)
    {
      // look for any number
      // anything else will be assumed to be a 'divider'
      const MYODD_CHAR c = lpPos[ s+n ];
      if( (s+n)<l && ((c >= '0' && c <= '9') || c=='.' || c==',' )){
        ++n;
        continue;
      }

      if( c == 'S' || c == 's' || c == 'W' || c == 'w' )
        direction = -1;     // -ve
      
      // first number could -ve
      if( c=='-' && nums.size() == 0 )
        direction = -1;     // -ve
      break;
    }

    // spaces and breaks do not count
    if( n>0 )
    {
      MYODD_CHAR* t = new MYODD_CHAR[ n+1];
      _tcsncpy_s( t, n+1, lpPos+s, n);
      // by definition all the numbers are +ve
      // because we skip any non numeric characters.
      nums.push_back( _tstof( t ) );
      delete [] t;
      s+=(n);
    }
  }

  // with the numbers we can not calculate the position
  if( nums.size() == 3 )
  {
    dPos = nums[0] + (nums[1]/60.f) + (nums[2]/3600.f);
  }
  else if( nums.size() == 2 )
  {
    dPos = nums[0] + ((nums[1] ) / 60);  //  no seconds given.
  }
  else if( nums.size() == 1 )
  {
    dPos = nums[0];                 //  the user passed a number...
  }
  else
  {
    _ASSERT(0);  //  not sure what to do with it.
    return 0;
  }

  // and the direction
  dPos *= direction;

  return dPos;
}

/**
 * Get the number of days between 2 times.
 * @param const SYSTEMTIME& the first day we are comparing.
 * @param const SYSTEMTIME& the other day.
 * @return double the exact number of days between 2 times.
 */
double numberOfDays( const SYSTEMTIME& st1, const SYSTEMTIME& st2 )
{
  union timeunion {
    FILETIME fileTime;
    ULARGE_INTEGER ul;
  } ;

  timeunion ft1;
  timeunion ft2;

  SystemTimeToFileTime(&st1, &ft1.fileTime);
  SystemTimeToFileTime(&st2, &ft2.fileTime);

  if( ft1.ul.QuadPart > ft2.ul.QuadPart )
  {
    myodd::math::swap( ft1, ft2 );
  }
  __int64 i = (ft2.ul.QuadPart - ft1.ul.QuadPart);
  double days = i / 10000000.f  /*seconds*/
                   / 60.f       /* min */
                   / 60.f       /* hrs */
                   / 24.f       /* days */
                   ;
  return days;
}

/**
 * Convert a given nautical miles distance to radians
 * @param double the distance in nm
 * @return double the distance in radians
 */
double  distanceNmToRadians( double nms )
{
  return (geo_pi/(180.0*60.0))*nms;
}

/**
 * Convert a given radian to nautical miles distance
 * @param double the distance in radians
 * @return double the distance in nautical miles
 */
double  distanceRadianToNm( double radians )
{
  return ((180.0*60.0)/geo_pi)*radians;
}

/**
 * Get the intercept of two lines.
 * The two lines start from a single point each and have a heading
 * @see http://williams.best.vwh.net/avform.htm
 * NB:	This formula assumes that W lat is -ve
 * ~~							                E lat is +ve
 *                                N lon is +ve
 *                                S lon is -ve
 * @param double& the latitude where the lines intercept
 * @param double& the longitude where the lines intercept
 * @param double lat of the first point
 * @param double lon of the first point
 * @param double course of the first point
 * @param double lat of the second point
 * @param double lon of the second point
 * @param double course of the second point
 * @return bool if the lines intercept at all.
 */
bool Intercept( double& lat3, double& lon3, double lat1, double lon1, double crs13,double lat2, double lon2, double crs23 )
{
  lon1 = -1 * lon1;
  lon2 = -1 * lon2;

  crs13 = degreeToRadian(crs13);
  crs23 = degreeToRadian(crs23);
  lat1 = degreeToRadian(lat1);
  lat2 = degreeToRadian(lat2);
  lon1 = degreeToRadian(lon1);
  lon2 = degreeToRadian(lon2);

  double lat12 = lat1-lat2;
  double lon12 = lon1-lon2;
  double sin_lat12 = pow( sin(lat12/2), double(2));
  double sin_lon12 = pow( sin(lon12/2), double(2));
  double dst12=2.*asin(sqrt(sin_lat12+ cos(lat1)*cos(lat2)*sin_lon12));
  double crs12=0;
  double crs21=0;


  if( myodd::math::AreAlmostSame(lon2-lon1, 0.) )
  {
    lat3 = radianToDegree(lat1);
    lon3 = -1 * radianToDegree(lon1);
    return true;
  }

  double sin_dst12 = sin(dst12);
  double cos_lat1 = cos(lat1);
  double cos_lat2 = cos(lat2);
  if( sin(lon2-lon1)<0 )
  {
    crs12=acos((sin(lat2)-sin(lat1)*cos(dst12))/(sin_dst12*cos_lat1));
    crs21=2.*geo_pi-acos((sin(lat1)-sin(lat2)*cos(dst12))/(sin(dst12)*cos_lat2));
  }
  else
  {
    crs12=2.f*geo_pi-acos((sin(lat2)-sin(lat1)*cos(dst12))/(sin_dst12*cos_lat1));
    crs21=acos((sin(lat1)-sin(lat2)*cos(dst12))/(sin_dst12*cos_lat2));
  }

  double ang1=myodd::math::modulo(crs13-crs12+geo_pi,2.*geo_pi)-geo_pi;
  double ang2=myodd::math::modulo(crs21-crs23+geo_pi,2.*geo_pi)-geo_pi;

  if(sin(ang1)==0 && sin(ang2)==0)
  {
    //  "infinity of intersections"
    return false;
  }
  else if( sin(ang1)*sin(ang2)<0 )
  {
    //  "intersection ambiguous"
    return false;
  }

  ang1=abs(ang1);
  ang2=abs(ang2);
    
  double ang3=acos(-cos(ang1)*cos(ang2)+sin(ang1)*sin(ang2)*cos(dst12)) ;
  double dst13=atan2(sin(dst12)*sin(ang1)*sin(ang2),cos(ang2)+cos(ang1)*cos(ang3));
  lat3=asin(sin(lat1)*cos(dst13)+cos(lat1)*sin(dst13)*cos(crs13));
  double dlon=atan2(sin(crs13)*sin(dst13)*cos(lat1),cos(dst13)-sin(lat1)*sin(lat3));
  lon3=myodd::math::modulo(lon1-dlon + geo_pi,2.*geo_pi)-geo_pi;
  
  lat3 = radianToDegree(lat3);
  lon3 = -1 * radianToDegree(lon3);
  return true;
}

/**
 * Get the shortest distance to a line from a position not on the line itself.
 * @see http://williams.best.vwh.net/avform.htm
 * NB:	This formula assumes that W lat is -ve
 * ~~							                E lat is +ve
 *                                N lon is +ve
 *                                S lon is -ve
 * @param double the start lat of the line
 * @param double the start lon of the line
 * @param double the end lat of the line
 * @param double the end lon of the line
 * @param double the latitude where we are compared to the line
 * @param double the longitude where we are compared to the line. 
 * @param double* if the value us not NULL we will set the latitude of the nearest intercept on the line.
 * @param double* if the value us not NULL we will set the longitude of the nearest intercept on the line.
 * @return double the shortest distance to the line.
 */
double DistanceToShortestIntercept
( 
  double lat11, 
  double lon11, 
  double lat12, 
  double lon12, 
  double lat, 
  double lon,
  double* latNear/* = NULL*/, 
  double* lonNear/* = NULL*/
)
{
  double latNearLoc, lonNearLoc;
  myodd::geo::ShortestIntercept(latNearLoc,
                                lonNearLoc,
                                lat11,
                                lon11, 
                                lat12,
                                lon12,
                                lat,
                                lon
                                );
  if( latNear )
  {
    *latNear = latNearLoc;
  }
  if( lonNear )
  {
    *lonNear = lonNearLoc;
  }
  return myodd::geo::nmDistance( latNearLoc, lonNearLoc, lat, lon );
}

/**
 * Get the shortest lat/lon on a line from a position not on the line itself.
 * @see http://williams.best.vwh.net/avform.htm
 * NB:	This formula assumes that W lat is -ve
 * ~~							                E lat is +ve
 *                                N lon is +ve
 *                                S lon is -ve
 * @param double& the return latitude.
 * @param double& the return longitude
 * @param double the start lat of the line
 * @param double the start lon of the line
 * @param double the end lat of the line
 * @param double the end lon of the line
 * @param double the latitude where we are compared to the line
 * @param double the longitude where we are compared to the line. 
 * @return PosIntercept the intercept value.
 */
PosIntercept ShortestIntercept
( 
  double& lat3, 
  double& lon3, 
  double lat11, 
  double lon11, 
  double lat12, 
  double lon12, 
  double lat, 
  double lon 
)
{
  // get the heading
  double lineHeading = TrueCourse( lat11, lon11, lat12, lon12 );

  // get the length of the line
  double lineDistance = nmDistance( lat11, lon11, lat12, lon12 );

  double lineDistanceToStart = nmDistance( lat, lon, lat11, lon11 );
  double lineDistanceToEnd = nmDistance( lat, lon, lat12, lon12 );

  // get the intercept 90 degrees from our point to the line
  if( !Intercept( lat3, lon3, lat11, lon11, lineHeading, lat, lon, lineHeading+90 ))
  {
    if( !Intercept( lat3, lon3, lat11, lon11, lineHeading, lat, lon, lineHeading-90 ))
    {
      if( lineDistanceToStart < lineDistanceToEnd )
      {
        lat3 = lat11;
        lon3 = lon11;
        return PosIntercept_Start;
      }
      else
      {
        lat3 = lat12;
        lon3 = lon12;
        return PosIntercept_End;
      }
    }
  }

  // we now have lat3/lon3 that is on the line lat11/lon11 > lat12/lon12
  // if the distance to either the start or end point is greater than the total 
  // length of the line, then we know we are not _on_ the line
  double lineDistancePointToStart  = nmDistance( lat3, lon3, lat11, lon11 );
  double lineDistancePointToEnd    = nmDistance( lat3, lon3, lat12, lon12 );
  
  // if the distance is shorter then we are going at a right angle.
  if( lineDistancePointToEnd > lineDistance )
  {
    // the point(P) is on the line (AB)
    // -----P----A---------B-------
    // but the distance P > B is greater than A > B
    // so we must be closer to the start
    lat3 = lat11;
    lon3 = lon11;
    return PosIntercept_Start;
  }
  else if( lineDistancePointToStart > lineDistance )
  {
    // the point(P) is on the line (AB)
    // ----------A---------B----P--
    // but the distance P > A is greater than A > B
    // so we must be closer to the end
    lat3 = lat12;
    lon3 = lon12;
    return PosIntercept_End;
  }

  // the point P is on the line, so the distance
  // P > A and P > B is less than the distance A > B
  // nothing to change.
  // ----------A---------B----P--
  return PosIntercept_Line;
}

/**
 * Check if a point is on a line or not.
 * @param double the latitude of the point
 * @param double the longitude of the point
 * @param double the start lat position of the line
 * @param double the start lon position of the line
 * @param double the end lat position of the line
 * @param double the end lon position of the line
 * @return PosIntercept where the point intercept the line, of not, then the closest point.
 */
PosIntercept PosOnLine
( 
  double lat, 
  double lon, 
  double lat1, 
  double lon1, 
  double lat2, 
  double lon2 
)
{
  // get the length of the line
  double lineDistance = nmDistance( lat1, lon1, lat2, lon2 );

  // we now have lat3/lon3 that is on the line lat11/lon11 > lat12/lon12
  // if the distance to either the start or end point is greater than the total 
  // length of the line, then we know we are not _on_ the line
  double lineDistancePointToStart  = nmDistance( lat, lon, lat1, lon1 );
  double lineDistancePointToEnd    = nmDistance( lat, lon, lat2, lon2 );

  // if the distance is shorter then we are going at a right angle.
  if( lineDistancePointToEnd > lineDistance )
  {
    // the point(P) is on the line (AB)
    // -----P----A---------B-------
    // but the distance P > B is greater than A > B
    // so we must be closer to the start
    return PosIntercept_Start;
  }
  else if( lineDistancePointToStart > lineDistance )
  {
    // the point(P) is on the line (AB)
    // ----------A---------B----P--
    // but the distance P > A is greater than A > B
    // so we must be closer to the end
    return PosIntercept_End;
  }

  // the point P is on the line, so the distance
  // P > A and P > B is less than the distance A > B
  // nothing to change.
  // ----------A---------B----P--
  return PosIntercept_Line;
}

/**
 * The true course heading between point 1 and point 2
 * @see http://williams.best.vwh.net/avform.htm
 * NB:	This formula assumes that W lat is -ve
 * ~~							                E lat is +ve
 *                                N lon is +ve
 *                                S lon is -ve
 * @param double lat of the point we are heading from
 * @param double lon of the point we are heading from
 * @param double lat of the point we are heading to
 * @param double lon of the point we are heading to
 * @return double the true course heading.
 */

double TrueCourse
( 
  double lat1, 
  double lon1, 
  double lat2, 
  double lon2 
)
{
  ASSERT_LAT( lat1);
  ASSERT_LAT( lat2);

  ASSERT_LON( lon1);
  ASSERT_LON( lon2);

  lon1 = -1 * lon1;
  lon2 = -1 * lon2;

  lat1 = degreeToRadian(lat1);
  lat2 = degreeToRadian(lat2);
  lon1 = degreeToRadian(lon1);
  lon2 = degreeToRadian(lon2);

  double tc	=	0;
  //tc1=mod(atan2(sin(lon1-lon2)*cos(lat2),cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon1-lon2)), 2*pi)
  tc = myodd::math::modulo(atan2(sin(lon1-lon2)*cos(lat2),cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon1-lon2)), 2.*geo_pi);

  //	return the value in degrees
  tc = radianToDegree( tc );
  //	done
  return tc;
}

/**
 * Get the distance between two lat/lon points.
 * @see http://williams.best.vwh.net/avform.htm
 * NB:	This formula assumes that W lat is -ve
 * ~~							                E lat is +ve
 *                                N lon is +ve
 *                                S lon is -ve
 * @param double the lat of point1
 * @param double the lon of point1
 * @param double the lat of point2
 * @param double the lon of point2
 * @return double the distance in nm between the two points.
 */
double nmDistance
( 
  double lat1_indegree, 
  double lon1_indegree, 
  double lat2_indegree, 
  double lon2_indegree 
)
{
  // sanity check
  if( lat1_indegree == lat2_indegree && lon1_indegree == lon2_indegree )
  {
    return 0;
  }
  ASSERT_LAT( lat1_indegree);
  ASSERT_LAT( lat2_indegree);

  ASSERT_LON( lon1_indegree);
  ASSERT_LON( lon2_indegree);

  //	lat
  double lat1_inradian = degreeToRadian(lat1_indegree);
  double lat2_inradian = degreeToRadian(lat2_indegree);

  //	lon
  double lon1_inradian = degreeToRadian(lon1_indegree);
  double lon2_inradian = degreeToRadian(lon2_indegree);

  double nm;
  //  d  = acos ( sin( lat1)* sin( lat2) + cos( lat1) * cos( lat2) * cos( lon1 - lon2))
  nm = acos ( sin( lat1_inradian)* sin( lat2_inradian) + cos( lat1_inradian) * cos( lat2_inradian) * cos( lon1_inradian - lon2_inradian));
  //	convert back to NM
  nm = distanceRadianToNm( nm);
  //	done
  return nm;
}

/**
 * Get the lat/lon en route given a starting lat/lon and a heading
 * @see http://williams.best.vwh.net/avform.htm
 * NB:	This formula assumes that W lat is -ve
 * ~~							                E lat is +ve
 *                                N lon is +ve
 *                                S lon is -ve
 * It is very important for this formula!!
 * @param double& the return lat
 * @param double& the return long
 * @param double the heading degree
 * @param double the distance from the given lat/lon
 * @param double the latitude we are going from
 * @param double the longitude we are going from
 * @return none
 */
void GetEnroute( double& enRouteLat, double& enRouteLon, double deg, double distancefrom, double lat, double lon)
{
	ASSERT_LON( lon );
	ASSERT_LAT( lat );

	//	zero distance is useless
	if ( distancefrom == 0)
  {
    enRouteLat = lat;
    enRouteLon = lon;
		return;
  }

	lon *= -1.f;
	//	convert to radian the lat and lon
	double startlat = degreeToRadian( lat );
	double startlon = degreeToRadian( lon );

	//	convert the distance to radian
	double distanceradian = distanceNmToRadians( distancefrom );
	
	//	convert the angle from deg to radian
	double radian = degreeToRadian(deg);
  //	double degree = radianToDegree(radian);
	
	lat	= asin(sin( startlat )*cos(distanceradian)+cos(startlat )*sin(distanceradian)*cos(radian));
//lat = asin(sin( lat1     )*cos(d             )+cos(lat1     )*sin(d             )*cos(tc    ))
  
	double dlon=atan2(sin(radian)*sin(distanceradian)*cos(startlat),cos(distanceradian)-sin(startlat)*sin(lat));

	//	   dlon=atan2(sin(tc    )*sin(d			        )*cos(lat1	  ),cos(d			        )-sin(lat1    )*sin(lat))
	lon= myodd::math::modulo(startlon - dlon +geo_pi, 2.*geo_pi)-geo_pi;
//lon=mod(lon1	   - dlon +pi,2*pi)-pi

  //lon=mod(startlon - asin( sin( radian)*sin( distanceradian)/cos(startlat))+PI, PI_2)-PI;
  //lon=mod(lon1     - asin( sin( tc    )*sin( d             )/cos(lat     ))+pi, 2*pi)-pi

//	convert the lat to degree
	lat	= radianToDegree( lat);

//	convert the lon to degree
	lon = radianToDegree( lon);
	lon *= -1.f;

  enRouteLat = lat;
  enRouteLon = lon;
}

/**
 * Make sure that the given value is a valid latitude.
 * @param double the possibly invalid latitude.
 * @return none
 */
void MakeValidLat( double &lat )
{
  lat=(lat>90?90:lat);
  lat=(lat<-90?-90:lat);
}

/**
 * Make sure that the given value is a valid longitude.
 * @param double the possibly invalid longitude.
 * @return none
 */
void MakeValidLon( double &lon )
{
  lon=(lon>180?180:lon);
  lon=(lon<-180?-180:lon);
}

} //  geo
} //  myodd