#pragma once

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <string>
#include <boost/lexical_cast.hpp>

#include <chrono>
#include <random>

#undef max
#undef min

template<typename T>
T RealRandomNumber()
{

  // construct a trivial random generator engine from a time-based seed:
  unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
  std::default_random_engine generator(seed);
  std::default_random_engine re(seed);

  T lower_bound = std::numeric_limits<T>::min();
  T upper_bound = std::numeric_limits<T>::max();
  std::uniform_real_distribution<T> unif(lower_bound, upper_bound);
  return unif(re);
}

template<typename T>
T IntRandomNumber( bool canBeZero = true )
{

  // construct a trivial random generator engine from a time-based seed:
  unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
  std::default_random_engine generator(seed);
  std::default_random_engine re(seed);

  T lower_bound = std::numeric_limits<T>::min();
  T upper_bound = std::numeric_limits<T>::max();
  
  std::uniform_int_distribution<T> unif(lower_bound, upper_bound);
  auto value =  unif(re);
  if (canBeZero == false && value == 0 )
  {
    return IntRandomNumber<T>(canBeZero);
  }
  return value;
}

template<typename T>
T CharRandom()
{

  // construct a trivial random generator engine from a time-based seed:
  unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
  std::default_random_engine generator(seed);
  std::default_random_engine re(seed);

  T lower_bound = std::numeric_limits<T>::min();
  T upper_bound = std::numeric_limits<T>::max();
  std::uniform_int_distribution<int> unif(lower_bound, upper_bound);
  return T(unif(re));
}

inline bool BoolRandomNumber()
{

  // construct a trivial random generator engine from a time-based seed:
  unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
  std::default_random_engine generator(seed);
  std::default_random_engine re(seed);

  std::uniform_int_distribution<int> unif(0, 1);
  return (unif(re) != 0);
}

inline std::wstring Uuid()
{
  return boost::lexical_cast<std::wstring>(boost::uuids::random_generator()());
}

enum ExpectedIpcDataType : char
{
  None = 0,
  Guid = 1,
  Int32 = 2,
  Int64 = 3,
  String = 4,
  StringAscii = 5,
};