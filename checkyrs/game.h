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
  mutable bool m_gameover;
  mutable int m_winner;
  int m_currentplayer;
  int m_staleness;
  int m_maxstaleness;
  bool m_stale;
  
  std::vector<Position> getJumpsFrom(const Position &p) const;
  std::vector<Position> getSingleMovesFrom(const Position &p) const;
  std::vector<std::vector<Position> > ExtendMove(const std::vector<Position> &p) const;
  void RemovePiece(const Position &p);
  
public:
  Game(){
    m_board=Board::Board();
    m_gameover=false;
    m_winner=0;
    m_currentplayer=1;
    m_staleness=0;
    m_maxstaleness=50;
    m_stale=false;
  }
  Game(const int size){
    m_board=Board::Board(size);
    m_gameover=false;
    m_winner=0;
    m_currentplayer=1;
    m_staleness=0;
    m_maxstaleness=50;
    m_stale = false;
  }
  Game(const Board &board){
    m_board=board;
    m_gameover=false;
    m_winner=0;
    m_currentplayer=1;
    m_staleness=0;
    m_maxstaleness=50;
    m_stale = false;
  };
  Game(const Game &g){
    m_board         = g.m_board;
    m_gameover      = g.m_gameover;
    m_winner        = g.m_winner;
    m_currentplayer = g.m_currentplayer;
    m_staleness     = g.m_staleness;
    m_maxstaleness  = g.m_maxstaleness;
    m_stale         = g.m_stale;
  };
  
  Board getBoard() const{return m_board;}
  
  void AddPiece(const Position &pos,const int player=1, const bool isKing=false);
  void AddPieces(const std::vector<Position> &pos, const int player=1, const bool isKing=false);
  void AddPieces(const std::vector<Position> &p1, const std::vector<Position> &p2, const bool isKing=false);
  void MovePiece(const Position &oldp, const Position &newp);
  void PrepareBoard();
  void ExecuteMove(const std::vector<Position> &move);
  
  bool gameOver() const{ return m_gameover; }
  int getWinner() const{ return m_winner; }
  int getCurrentPlayer() const{ return m_currentplayer; }
  
  int getStaleness() const{ return m_staleness; }
  int getMaxStaleness() const{ return m_maxstaleness; }
  bool isStale() const{ return m_stale; }
  
  int getNumPiecesPlayer(const int player) const{ return m_board.getNumPiecesPlayer(player); }

  std::vector<std::vector<Position> > getMovesFrom(const Position &p, const bool alreadyMoved=false) const;
  std::vector<std::vector<Position> > getMovesForPlayer(const int player) const;
  
  std::vector<Position> getJumpedSquares(const std::vector<Position> &p) const;
};


#endif /* defined(__checkyrs__game__) */
