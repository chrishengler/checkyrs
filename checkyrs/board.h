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
#include "piece.h"

class Board {
  int m_size;
  std::vector<std::vector<Piece*> > m_board;
  
public:
  Board(int size=8);
  
  int getSize(){return m_size;}
  
  bool SquareIsOccupied(int x, int y);
  
  void AddPiece(int x, int y);
  void MovePiece(int oldx, int oldy, int newx, int newy);
  
};

#endif /* defined(__checkyrs__board__) */
