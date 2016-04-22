//
//  board.cpp
//  checkyrs
//
//  Created by Chris Hengler on 22/04/16.
//  Copyright (c) 2016 chrysics. All rights reserved.
//

#include "board.h"

/**
 *  Constructor for new board with given size
 *
 *  Boards are always square
 *
 *  @param size the number of rows & columns this board should have
 */
Board::Board(const int &size){

  m_size=size;
  
  m_piecesp1=0;
  m_piecesp2=0;
  
  m_board.resize(size);
  for(int ii=0;ii<size;ii++){
    m_board[ii].resize(size);
    for(int jj=0;jj<size;jj++){
      m_board[ii][jj] = Square::Square();
    }
  }
  
}

/**
 *  Board copy-constructor
 *
 *  @param board the Board to copy
 */
Board::Board(const Board &board){
  
  m_size=board.m_size;
  
  m_piecesp1=board.m_piecesp1;
  m_piecesp2=board.m_piecesp2;
  
  m_board.resize(m_size);
  for(int ii=0;ii<m_size;ii++){
    m_board[ii].resize(m_size);
    for(int jj=0;jj<m_size;jj++){
      m_board[ii][jj] = Square::Square(board.m_board[ii][jj]);
    }
  }
  
}

/**
 *  Check that a square exists
 *
 *  Avoid calling methods on non-existent squares
 *
 *  @param p Position to check for existence
 *  @return true if valid position
 */
bool Board::squareExists(const Position &p) const{
  return (p.m_x >= 0 && p.m_x < m_size && p.m_y >=0 && p.m_y < m_size);
}

/**
 *  Check if a piece exists on a square
 *
 *  @param p the Position to check
 *  @return true if occupied
 */
bool Board::squareIsOccupied(const Position &p) const{
  if(!squareExists(p)){
    std::string errmsg("Trying to check occupation of invalid position:");
    errmsg+=p.toString();
    throw std::out_of_range(errmsg);
  }
  else return (m_board[p.m_x][p.m_y].isOccupied());
}

/**
 *  Get the distance from this Position to the side (i.e. x-axis edge) of the board
 *
 *  @param p the Position to check
 *  @return how far this square is from the side of the board
 */
int Board::distanceToSide(const Position &p) const{
  if(!squareExists(p)){
    std::string errmsg("Checking distance to side from non-existent position:");
    errmsg+=p.toString();
    throw std::out_of_range(errmsg);
  }
  int left = p.m_x;
  int right = (m_size-1)-p.m_x;
  return ( left>right ? right : left );
}


/**
 *  Get the distance from this Position to the end (i.e. y-axis edge) of the board
 *
 *  @param p the Position to check
 *  @return how far this square is from the end of the board
 */
int Board::distanceToEnd(const Position &p) const{
  if(!squareExists(p)){
    std::string errmsg("Checking distance to end from non-existent position:");
    errmsg+=p.toString();
    throw std::out_of_range(errmsg);
  }
  int bottom = p.m_x;
  int top = (m_size-1)-p.m_y;
  return ( top>bottom ? bottom : top );
}


/**
 *  Get the distance from this Position to the edge (either axis) of the board
 *
 *  @param p the Position to check
 *  @return how far this square is from the edge of the board
 */
int Board::distanceToEdge(const Position &p) const{
  int lateral = distanceToSide(p);
  int longitudinal = distanceToEnd(p);
  return ( lateral>longitudinal ? longitudinal : lateral );
}

/**
 *  Which player has a piece on this square?
 *
 *  @throw std::out_of_range if not a valid square
 *  @throw std::runtime_error if square is empty
 *  @param p the Position to check
 *  @return which player does the piece belong to?
 */
int Board::getPlayer(const Position &p) const{
  if(!squareExists(p)){
    std::string errmsg("Checking owner of non-existent position:");
    errmsg+=p.toString();
    throw std::out_of_range(errmsg);
  }
  if(!squareIsOccupied(p)){
    std::string errmsg("Checking owner of empty square:");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }
  return m_board[p.m_x][p.m_y].getPlayer();
}

/**
 *  Return a square from the board
 *
 *  @throw std::runtime_error if square does not exist
 *  @param p Position of Square to be returned
 *  @return the desired Square
 */
Square Board::getSquare(const Position &p) const{
  if(!squareExists(p)){
    std::string errmsg("Tried to get piece from non-existent square");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }
  /*if(!squareIsOccupied(p)){
   std::string errmsg("Tried to get piece from empty square");
   errmsg+=p.toString();
   throw std::runtime_error(errmsg);
   }*/
  return m_board[p.m_x][p.m_y];
}



/**
 *  Move a piece
 *
 *  @throw std::runtime_error if no such piece exists
 *  @throw std::runtime_error if destination is already occupied
 *  @param oldp starting position
 *  @param newp new position
 */
void Board::movePiece(const Position &oldp, const Position &newp){
  if(!(squareIsOccupied(oldp))){
    std::string errmsg("Tried to move non-existent piece:");
    errmsg+=oldp.toString();
    throw std::runtime_error(errmsg);
  }
  else if(squareIsOccupied(newp)){
    std::string errmsg("Tried to move to occupied square:");
    errmsg+=newp.toString();
    throw std::runtime_error(errmsg);
  }
  else{
    m_board[newp.m_x][newp.m_y]=m_board[oldp.m_x][oldp.m_y];
    m_board[oldp.m_x][oldp.m_y]=Square::Square();
  }
}