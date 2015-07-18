//
//  position.h
//  checkyrs
//
//  Created by Chris Hengler on 18/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef checkyrs_position_h
#define checkyrs_position_h

struct Position {
  int _x;
  int _y;
  
  Position(int x,int y){ _x = x; _y = y;}
  std::string toString() const{
    char buf[10];
    sprintf(buf, "%d,%d",_x,_y);
    std::string str(buf);
    return str;
  }

};
  
#endif