//
//  game.h
//  checkyrs
//
//  Created by Chris Hengler on 19/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef __checkyrs__draughts__
#define __checkyrs__draughts__

#include <stdio.h>
#include <vector>
#include <functional>
#include <map>

#include "game.h"

#include "draughtsboard.h"


/**
 *  Game class
 *
 *  Each instance holds a board for the current game, and tracks relevant 
 *  variables such as whether the game is over, which player's turn is next,
 *  how close the game is to a draw being forced
 */
class Draughts: public Game {
protected:
  DraughtsBoard m_board;
  
  std::vector<Position> getJumpsFrom(const Position &p) const;
  std::vector<Position> getSingleMovesFrom(const Position &p) const;
  std::vector<std::vector<Position> > extendMove(const std::vector<Position> &p) const;
  
  std::map<DraughtsBoard,int> m_paststates;
  
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
  Draughts(const int size=8): Game(size){
    m_board = DraughtsBoard(size);
  }
  /**
   *  Constructor copying board
   *
   *  @param board the Board object will be copied as the initial board state at game start
   */
  Draughts(const DraughtsBoard &board): Game( board.getSize() ){
    m_board = DraughtsBoard(board);
    initMembers();
  };
  
  /**
   *  Copy constructor
   *
   *  @param g Game object to be copied
   */
  Draughts(const Draughts &g) : Game( g.m_board.getSize() ){
    m_board         = g.m_board;
    m_gameOver      = g.m_gameOver;
    m_winner        = g.m_winner;
    m_currentPlayer = g.m_currentPlayer;
    m_staleness     = g.m_staleness;
    m_maxStaleness  = g.m_maxStaleness;
    m_stale         = g.m_stale;
    m_turn          = g.m_turn;
    m_paststates    = g.m_paststates;
  };
  
  /**
   *  Get board
   *
   *  @return the DraughtsBoard
   */
  DraughtsBoard getBoard() const{return m_board;}
  
  void prepareBoard();
  void executeMove(const std::vector<Position> &move);
  
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
  
  bool pieceIsThreatened(const Position &p) const;
  int pieceDefence(const Position &p) const;
  bool pieceCanCapture(const Position &p) const;
  bool pieceCanCrown(const Position &p) const;

  std::vector<std::vector<Position> > getMovesFrom(const Position &p) const;
  std::vector<std::vector<Position> > getMovesForPlayer(const int player) const;
  
  std::vector<Position> getJumpedSquares(const std::vector<Position> &p) const;
  
  /**
   *  Returns the number of pieces held by the player
   *
   *  Virtual, should be implemented by inheriting class
   *
   *  @param player which player to check
   *  @return number of pieces player has on board
   */
  int getNumPiecesPlayer(const int &player) const{
    return m_board.getNumPiecesPlayer(player);
  }

  /**
   *  Equals operator for Game objects
   *
   *  Only checks board positions and current player
   *
   *  @param g Game for comparison
   *  @return true if Games are equal as defined above, false otherwise
   */
  inline bool operator==(const Draughts &g) const{
    if(m_currentPlayer != g.m_currentPlayer) return false;
    else return m_board==g.m_board;
  }
  /**
   *  Not equal operator for Game objects
   *
   *  @param g Game for comparison
   *  @return opposite of ==
   */
  inline bool operator!=(const Draughts &g) const{
    return !(*this==g);
  }
  
  virtual ~Draughts() { };
};
#endif /* defined(__checkyrs__draughts__) */
