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
#include <math.h>

struct Position {
  int _x;
  int _y;
  
  Position(int x,int y){ _x = x; _y = y;}

  std::string toString() const{
    std::string str( ("abcdefghijklmnopqrstuvwxyz"[_x]+std::to_string(_y+1)) ); //let's allow more than 8 in case we want oversized games at some point.
    return str;
  }
  
  bool operator==(const Position &p) const{
    return (_x == p._x && _y == p._y);
  }
  
  bool operator!=(const Position &p) const{
    return !(*this==p);
  }
  
};
  
#endif
