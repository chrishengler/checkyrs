//
//  board.h
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef __checkyrs__board__
#define __checkyrs__board__

#include <stdio.h>
#include <vector>

#include "square.h"
#include "position.h"

/**
 *  Board object
 *
 *  Board object contains 2D vector of Squares, representing board\n
 *  Also tracks number of pieces and number of kings held by each player
 */
class Board {
  int m_size;
  int m_piecesp1, m_piecesp2, m_kingsp1, m_kingsp2;
  std::vector<std::vector<Square> > m_board;
  
public:
  Board(const Board &board);
  Board(const int &size=8); //default 8x8 board, but let's leave the prospect of other sizes available
  
  /**
   *  Return the size of the Board
   *
   *  @return the size of the board
   */
  int getSize() const{return m_size;}
  
  bool squareExists(const Position &p) const;
  bool squareIsOccupied(const Position &p) const;
  int distanceToSide(const Position &p) const;
  int distanceToEnd(const Position &p) const;
  int distanceToEdge(const Position &p) const;
  int getPlayer(const Position &p) const;
  bool wasJump(const Position &p1, const Position &p2) const;
  Position getJump(const Position &p1, const Position &p2) const;
  
  bool squareHasKing(const Position &p) const;
  void setKing(const Position &p, const bool isKing=true);

  void addPiece(const Position &p,const int player=1, const bool isKing=false);
  void addPieces(const std::vector<Position> &p, const int player=1, const bool isKing=false);
  void movePiece(const Position &oldp, const Position &newp);
  void removePiece(const Position &p);
  
  Square getSquare(const Position &p) const;

  /**
   *  Get number of pieces player has on board
   *
   *  @param player which player to check\n'1' is first player, any other value 2nd player
   *  @return number of pieces that player has on board
   */
  int getNumPiecesPlayer(const int player) const{return player==1 ? m_piecesp1 : m_piecesp2;}
  /**
   *  Get number of kings player has on board
   *
   *  @param player which player to check\n'1' is first player, any other value 2nd player
   *  @return number of kings that player has on board
   */
  int getNumKingsPlayer(const int player) const{return player==1 ? m_kingsp1 : m_kingsp2;}
  
  /**
   *  Equals operator for Board objects
   *
   *  @param b Board for comparison
   *  @return true if Boards are the same
   */
  inline bool operator==(const Board &b) const{
    for(int ii=0;ii<m_size;ii++){
      for(int jj=( ii%2 == 0 ? 0 : 1); jj<m_size;jj++){
        Position p(ii,jj);
        if(this->getSquare(p)!=b.getSquare(p)) return false;
      }
    }
    return true;
  }
  
  /**
   *  Not-quals operator for Board objects
   *
   *  @param b Board for comparison
   *  @return true if Boards are not the same
   */
  inline bool operator!=(const Board &b) const{
    return !(*this==b);
  }
  
  /**
   *  Less than operator for Boards
   * 
   *  Actual ordering is arbitrary, but some comparison needed to be able to use in std::map\n
   *  Test size, smaller board considered smaller.\n
   *  If no difference in size then loop through each square and return comparison of
   *  first square where boards differ
   *
   *  @param b Board for comparison
   *  @return according to description above
   */
  inline bool operator<(const Board &b) const{

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
  
};

namespace std{

  /**
   *  Hash for use in std::map
   */
  template<> struct hash<Board>{
    /**
     * () operator for hash
     *
     *  @param b Board to be hashed
     *  @return hash
     */
    size_t operator()(const Board &b) const{
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

#endif /* defined(__checkyrs__board__) */
