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
  bool m_gameover;
  int m_winner;
  
  std::vector<Position> getJumpsFrom(const Position &p);
  std::vector<Position> getSingleMovesFrom(const Position &p);
  std::vector<std::vector<Position> > ExtendMove(const std::vector<Position> &p);
  void RemovePiece(const Position &p);
  
  int evalState(const int &player, const int &depth);
  
public:
  Game(){m_board=Board::Board();m_gameover=false;m_winner=0;}
  Game(const Board &board){m_board=board;};
  
  Board getBoard(){return m_board;}
  
  void AddPiece(const Position &pos,const int &player=1, const bool &isKing=false);
  void MovePiece(const Position &oldp, const Position &newp);
  void ExecuteMove(const std::vector<Position> &move);
  std::pair<bool,int> gameOver();
                                                                       
  int getNumPiecesPlayer(const int &player){ return m_board.getNumPiecesPlayer(player); }

  std::vector<std::vector<Position> > getMovesFrom(const Position &p, const bool &alreadyMoved=false);
  std::vector<std::vector<Position> > getMovesForPlayer(const int &player);
  
  std::vector<Position> getJumpedSquares(const std::vector<Position> &p);
  

};


#endif /* defined(__checkyrs__game__) */
