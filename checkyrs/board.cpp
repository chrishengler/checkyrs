//
//  board.cpp
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <string>
#include "board.h"

Board::Board(int size){
  
  m_size=size;
  
  m_board.resize(size);
  for(int ii=0;ii<size;ii++){
    m_board[ii].resize(size);
    for(int jj=0;jj<size;jj++){
      m_board[ii][jj] = NULL;
    }
  }
}

bool Board::isValidPosition(const Position &p){
  if(p._x < 0 || p._x >=m_size || p._y < 0 || p._y >= m_size){
    return false;
  }
  else return true;
}

bool Board::SquareIsOccupied(const Position &p){
  int x = p._x;
  int y = p._y;
  if(!isValidPosition(p)){
    std::string errmsg("Trying to check occupation of invalid position:");
    errmsg+=p.toString();
    throw std::out_of_range(errmsg);
  }
  else return (m_board[x][y]!=NULL);
}

void Board::AddPiece(const Position &p){
  int x = p._x;
  int y = p._y;
  if(!isValidPosition(p) || !isValidPosition(p)){
    std::string errmsg("Attempted to add piece out of bounds:");
    errmsg+=p.toString();
    throw std::out_of_range(errmsg);
  }
  else if(this->SquareIsOccupied(p)){
    std::string errmsg("Square is occupied:");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }
  else{
    m_board[x][y] = new Piece();
  }
}

void Board::MovePiece(const Position &oldp, const Position &newp){
  int oldx = oldp._x;
  int oldy = oldp._y;
  int newx = newp._x;
  int newy = newp._y;
  if(!(this->SquareIsOccupied(oldp))){
    std::string errmsg("Tried to move non-existent piece:");
    errmsg+=oldp.toString();
    throw std::runtime_error(errmsg);
  }
  else if(this->SquareIsOccupied(newp)){
    std::string errmsg("Tried to move to occupied square:");
    errmsg+=newp.toString();
    throw std::runtime_error(errmsg);
  }
  else{
    m_board[newx][newy]=m_board[oldx][oldy];
    m_board[oldx][oldy]=NULL;
  }
}
