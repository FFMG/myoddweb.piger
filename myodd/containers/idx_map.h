// log.h: interface for the Offset class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#ifndef _Idx_map_h
#define _Idx_map_h

namespace myodd{ 
#ifdef _DEBUG
  #define _MYODD_IDX_MAP_INRANGE	\
    assert( _mapVector.size() == __super::size() );
#else
  #define _MYODD_IDX_MAP_INRANGE	__noop;
#endif 
  template
    <class _Kty,
     class _Ty >
  class idx_map : private std::map< _Kty, _Ty >
  {
  public:
    /**
     * constructor.
     */
    idx_map( )
    {
    }

    /**
     * destructor
     */
    virtual ~idx_map()
    {
    }

    /**
     * Test that all the functions are still running as expected.
     * @param none
     * @return none
     */
    static void Test()
#ifdef _DEBUG
    {
      myodd::idx_map< int, int > someTest;
      someTest[ 10 ] = 15;
      someTest[ 10 ] = 12;
      someTest[ 10 ] = 10;

      myodd::idx_map< int, int >::const_iterator it1 = someTest.at( 0 );
      myodd::idx_map< int, int >::iterator it2 = someTest.at( 0 );
      assert( it1->second == 10 );
      assert( it2->second == 10 );

      someTest[ 11 ] = 11;
      someTest[ 12 ] = 12;
      someTest[ 13 ] = 13;
      assert( someTest.size() == 4 );

      it1 = someTest.at( 0 );
      it2 = someTest.at( 0 );
      assert( it1->second == 10 );
      assert( it2->second == 10 );

      someTest[ 5 ] = 5;
      someTest[ 6 ] = 6;

      // reorder the data
      someTest.reorder();
      it1 = someTest.at( 0 );
      assert( it1->second == 5 ); //  make sure that the order is correct.
      it1 = someTest.at( 1 );
      assert( it1->second == 6 );

      someTest.erase_at( 0 );
      assert( someTest.at( 0 )->second != 5 );
      someTest.erase( someTest.begin() );
      someTest.erase( someTest.begin(), someTest.end() );
      assert( someTest.size() == 0 );
    }
#else
    {
      //  do nothing in release.
      __noop;
    }
#endif // _DEBUG

    typedef typename __super::iterator iterator;
    typedef typename __super::const_iterator const_iterator;

    /**
     * get the beginning iterator
     * @return iterator
     */
    iterator begin()
    {	
      _MYODD_IDX_MAP_INRANGE
      return __super::begin();
    }

    /**
     * get the beginning const_iterator
     * @return cont_iterator
     */
    const_iterator begin() const
    {	
      _MYODD_IDX_MAP_INRANGE
      return __super::begin();
    }

    /**
     * get the end iterator
     * @return iterator
     */
    iterator end()
    {	
      _MYODD_IDX_MAP_INRANGE
      return __super::end();
    }

    /**
     * get the end const_iterator
     * @return cont_iterator
     */
    const_iterator end() const
    {	
      _MYODD_IDX_MAP_INRANGE
      return __super::end();
    }

    /**
     * make sure that the index vector matches
     * the map data order.
     * @return none
     */
    void reorder()
    {
      // we don't need to remove anything as it is the same size.
      // if this assert fires then it will not be the same size and something will crash.
      _MYODD_IDX_MAP_INRANGE

      size_t pos = 0;
      for( iterator it = begin(); 
        it != end(); 
        ++it 
        )
      {
        _mapVector[ pos++] = it;
      }
      _MYODD_IDX_MAP_INRANGE
    }

    /**
     * Erase an item given a key type.
     * @param const _Kty& the keytype
     * @return size_type the number of matching items that where deleted.
     */
    size_type erase(const _Kty& _Keyval)
    {
      // erase and count all that match _Keyval
      size_type num = 0;
      while( true )
      {
        const_iterator _Where = find( _Keyval );
        if( _Where == end() )
          break;
        ++num;
        erase( _Where );
      }
      _MYODD_IDX_MAP_INRANGE
      return num;
    }

    /**
     * Erase an item given an iterator
     * @param const_iterator the iterator we are deleting from.
     * @return none.
     */
    void erase(const_iterator _Where)
    {
      //  we need to look for that guy in the the container as well.
      for( map_vector::iterator it = _mapVector.begin();
           it != _mapVector.end();
           ++it )
      {
        if( *it != _Where )
          continue;
        _mapVector.erase( it );
        break; // we are done.
      }
      __super::erase( _Where );
      _MYODD_IDX_MAP_INRANGE
    }

    /**
    * Erase one or more items between a range of iterators.
    * @param const_iterator the first iterator we are deleting.
    * @param const_iterator the last iterator we are deleting up to.
    * @return none.
    */
    void erase(const_iterator _First, const_iterator _Last)
    {
      //  save time and remove everything if we can, it is faster
      if( _First == begin() && _Last == end() )
      {
        clear();
      }
      else
      {
        for( const_iterator it = _First; it != _Last; ++it )
          __super::erase( it );
      }
      _MYODD_IDX_MAP_INRANGE
    }

    /**
     * Delete everything.
     * @return 
     */
    void clear()
    {
      //  remove everything, do not use the erase() 
      //  function as we will be deleting everything.
      _mapVector.erase( _mapVector.begin(), _mapVector.end() );
      __super::erase( __super::begin(), __super::end() );
      _MYODD_IDX_MAP_INRANGE
    }

    /**
     * Find a the first iterator that matches the keytype
     * @param const _Kty& the keytype we are looking for.
     * @return iterator the matching iterator, (or end() )
     */
    iterator find(const _Kty& _Keyval)
    {
      _MYODD_IDX_MAP_INRANGE;
      return __super::find( _Keyval );
    }

    /**
     * Find a the first iterator that matches the keytype
     * @param const _Kty& the keytype we are looking for.
     * @return const_iterator the matching iterator, (or end() )
     */
    const_iterator find(const _Kty& _Keyval) const
    {
      _MYODD_IDX_MAP_INRANGE
      return __super::find( _Keyval );
    }

    /**
     * Get the size of the container, (how many items we currently have).
     * @return size_type the size of the container.
     */
    size_type size() const
    {
      _MYODD_IDX_MAP_INRANGE
      return __super::size();
    }

    /**
     * Add/Update a value in the container.
     * We return the reference so the user can update the value then.
     * @param const _Kty& the key type we are adding/replacing.
     * @return _Ty& the second of the data.
     */
    _Ty& operator[](const _Kty& _Keyval)
    {
      key_compare comp = this->key_comp();

      iterator _Where = this->lower_bound(_Keyval);
      if(_Where == this->end() || comp(_Keyval, this->_Key(_Where._Mynode())))
      {
        _Where = this->insert(_Where, value_type(_Keyval, mapped_type()));
        _mapVector.push_back( _Where );
      }

      _MYODD_IDX_MAP_INRANGE
      return ((*_Where).second);
    }

    /**
     * erase at a certain index number.
     * @param unsigned int the location of the item we are erasing.
     * @return none
     */
    void erase_at( unsigned int _idx )
    {
      const_iterator it = at( _idx );
      erase( it );
    }

    /**
     * Get an const_iteratror at a certain index number.
     * we return end() if the item does not exist.
     * @param unsigned int the location of the item we are erasing.
     * @return const_iterator the item we are looking for.
     */
    const_iterator at( unsigned int _idx ) const
    {
      return const_cast<idx_map*>(this)->at( _idx );
    }

    /**
    * Get an const_iteratror at a certain index number.
    * we return end() if the item does not exist.
    * @param unsigned int the location of the item we are erasing.
    * @return const_iterator the item we are looking for.
    */
    iterator at( unsigned int _idx )
    {
      _MYODD_IDX_MAP_INRANGE
      if( _idx >= _mapVector.size() )
        return end();

      map_vector::iterator it = _mapVector.begin() + _idx;
      return (*it);
    }

  protected:
    /**
     * this is the vector that contains the index of all the map.
     */
    typedef typename std::vector<iterator> map_vector;
    map_vector _mapVector;
  }; //  idx_map
} //  myodd
#endif // _Idx_map_h
