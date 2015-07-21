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
  Board m_board;
  
  std::vector<Position> getJumpsFrom(const Position &p);
  std::vector<Position> getSingleMovesFrom(const Position &p);

public:
  Game(){m_board=Board::Board();}
  Game(const Board &board){m_board=board;};
  
  Board getBoard(){return m_board;}
  
  void AddPiece(const Position &pos,const int &player=1, const bool &isKing=false);
  void MovePiece(const Position &oldp, const Position &newp);

  std::vector<std::vector<Position> > getMovesFrom(const Position &p, const bool &alreadyMoved=false);
};


#endif /* defined(__checkyrs__game__) */
