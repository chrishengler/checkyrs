//
//  board.h
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef __checkyrs__draughtsboard__
#define __checkyrs__draughtsboard__

#include <stdio.h>
#include <vector>

#include "board.h"

#include "square.h"
#include "position.h"

/**
 *  DraughtsBoard object
 *
 *  DraughtsBoard object contains 2D vector of Squares, representing board\n
 *  Also tracks number of pieces and number of kings held by each player
 */
class DraughtsBoard : public Board {
  
  int m_kingsp1, m_kingsp2;
  
public:
  DraughtsBoard(const DraughtsBoard &board);
  DraughtsBoard(const int &size=8); //default 8x8 board, but let's leave the prospect of other sizes available
  
  bool wasJump(const Position &p1, const Position &p2) const;
  Position getJump(const Position &p1, const Position &p2) const;
  
  bool squareHasKing(const Position &p) const;
  void setKing(const Position &p, const bool isKing=true);

  void addPiece(const Position &p,const int player=1, const int &type=0);
  void addPieces(const std::vector<Position> &p, const int player=1, const int &type=0);
  void removePiece(const Position &p);

  /**
   *  Get number of kings player has on board
   *
   *  @param player which player to check\n'1' is first player, any other value 2nd player
   *  @return number of kings that player has on board
   */
  int getNumKingsPlayer(const int player) const{return player==1 ? m_kingsp1 : m_kingsp2;}
  
  /**
   *  Equals operator for DraughtsBoard objects
   *
   *  @param b DraughtsBoard for comparison
   *  @return true if DraughtsBoards are the same
   */
  inline bool operator==(const DraughtsBoard &b) const{
    for(int ii=0;ii<m_size;ii++){
      //only even-even and odd-odd positions are valid, don't bother checking others
      for(int jj=( ii%2 == 0 ? 0 : 1); jj<m_size;jj++){
        Position p(ii,jj);
        if(this->getSquare(p)!=b.getSquare(p)){
          return false;
        }
      }
    }
    return true;
  }
  
  /**
   *  Not-equals operator for DraughtsBoard objects
   *
   *  @param b DraughtsBoard for comparison
   *  @return true if DraughtsBoards are not the same
   */
  inline bool operator!=(const DraughtsBoard &b) const{
    return !(*this==b);
  }
  
  /**
   *  Less than operator for DraughtsBoards
   *
   *  Actual ordering is arbitrary, but some comparison needed to be able to use in std::map\n
   *  Test size, smaller board considered smaller.\n
   *  If no difference in size then loop through each square and return comparison of
   *  first square where boards differ
   *
   *  @param b DraughtsBoard for comparison
   *  @return according to description above
   */
   inline bool operator<(const DraughtsBoard &b) const{
    
    if(m_size!=b.getSize()) return (m_size<b.getSize());
    for(int ii=0;ii<m_size;ii++){
      for(int jj=0; jj<m_size;jj++){
        Position p(ii,jj);
        if(getSquare(p) == b.getSquare(p)) continue;
        else return (getSquare(p) < b.getSquare(p));
      }
    }
    return false;
  }
  
  virtual ~DraughtsBoard(){};
  
};

namespace std{

  /**
   *  Hash for use in std::map
   */
  template<> struct hash<DraughtsBoard>{
    /**
     * () operator for hash
     *
     *  @param b DraughtsBoard to be hashed
     *  @return hash
     */
    size_t operator()(const DraughtsBoard &b) const{
      size_t h=0;
      for(int ii=0;ii<b.getSize();ii++){
        for(int jj=0; jj<b.getSize();jj++){
          Position p(ii,jj);
          h |= (hash<Position>()(p) ^ hash<Square>()(b.getSquare(p)));
        }
      }
      return h;
    }
  };
  
}

#endif /* defined(__checkyrs__draughtsboard__) */
