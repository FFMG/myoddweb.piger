#pragma once

#include "../math/math.h"

namespace myodd{ namespace matrix{

template<class T>
class map2D
{
protected:
  typedef std::map<__int64,T> typedef2DMap;
  typedef2DMap _data;

public:
  map2D(void);
  virtual ~map2D(void);

  typedef typename typedef2DMap::iterator iterator;
  typedef typename typedef2DMap::const_iterator const_iterator;

  const T* GetAt( int x, int y ) const;
  void Clear();
  void Add( int x, int y, T& val );
};


template<class T>
map2D<T>::map2D(void)
{
}

template<class T>
map2D<T>::~map2D(void)
{
}

template<class T>
const T* map2D<T>::GetAt( int x, int y ) const
{
  __int64 idx = myodd::math::MAKEI( x, y );
  const_iterator it = _data.find( idx );
  if( it == _data.end() )
    return NULL;

  return &(it->second);
}

/**
* Add a T value in the array.
* @param 
* @return 
*/
template<class T>
void map2D<T>::Add( int x, int y, T& val )
{
  __int64 idx = myodd::math::MAKEI( x, y );
  _data[idx] = val;
}

/**
* Clear the buffer.
* @param 
* @return 
*/
template<class T>
void map2D<T>::Clear()
{
  _data.clear();
}

} //  matrix
} //  myodd
