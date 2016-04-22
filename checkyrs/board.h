//
//  board.h
//  checkyrs
//
//  Created by Chris Hengler on 22/04/16.
//  Copyright (c) 2016 chrysics. All rights reserved.
//

#ifndef __checkyrs__board__
#define __checkyrs__board__

#include <stdio.h>
#include <vector>

#include "square.h"
#include "position.h"


class Board {
  
protected:
  int m_size;
  int m_piecesp1, m_piecesp2;
  std::vector<std::vector<Square> > m_board;
  
public:
  Board(const int &size);
  Board(const Board &board);
  
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
  
  virtual void addPiece(const Position &p,const int player=1, const int &type=0){ };
  virtual void addPieces(const std::vector<Position> &p, const int player=1, const int &type=0){ };
  virtual void movePiece(const Position &oldp, const Position &newp){ };
  virtual void removePiece(const Position &p){ };
  
  Square getSquare(const Position &p) const;
  
  /**
   *  Get number of pieces player has on board
   *
   *  @param player which player to check\n'1' is first player, any other value 2nd player
   *  @return number of pieces that player has on board
   */
  int getNumPiecesPlayer(const int player) const{return player==1 ? m_piecesp1 : m_piecesp2;}

  /**
   *  Equals operator for Board objects
   *
   *  @param b Board for comparison
   *  @return true if Boards are the same
   */
  inline bool operator==(const Board &b) const{
    for(int ii=0;ii<m_size;ii++){
      //only even-even and odd-odd positions are valid, don't bother checking others
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
  
  virtual ~Board(){};  
};


#endif /* defined(__checkyrs__board__) */
