//
//  board.cpp
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <iostream>
#include <string>
#include <math.h>
#include "draughtsboard.h"

/**
 *  Constructor for a DraughtsBoard
 *  
 *  DraughtsBoards are always square
 *
 *  @param size number of columns & rows for this DraughtsBoard
 */
DraughtsBoard::DraughtsBoard(const int &size) : Board(size){
  
  m_kingsp1=0;
  m_kingsp2=0;

}

/**
 *  Copy constructor
 *
 *  @param board the DraughtsBoard to copy
 */
DraughtsBoard::DraughtsBoard(const DraughtsBoard &board) : Board(board){
  m_size=board.m_size;
  m_kingsp1=board.m_kingsp1;
  m_kingsp2=board.m_kingsp2;
}



/**
 *  Check if a move was a jump
 *
 *  @param p1 starting point
 *  @param p2 ending point
 *  @return true if move requires jumping
 */
bool DraughtsBoard::wasJump(const Position &p1, const Position &p2) const{
  return ( fabs(p1.m_x - p2.m_x) == 2 && fabs(p1.m_y - p2.m_y) == 2 );
}

/**
 *  Get the Position of a square jumped over
 *
 *  Checks that positions are valid and that move is a legitimate jump,
 *  then returns the square which was jumped over.
 *
 *  @param p1 starting point of move
 *  @param p2 ending point of move
 *  @return Position of square jumped over
 */
Position DraughtsBoard::getJump(const Position &p1, const Position &p2) const{
  if(!squareExists(p1)){
    std::string errmsg("Position does not exist: ");
    errmsg+=p1.toString();
    throw std::logic_error(errmsg);
  }
  else if(!squareExists(p2)){
    std::string errmsg("Position does not exist: ");
    errmsg+=p2.toString();
    throw std::logic_error(errmsg);
  }
  else if(!wasJump(p1,p2)){
    std::string errmsg("jump is not valid: ");
    errmsg+=p1.toString(); errmsg+=" to "; errmsg+=p2.toString();
    throw std::runtime_error(errmsg);
  }
  return Position( (p1.m_x+p2.m_x)/2 , (p1.m_y+p2.m_y)/2 );
}

/**
 *  Set Kingness of piece on this Position
 *
 *  @param p Position of piece
 *  @param isKing should the piece be a king?
 */
void DraughtsBoard::setKing(const Position &p, const bool isKing){
  if(!squareExists(p)){
    std::string errmsg("Position does not exist: ");
    errmsg+=p.toString();
    throw std::logic_error(errmsg);
  }
  if(isKing == squareHasKing(p)){
    return;
  }
  m_board[p.m_x][p.m_y].setKing();
  if(isKing){
    getPlayer(p) == 1 ? m_kingsp1++ : m_kingsp2++;
  }
  else{
    getPlayer(p) == 1 ? m_kingsp1-- : m_kingsp2--;
  }
  return;
}



/**
 *  Check if a square has a king on it
 *
 *  @param p the Position to check
 *  @return true if is occupied by a king
 */
bool DraughtsBoard::squareHasKing(const Position &p) const{
  if(!squareExists(p)){
    std::string errmsg("Checking kinghood of non-existent position:");
    errmsg+=p.toString();
    throw std::out_of_range(errmsg);
  }
  if(!squareIsOccupied(p)){
    std::string errmsg("Checking kinghood of empty square:");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }
  return m_board[p.m_x][p.m_y].isKing();
}


/**
 *  Add a piece to a square
 *
 *  @throw std::out_of_range if not a valid Position
 *  @throw std::runtime_error if square is already occupied
 *  @param pos where to add the piece
 *  @param player which player this piece should belong to
 *  @param isKing whether the piece is a king
 */
void DraughtsBoard::addPiece(const Position &pos,const int player, const int &type){
  if(!squareExists(pos) || !squareExists(pos)){
    std::string errmsg("Attempted to add piece out of bounds:");
    errmsg+=pos.toString();
    throw std::out_of_range(errmsg);
  }
  else if(squareIsOccupied(pos)){
    std::string errmsg("square is occupied:");
    errmsg+=pos.toString();
    throw std::runtime_error(errmsg);
  }
  Square newpiece(player,type);
  m_board[pos.m_x][pos.m_y] = newpiece;
  player==1 ? m_piecesp1++ : m_piecesp2++;
  if(type) player==1 ? m_kingsp1++ : m_kingsp2++;
}

/**
 *  Add several pieces
 *
 *  Player and Kingness the same for all added pieces
 *
 *  @param p vector of Positions to add
 *  @param player which player these pieces belong to
 *  @param isKing whether these pieces are kings
 */
void DraughtsBoard::addPieces(const std::vector<Position> &p,const int player, const int &type){
  for(auto pos: p){
    addPiece(pos,player,type);
  }
}


/**
 *  Move a piece
 *
 *  @throw std::runtime_error if no such piece exists
 *  @throw std::runtime_error if destination is already occupied
 *  @param oldp starting position
 *  @param newp new position
 */
void DraughtsBoard::movePiece(const Position &oldp, const Position &newp){
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


/**
 *  Remove a piece from the DraughtsBoard
 *
 *  @throw std::runtime_error if square does not exist
 *  @throw std::runtime_error if no piece to remove
 *  @param p the Position to remove a piece from
 */
void DraughtsBoard::removePiece(const Position &p){
  if(!squareExists(p)){
    std::string errmsg("Tried to remove piece from non-existent square");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }
  if(!squareIsOccupied(p)){
    std::string errmsg("Tried to remove piece from empty square");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }
  getPlayer(p) == 1 ? m_piecesp1-- : m_piecesp2--;
  if(squareHasKing(p)){
    getPlayer(p) == 1 ? m_kingsp1-- : m_kingsp2--;
  }
  m_board[p.m_x][p.m_y].removePiece();
}

