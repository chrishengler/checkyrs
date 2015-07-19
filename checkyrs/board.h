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
  std::vector<std::vector<Square> > m_board;
  
public:
  Board(int size=8);
  
  int getSize(){return m_size;}
  
  bool PositionExists(const Position &p);
  bool SquareIsOccupied(const Position &p);
  
  bool SquareHasKing(const Position &p);
  std::vector<Position> getMovesFrom(const Position &p);
  
  void AddPiece(const Position &p,const int &player=1, const bool &isKing=false);
  void MovePiece(const Position &oldp, const Position &newp);
  
};

#endif /* defined(__checkyrs__board__) */
