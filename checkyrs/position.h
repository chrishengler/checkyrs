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
    char buf[10];
    sprintf(buf, "%c%d", "abcdefghijklmnopqrstuvwxyz"[_x] ,(_y+1));
    std::string str(buf);
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
