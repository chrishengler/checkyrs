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
#include <functional>
#include <map>
#include "board.h"

/**
 *  Game class
 *
 *  Each instance holds a board for the current game, and tracks relevant 
 *  variables such as whether the game is over, which player's turn is next,
 *  how close the game is to a draw being forced
 */
class Game {
  Board m_board;
  mutable bool m_gameOver;
  mutable int m_winner;
  int m_currentPlayer;
  int m_staleness;
  int m_maxStaleness;
  bool m_stale;
  
  int m_turn;
  
  std::map<Board,int> m_paststates;
  
  std::vector<Position> getJumpsFrom(const Position &p) const;
  std::vector<Position> getSingleMovesFrom(const Position &p) const;
  std::vector<std::vector<Position> > extendMove(const std::vector<Position> &p) const;
  void removePiece(const Position &p);
  
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
    m_board=Board::Board(size);
  }
  /**
   *  Constructor copying board
   *
   *  @param board the Board object will be copied as the initial board state at game start
   */
  Game(const Board &board){
    initMembers();
    m_board=board;
  };
  /**
   *  Copy constructor
   *
   *  @Param g Game object to be copied
   */
  Game(const Game &g){
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
   *  @return the Board
   */
  Board getBoard() const{return m_board;}
  
  void addPiece(const Position &pos,const int player=1, const bool isKing=false);
  void addPieces(const std::vector<Position> &pos, const int player=1, const bool isKing=false);
  void addPieces(const std::vector<Position> &p1, const std::vector<Position> &p2, const bool isKing=false);
  void movePiece(const Position &oldp, const Position &newp);
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
   *  @return 1 if first player has won,\n -1 if second player has won\n0 if no winner
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
  
  /**
   *  get number of pieces on board for player
   *
   *  Simply calls Board.getNumPiecesPlayer(player)
   *
   *  @param player which player to check
   *  @return number of pieces player has
   */
  int getNumPiecesPlayer(const int player) const{ return m_board.getNumPiecesPlayer(player); }
  /**
   *  get number of kings on board for player
   *
   *  Simply calls Board.getNumKingsPlayer(player)
   *
   *  @param player which player to check
   *  @return number of kings player has
   */
  int getNumKingsPlayer(const int player) const{ return m_board.getNumKingsPlayer(player); }
  
  bool pieceIsThreatened(const Position &p) const;
  int pieceDefence(const Position &p) const;
  bool pieceCanCapture(const Position &p) const;
  bool pieceCanCrown(const Position &p) const;

  std::vector<std::vector<Position> > getMovesFrom(const Position &p, const bool alreadyMoved=false) const;
  std::vector<std::vector<Position> > getMovesForPlayer(const int player) const;
  
  std::vector<Position> getJumpedSquares(const std::vector<Position> &p) const;
  
  /**
   *  Equals operator for Game objects
   *
   *  Only checks board positions and current player
   *
   *  @param g Game for comparison
   *  @return true if Games are equal as defined above, false otherwise
   */
  inline bool operator==(const Game &g) const{
    if(m_currentPlayer != g.m_currentPlayer) return false;
    else return m_board==g.m_board;
  }
  /**
   *  Not equal operator for Game objects
   *
   *  @param g Game for comparison
   *  @return opposite of ==
   */
  inline bool operator!=(const Game &g) const{
    return !(*this==g);
  }
};
#endif /* defined(__checkyrs__game__) */
