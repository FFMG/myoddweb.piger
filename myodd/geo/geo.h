// MagVar.h: interface for the CMagVar class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _MAGVAR_H
#define _MAGVAR_H

#include "../common/includes.h"
#include <wtypes.h>

#ifdef _DEBUG  //  http://en.wikipedia.org/wiki/Longitude#Noting_and_calculating_longitude

#include <assert.h>
template<typename T>
void ASSERT_LAT( T lat )	
{
  assert( lat >= -90 && lat <= 90 );
};   //

template<typename T>
void ASSERT_LON( T lon )	
{
  assert( lon >= -180 && lon <= 180 );
};
#else
template<typename T>
void ASSERT_LAT( T lat )
{
  void();
}
template<typename T>
void ASSERT_LON( T lon )
{
  void();
}
#endif

namespace myodd{ namespace geo{
  static const double geo_pi = 3.1415926535897932384626433832795;

  void Test();  //  test data

  double radians_magvar( double lat, double lon, double h, unsigned long int dat=0, double* field=0 );
  double degree_magvar( double lat, double lon, double h, unsigned long int dat=0, double* field=0 );

  unsigned long int yymmdd_to_julian_days(int yy,int mm,int dd);
  unsigned long int today_to_julian_days();

  double numberOfDays( const SYSTEMTIME& st1, const SYSTEMTIME& st2 );

  void decimalToDegree( double n, int &degree, int &minute, double &second);

  double positionToDecimal(const MYODD_CHAR* lpPos );

  // --------------------
  // conversions.
  // --------------------

  /**
   * Convert a feet number to a NM value.
   * @param template<typename T> the feet number.
   * @return template<typename T> the NM value.
   */
  template<typename T>
  T feetToNm( const T& d )
  {
    static const double f2m = (0.000164578834);
    return static_cast<T>(d*f2m);
  }

  /**
   * Convert a feet number to a NM value.
   * @param template<typename T> the feet number.
   * @return template<typename T> the NM value.
   */
  template<typename T>
  T metersToNm( const T& d )
  {
    return feetToNm( metresToFeet(d) );
  }

  /**
   * Convert a NM number to a feet value.
   * @param template<typename T> the NM number.
   * @return template<typename T> the Feet value.
   */
  template<typename T>
  T nmToFeet( const T& d )
  {
    static const double f2m =	(0.000164578834);
    return static_cast<T>(d/f2m);
  }

  /**
   * Convert a meter number to a feet value.
   * @param template<typename T> the metre number.
   * @return template<typename T> the feet value.
   */
  template<typename T>
  T metresToFeet( const T& d )
  {
    static const double m2f	  =	(3.2808399);
    return static_cast<T>(d*m2f);
  }

  /**
  * Convert a feet value to a meter value.
  * @param template<typename T> the feet number.
  * @return template<typename T> the meter value.
  */
  template<typename T>
  T feetToMeters( const T& d )
  {
    static const double f2m	  =	(0.3048);
    return static_cast<T>(d*f2m);
  }

  /**
   * Convert a radian number to a degree value.
   * @param template<typename T> the degree number.
   * @return template<typename T> the degree value.
   */
  template<typename T>
  T radianToDegree( const T& radian )
  {
    static const double radian_to_degree = (180.0/geo_pi);
	  return static_cast<T>(radian_to_degree*radian);
  }

  /**
   * Convert a degree number to a radian value.
   * @param template<typename T> the radian number.
   * @return template<typename T> the radian value.
   */
  template<typename T>
  T degreeToRadian( const T& degrees )
  {
    static const double degree_to_radian = (geo_pi/180.0);
	  return static_cast<T>(degree_to_radian*degrees);
  }

  /**
   * -ve turn left  }
   * +ve turn right } the number of given degrees.
   * Return the degree correction to a target heading.
   * @param T the current heading.
   * @param T the target heading.
   * @return T the difference between the two heading.
   */
  template<typename T>
  T HeadingToPoint( T currentHeading, T targetHeading )
  {
    // validate the headings.
    currentHeading = ValidateHdg( currentHeading );
    targetHeading = ValidateHdg( targetHeading );

    T diff = (targetHeading - currentHeading);
    T validatedDiff = ValidateHdg( diff );
    if( validatedDiff > 180 )
    {
      diff = (validatedDiff - static_cast<T>(360));
    }
    else if( diff < -180 )
    {
      diff = (diff + static_cast<T>(360));
    }
    return diff;
  }

  /**
   * Make sure that the heading falls within the 0/360 degree range.
   * @param T& double/float/int the given heading.
   * @return T the corrected value.
   */
  template<typename T>
  T ValidateHdg( T hdg )
  {
    //	we limit the number of looks to prevent an infinite error 
    while ( hdg > 360 )
    {
      hdg-= static_cast<T>(360);
    }
    while ( hdg <= 0 )
    {
      hdg+= static_cast<T>(360);
    }
    _ASSERT( hdg > 0.f && hdg <=360.f );
    return hdg;
  }

  double  distanceNmToRadians( double nms );
  double  distanceRadianToNm( double radians );

  bool Intercept( double& lat3, double& lon3, double lat1, double lon1, double crs13,double lat2, double lon2, double crs23 );

  enum PosIntercept {
    PosIntercept_Start,
    PosIntercept_Line,
    PosIntercept_End,
  };
  
  PosIntercept ShortestIntercept( double& lat3, double& lon3, double lat11, double lon11, double lat12, double lon12, double lat, double lon );
  double DistanceToShortestIntercept( double lat11, double lon11, double lat12, double lon12, double lat, double lon, double* latNear=NULL, double* lonNear =NULL );

  // ----------------------------------------------------------------------
  //	get the true course given two points
  double	TrueCourse( double lat1, double lon1, double lat2, double lon2 );

  double nmDistance	( double lat1_indegree, double lon1_indegree, double lat2_indegree, double lon2_indegree );

  // ----------------------------------------------------------------------
  void GetEnroute( double& enRouteLat, double& enRouteLon, double deg, double distancefrom, double lat, double lon);

  // Check if a point is on a line or not.
  PosIntercept PosOnLine( double lat, double lon, double lat1, double lon1, double lat2, double lon2 );

  // ----------------------------------------------------------------------
  void MakeValidLat( double &lat );

  // ----------------------------------------------------------------------
  void MakeValidLon( double &lon );

} //  geo
} //  myodd
#endif // _MAGVAR_H
