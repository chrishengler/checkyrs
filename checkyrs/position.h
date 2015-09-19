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
  int _x;
  int _y;
  
  Position(int x,int y){ _x = x; _y = y;}

  std::string toString() const{
    //would rather use the std::to_string method but g++ doesn't seem to find it when compiling extension
    //for python3 via swig and python distutils, even with -std=c++11 specified
    //std::string str( ("abcdefghijklmnopqrstuvwxyz"[_x]+std::to_string(_y+1)) );
    std::ostringstream str("");
    str<<("abcdefghijklmnopqrstuvwxyz"[_x]);
    str<<(_y+1);
    return str.str();
  }
  
  bool operator==(const Position &p) const{
    return (_x == p._x && _y == p._y);
  }
  
  bool operator!=(const Position &p) const{
    return !(*this==p);
  }
  
};
  
#endif
