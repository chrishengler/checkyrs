//
//  square.h
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef __checkyrs__square__
#define __checkyrs__square__

#include <stdio.h>
class Square {
  
  bool _isOccupied;
  
  int _player;
  bool _isKing;

public:
  Square(){_isOccupied=false; _player=0; _isKing=false;}
  Square(const int &p, const bool &k=false){_isOccupied=true; _player=p; _isKing=k;}
  
  bool isOccupied(){return _isOccupied;}
  bool isKing(){return _isKing;}
  int getPlayer(){return _player;}
  void removePiece(){_isOccupied=false; _player=0; _isKing=false;}
};
#endif /* defined(__checkyrs__square__) */
