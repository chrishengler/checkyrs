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
  
  bool m_isOccupied;
  
  int m_player;
  bool m_isKing;

public:
  Square(){m_isOccupied=false; m_player=0; m_isKing=false;}
  Square(const int &p, const bool &k=false){m_isOccupied=true; m_player=p; m_isKing=k;}
  Square(const Square &s);
  
  bool isOccupied(){return m_isOccupied;}
  bool isKing(){return m_isKing;}
  int getPlayer(){return m_player;}
  void removePiece(){m_isOccupied=false; m_player=0; m_isKing=false;}
};
#endif /* defined(__checkyrs__square__) */
