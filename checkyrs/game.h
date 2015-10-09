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
#include <unordered_map>
#include "board.h"

class Game {
  Board m_board;
  mutable bool m_gameOver;
  mutable int m_winner;
  int m_currentPlayer;
  int m_staleness;
  int m_maxStaleness;
  bool m_stale;
  
  int m_turn;
  
  std::vector<Position> getJumpsFrom(const Position &p) const;
  std::vector<Position> getSingleMovesFrom(const Position &p) const;
  std::vector<std::vector<Position> > extendMove(const std::vector<Position> &p) const;
  void removePiece(const Position &p);
  
public:
  Game(){
    m_board=Board::Board();
    m_gameOver=false;
    m_winner=0;
    m_currentPlayer=1;
    m_staleness=0;
    m_maxStaleness=50;
    m_stale=false;
    m_turn=0;
  }
  Game(const int size){
    m_board=Board::Board(size);
    m_gameOver=false;
    m_winner=0;
    m_currentPlayer=1;
    m_staleness=0;
    m_maxStaleness=50;
    m_stale = false;
    m_turn = 0;
  }
  Game(const Board &board){
    m_board=board;
    m_gameOver=false;
    m_winner=0;
    m_currentPlayer=1;
    m_staleness=0;
    m_maxStaleness=50;
    m_stale = false;
    m_turn = 0;
  };
  Game(const Game &g){
    m_board         = g.m_board;
    m_gameOver      = g.m_gameOver;
    m_winner        = g.m_winner;
    m_currentPlayer = g.m_currentPlayer;
    m_staleness     = g.m_staleness;
    m_maxStaleness  = g.m_maxStaleness;
    m_stale         = g.m_stale;
    m_turn          = g.m_turn;
  };
  
  Board getBoard() const{return m_board;}
  
  void addPiece(const Position &pos,const int player=1, const bool isKing=false);
  void addPieces(const std::vector<Position> &pos, const int player=1, const bool isKing=false);
  void addPieces(const std::vector<Position> &p1, const std::vector<Position> &p2, const bool isKing=false);
  void movePiece(const Position &oldp, const Position &newp);
  void prepareBoard();
  void executeMove(const std::vector<Position> &move);
  
  bool gameOver() const{ return m_gameOver; }
  int getWinner() const{ return m_winner; }
  int getCurrentPlayer() const{ return m_currentPlayer; }
  int getCurrentTurn() const{ return m_turn; }
  
  int getStaleness() const{ return m_staleness; }
  int getMaxStaleness() const{ return m_maxStaleness; }
  bool isStale() const{ return m_stale; }
  
  int getNumPiecesPlayer(const int player) const{ return m_board.getNumPiecesPlayer(player); }
  int getNumKingsPlayer(const int player) const{ return m_board.getNumKingsPlayer(player); }
  
  bool pieceIsThreatened(const Position &p) const;
  int pieceDefence(const Position &p) const;
  bool pieceCanCapture(const Position &p) const;
  bool pieceCanCrown(const Position &p) const;

  std::vector<std::vector<Position> > getMovesFrom(const Position &p, const bool alreadyMoved=false) const;
  std::vector<std::vector<Position> > getMovesForPlayer(const int player) const;
  
  std::vector<Position> getJumpedSquares(const std::vector<Position> &p) const;
  
  inline bool operator==(const Game &g) const{
    if(m_currentPlayer != g.m_currentPlayer) return false;
    else return m_board==g.m_board;
  }
  inline bool operator!=(const Game &g) const{
    return !(*this==g);
  }
};

namespace std{
  template<> struct hash<Game>{
    size_t operator()(const Game &g) const{
      return ( hash<int>()(g.getCurrentPlayer()) ^ hash<Board>()(g.getBoard()));
    }
  };
}
#endif /* defined(__checkyrs__game__) */
