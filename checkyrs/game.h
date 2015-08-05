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
#include <vector>
#include "board.h"

class Game {
  Board m_board;
  bool m_gameover;
  int m_winner;
  
  std::vector<Position> getJumpsFrom(const Position &p) const;
  std::vector<Position> getSingleMovesFrom(const Position &p) const;
  std::vector<std::vector<Position> > ExtendMove(const std::vector<Position> &p) const;
  void RemovePiece(const Position &p);
  
public:
  Game(){m_board=Board::Board();m_gameover=false;m_winner=0;}
  Game(const Board &board){m_board=board;};
  
  Board getBoard() const{return m_board;}
  
  void AddPiece(const Position &pos,const int &player=1, const bool &isKing=false);
  void MovePiece(const Position &oldp, const Position &newp);
  void ExecuteMove(const std::vector<Position> &move);
  std::pair<bool,int> gameOver() const;
                                                                       
  int getNumPiecesPlayer(const int &player) const{ return m_board.getNumPiecesPlayer(player); }

  std::vector<std::vector<Position> > getMovesFrom(const Position &p, const bool &alreadyMoved=false) const;
  std::vector<std::vector<Position> > getMovesForPlayer(const int &player) const;
  
  std::vector<Position> getJumpedSquares(const std::vector<Position> &p) const;
  

};


#endif /* defined(__checkyrs__game__) */
