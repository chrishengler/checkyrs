//
//  board.cpp
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <string>
#include <math.h>
#include "board.h"

Board::Board(const int &size){
  
  m_size=size;
  
  m_board.resize(size);
  for(int ii=0;ii<size;ii++){
    m_board[ii].resize(size);
    for(int jj=0;jj<size;jj++){
      m_board[ii][jj] = Square::Square();
    }
  }
}

bool Board::PositionExists(const Position &p){
  if(p._x < 0 || p._x >=m_size || p._y < 0 || p._y >= m_size){
    return false;
  }
  else return true;
}


Position Board::getJump(const Position &p1, const Position &p2){
  if(!PositionExists(p1)){
    std::string errmsg("Position does not exist: ");
    errmsg+=p1.toString();
    throw std::logic_error(errmsg);
  }
  else if(!PositionExists(p2)){
    std::string errmsg("Position does not exist: ");
    errmsg+=p2.toString();
    throw std::logic_error(errmsg);
  }
  else if(p1._x%2 != p2._x%2 || p1._y%2 != p2._y%2){
    std::string errmsg("jump is not valid: ");
    errmsg+=p1.toString(); errmsg+=" to "; errmsg+=p2.toString();
    throw std::runtime_error(errmsg);
  }
  return Position( (p1._x+p2._x)/2 , (p1._y+p2._y)/2 );
}


bool Board::SquareIsOccupied(const Position &p){
  int x = p._x;
  int y = p._y;
  if(!PositionExists(p)){
    std::string errmsg("Trying to check occupation of invalid position:");
    errmsg+=p.toString();
    throw std::out_of_range(errmsg);
  }
  else return (m_board[x][y].isOccupied());
}

bool Board::SquareHasKing(const Position &p){
  if(!PositionExists(p)){
    std::string errmsg("Checking kinghood of non-existent position:");
    errmsg+=p.toString();
    throw std::out_of_range(errmsg);
  }
  if(!SquareIsOccupied(p)){
    std::string errmsg("Checking kinghood of empty square:");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }
  return m_board[p._x][p._y].isKing();
}


void Board::AddPiece(const Position &pos,const int &player, const bool &isKing){
  int x = pos._x;
  int y = pos._y;
  if(!PositionExists(pos) || !PositionExists(pos)){
    std::string errmsg("Attempted to add piece out of bounds:");
    errmsg+=pos.toString();
    throw std::out_of_range(errmsg);
  }
  else if(SquareIsOccupied(pos)){
    std::string errmsg("Square is occupied:");
    errmsg+=pos.toString();
    throw std::runtime_error(errmsg);
  }
  else{
    Square newpiece(player,isKing);
    m_board[x][y] = newpiece;
  }
}

void Board::MovePiece(const Position &oldp, const Position &newp){
  int oldx = oldp._x;
  int oldy = oldp._y;
  int newx = newp._x;
  int newy = newp._y;
  if(!(SquareIsOccupied(oldp))){
    std::string errmsg("Tried to move non-existent piece:");
    errmsg+=oldp.toString();
    throw std::runtime_error(errmsg);
  }
  else if(SquareIsOccupied(newp)){
    std::string errmsg("Tried to move to occupied square:");
    errmsg+=newp.toString();
    throw std::runtime_error(errmsg);
  }
  else{
    m_board[newx][newy]=m_board[oldx][oldy];
    m_board[oldx][oldy]=Square::Square();
  }
}

Square Board::getPiece(const Position &p){
  if(!PositionExists(p)){
    std::string errmsg("Tried to get piece from non-existent square");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }
  if(!SquareIsOccupied(p)){
    std::string errmsg("Tried to get piece from empty square");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }
  else return m_board[p._x][p._y];
}

void Board::RemovePiece(const Position &p){
  if(!PositionExists(p)){
    std::string errmsg("Tried to remove piece from non-existent square");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }
  if(!SquareIsOccupied(p)){
    std::string errmsg("Tried to remove piece from empty square");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }
  else m_board[p._x][p._y].removePiece();
}

