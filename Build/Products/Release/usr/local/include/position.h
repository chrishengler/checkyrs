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

struct Position {
  int m_x;
  int m_y;
  
  Position(int x,int y){ m_x = x; m_y = y;}

  std::string toString() const{
    //would rather use the std::to_string method but g++ doesn't seem to find it when compiling extension
    //for python3 via swig and python distutils, even with -std=c++11 specified
    //std::string str( ("abcdefghijklmnopqrstuvwxyz"[_x]+std::to_string(_y+1)) );
    std::ostringstream str("");
    str<<("abcdefghijklmnopqrstuvwxyz"[m_x]);
    str<<(m_y+1);
    return str.str();
  }
  
  bool operator==(const Position &p) const{
    return (m_x == p.m_x && m_y == p.m_y);
  }
  
  bool operator!=(const Position &p) const{
    return !(*this==p);
  }
  
};

namespace std{
  template<> struct hash<Position>{
    size_t operator()(const Position &p) const{
      return (hash<int>()(p.m_x) ^ hash<int>()(p.m_y));
    }
  };
}

  
#endif
