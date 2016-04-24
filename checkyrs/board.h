//
//  board.h
//  checkyrs
//
//  Created by Chris Hengler on 22/04/16.
//  Copyright (c) 2016 chrysics. All rights reserved.
//

#ifndef __checkyrs__board__
#define __checkyrs__board__

#include <stdio.h>
#include <vector>

#include "square.h"
#include "position.h"

/**
 *  DraughtsBoard object
 *
 *  DraughtsBoard object contains 2D vector of Squares, representing board\n
 *  Also tracks number of pieces held by each player
 */
class Board {
  
protected:
  /** the size of the board */
  int m_size;
  /** number of pieces held by player 1 */
  int m_piecesp1;
  /** number of pieces held by player 2*/
  int m_piecesp2;
  /** the board itself */
  std::vector<std::vector<Square> > m_board;
  
public:
  Board(const int &size);
  Board(const Board &board);
  
  /**
   *  Return the size of the Board
   *
   *  @return the size of the board
   */
  int getSize() const{return m_size;}
  
  bool squareExists(const Position &p) const;
  bool squareIsOccupied(const Position &p) const;
  int distanceToSide(const Position &p) const;
  int distanceToEnd(const Position &p) const;
  int distanceToEdge(const Position &p) const;
  int getPlayer(const Position &p) const;
  
  void movePiece(const Position &oldp, const Position &newp);
  
  /**
   *  add a piece to the board
   *
   *  virtual function, should be defined by inheriting class
   *  (tracking of various piece types can't be done here, base class doesn't know what the types are)
   *
   *  @param p the Position to add the piece at
   *  @param player which player the piece belongs to
   *  @param type int form of enum representing the type of piece
   */
  virtual void addPiece(const Position &p,const int player=1, const int &type=0) {};
  
  /**
   *  Add several pieces
   *
   *  Player and type the same for all added pieces
   *  virtual function, should be defined by inheriting class
   *  (tracking of various piece types can't be done here, base class doesn't know what the types are)
   *
   *  @param p vector of Positions to add
   *  @param player which player these pieces belong to
   *  @param type whether these pieces are kings
   */
  virtual void addPieces(const std::vector<Position> &p, const int player=1, const int &type=0) {};
  
  /**
   *  Remove a piece from the Board
   *
   *  virtual function, should be defined by inheriting class
   *  (tracking of various piece types can't be done here, base class doesn't know what the types are)
   *
   *  @throw std::runtime_error if square does not exist
   *  @throw std::runtime_error if no piece to remove
   *  @param p the Position to remove a piece from
   */
  virtual void removePiece(const Position &p) {};
  
  /**
   *  Return a copy of the Board
   *
   *  virtual function, should be defined by inheriting class
   */
  virtual Board* getCopy() const {
    Board *copy = new Board(*this);
    return copy;
  };
  
  Square getSquare(const Position &p) const;
  
  /**
   *  Get number of pieces player has on board
   *
   *  @param player which player to check\n'1' is first player, any other value 2nd player
   *  @return number of pieces that player has on board
   */
  int getNumPiecesPlayer(const int player) const{return player==1 ? m_piecesp1 : m_piecesp2;}
  
  virtual ~Board(){};  
};


#endif /* defined(__checkyrs__board__) */
