//
//  position.h
//  checkyrs
//
//  Created by Chris Hengler on 18/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef checkyrs_position_h
#define checkyrs_position_h

#include <string>
#include <sstream>
#include <math.h>

/**
 *  Position struct
 *
 *  Doesn't do too much, just holds x,y values and provides readable string
 */
struct Position {
  /** x-location */
  int m_x;
  /** y-location */
  int m_y;
  
  /**
   *  Constructor
   *
   *  @param x column
   *  @param y row
   */
  Position(int x,int y){ m_x = x; m_y = y;}

  /**
   *  Turn zero-indexed x,y into more intuitive coordinates
   *
   *  @return readable position
   */
  std::string toString() const{
    //would rather use the std::to_string method but g++ doesn't seem to find it when compiling extension
    //for python3 via swig and python distutils, even with -std=c++11 specified
    //std::string str( ("abcdefghijklmnopqrstuvwxyz"[_x]+std::to_string(_y+1)) );
    std::ostringstream str("");
    str<<("abcdefghijklmnopqrstuvwxyz"[m_x]);
    str<<(m_y+1);
    return str.str();
  }
  
  /**
   *  Equals operator
   *  
   *  @param p Position for comparison
   *  @return true if x and y values of each are equal
   */
  bool operator==(const Position &p) const{
    return (m_x == p.m_x && m_y == p.m_y);
  }
  
  /**
   *  Not-equal operator
   *
   *  @param p Position for comparison
   *  @return opposite of ==
   */
  bool operator!=(const Position &p) const{
    return !(*this==p);
  }
  
};

namespace std{
  /**
   * template for hashing Position
   */
  template<> struct hash<Position>{
    /**
     *  () Operator for getting hash
     *
     *  @param p Position to be hashed
     *  @return hash
     */
    size_t operator()(const Position &p) const{
      return (hash<int>()(p.m_x) ^ hash<int>()(p.m_y));
    }
  };
}

  
#endif
