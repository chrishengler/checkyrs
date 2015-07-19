//
//  game.h
//  checkyrs
//
//  Created by Chris Hengler on 19/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef __checkyrs__game__
#define __checkyrs__game__

#include <stdio.h>
#include "board.h"

class Game {
  Board _board;

public:
  Game(){_board=Board::Board();}
  
  void AddPiece(const Position &pos,const int &player=1, const bool &isKing=false);
  void MovePiece(const Position &oldp, const Position &newp);

  std::vector<Position> getMovesFrom(const Position &p);
};


#endif /* defined(__checkyrs__game__) */
