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
