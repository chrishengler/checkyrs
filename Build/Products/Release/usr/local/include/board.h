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

class Board {
  int m_size;
  int m_piecesp1, m_piecesp2, m_kingsp1, m_kingsp2;
  std::vector<std::vector<Square> > m_board;
  
public:
  Board(const Board &board);
  Board(const int &size=8); //default 8x8 board, but let's leave the prospect of other sizes available
  
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
  int getNumPiecesPlayer(const int player) const{return player==1 ? m_piecesp1 : m_piecesp2;}
  int getNumKingsPlayer(const int player) const{return player==1 ? m_kingsp1 : m_kingsp2;}
  
  inline bool operator==(const Board &b){
    for(int ii=0;ii<m_size;ii++){
      for(int jj=0;jj<m_size;jj++){
        if(m_board[ii][jj]!=b.m_board[ii][jj]) return false;
      }
    }
    return true;
  }
  
  inline bool operator!=(const Board &b){
    return !(*this==b);
  }
};

#endif /* defined(__checkyrs__board__) */
