//
//  game.h
//  checkyrs
//
//  Created by Chris Hengler on 24/04/16.
//  Copyright (c) 2016 chrysics. All rights reserved.
//

#ifndef __checkyrs__game__
#define __checkyrs__game__

#include <stdio.h>
#include <vector>
#include <functional>
#include <map>

#include "draughtsboard.h"

class Game {
protected:
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
  
  /**
   *  Initialise member variables tracking game state
   */
  void initMembers(){
    m_gameOver=false;
    m_winner=0;
    m_currentPlayer=1;
    m_staleness=0;
    m_maxStaleness=50;
    m_stale = false;
    m_turn = 0;
  }
  
public:
  /**
   *  Default constructor
   *
   *  @param size size of board
   */
  Game(const int size=8){
    initMembers();
  }
  
  virtual void prepareBoard() = 0;
  virtual void executeMove(const std::vector<Position> &move) = 0;
  
  virtual std::vector<std::vector<Position> > getMovesFrom(const Position &p) const = 0;
  virtual std::vector<std::vector<Position> > getMovesForPlayer(const int player) const = 0;
  
  virtual std::vector<Position> getJumpedSquares(const std::vector<Position> &p) const = 0;
  
  /**
   *  Check if game over state reached
   *
   *  @return true if game is over
   */
  bool gameOver() const{ return m_gameOver; }
  /**
   *  Get winning player
   *
   *  @return 1 if first player has won,\n -1 if second player has won\n 0 if no winner
   */
  int getWinner() const{ return m_winner; }
  /**
   *  Get current player
   *
   *  @return 1 if first player, -1 if second player
   */
  int getCurrentPlayer() const{ return m_currentPlayer; }
  /**
   *  Get current turn
   *
   *  @return which turn the game is on
   */
  int getCurrentTurn() const{ return m_turn; }
  
  /**
   *  get staleness
   *
   *  'staleness' tracks how close the Game is to a draw
   *  being forced due to no pieces  being promoted or taken
   *
   *  @return current staleness
   */
  int getStaleness() const{ return m_staleness; }
  
  /**
   *  get max staleness
   *
   *  @return value of staleness when draw will be declared
   */
  int getMaxStaleness() const{ return m_maxStaleness; }
  /**
   *  Check if game is 'stale'
   *
   *  @return true if staleness ≥ max staleness
   */
  bool isStale() const{ return m_stale; }
  
  virtual ~Game(){ }
};

#endif /* defined(__checkyrs__game__) */
