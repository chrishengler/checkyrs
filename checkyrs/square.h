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
#include <functional>

class Square {
  
  bool m_isOccupied;
  
  int m_player;
  bool m_isKing;

public:
  Square(){m_isOccupied=false; m_player=0; m_isKing=false;}
  Square(const int p, const bool k=false){m_isOccupied=true; m_player=p; m_isKing=k;}
  Square(const Square &s);
  
  bool isOccupied() const{return m_isOccupied;}
  bool isKing() const{return m_isKing;}
  void setKing(const bool isKing=true){ m_isKing=isKing; }
  int getPlayer() const{return m_player;}
  void removePiece(){m_isOccupied=false; m_player=0; m_isKing=false;}
  
  inline bool operator==(const Square &s){
    return (m_isOccupied==s.m_isOccupied && m_isKing==s.m_isKing && m_player==s.m_player);
  }
  inline bool operator!=(const Square &s){
    return !(*this==s);
  }
  
};

namespace std{
  template<> struct hash<Square>{
    size_t operator()(const Square &s) const{
      int bits=0;
      //use bits as bitmask
      if(s.isOccupied()) bits|=1;
      if(s.isKing()) bits|=2;
      if(s.getPlayer()==1) bits|=4;
      return hash<int>()(bits);
    }
  };
}
#endif /* defined(__checkyrs__square__) */
