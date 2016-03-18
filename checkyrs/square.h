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

/**
 *  Square class holds information for place on board
 */
class Square {
  
  bool m_isOccupied;
  
  int m_player;
  bool m_isKing;

public:
  Square(){m_isOccupied=false; m_player=0; m_isKing=false;}
  Square(const int p, const bool k=false){m_isOccupied=true; m_player=p; m_isKing=k;}
  Square(const Square &s);
  
  /**
   *  Check if Square holds piece
   *
   *  @return true if there is a piece on this Square
   */
  bool isOccupied() const{return m_isOccupied;}

  /**
   *  Check if Square holds king
   *
   *  @return true if king
   */
  bool isKing() const{return m_isKing;}
  
  /**
   *  Set King
   *
   *  Set the Piece on this Square to be (not) king
   *
   *  @param isKing whether piece should be king
   */
  void setKing(const bool isKing=true){ m_isKing=isKing; }

  /**
   *  Get player on this Square
   *
   *  @return which player has piece on this Square
   */
  int getPlayer() const{return m_player;}
  void removePiece(){m_isOccupied=false; m_player=0; m_isKing=false;}
  
  /**
   *  Int representing Square's occupation/king/player status
   *  
   *  used in generating hash for lookup tables\n
   *  definition is somewhat arbitrary and subject to change
   *
   *  @return int representing Square's status
   */
  inline int asBits() const{
    int bits=0;
    //use bits as bitmask
    if(m_isOccupied) bits|=1;
    if(m_isKing) bits|=2;
    if(m_player==1) bits|=4;
    return bits;
  }
  
  /**
   *  Equals operator for Square
   *
   *  Compares occupation status, piece type, and player
   *  Does not require position on board to be the same
   *
   *  @param Square for comparison
   *  @return true if equal
   */
  inline bool operator==(const Square &s){
    return (m_isOccupied==s.m_isOccupied && m_isKing==s.m_isKing && m_player==s.m_player);
  }
  
  /**
   *  Not-equal operator for Square
   *
   *  Returns opposite of Equals operator
   *
   *  @param s Square for comparison
   *  @return !(this==s)
   */
  inline bool operator!=(const Square &s){
    return !(*this==s);
  }
  
  /**
   *  Less than operator for Square
   *
   *  Not really meaningful? Required for hash implementation\n
   *  Comparison of Square.asBits()
   *
   *  @param s Square for comparison
   *  @return true if s.asBits() < this->asBits()
   */
  inline bool operator<(const Square &s){
    return (asBits()<s.asBits());
  }
  
};

namespace std{
  /**
   * Hash for Square
   */
  template<> struct hash<Square>{
    /**
     * () operator to get hash
     *
     * @param s Square to be hashed
     * @return hash
     */
    size_t operator()(const Square &s) const{
      return hash<int>()(s.asBits());
    }
  };
}
#endif /* defined(__checkyrs__square__) */
