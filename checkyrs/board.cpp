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

Board::Board(int size){
  
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

std::vector<Position> Board::getMovesFrom(const Position &p){
  std::vector<Position> possibleMoves;
  for(int ii=-2;ii<=2;ii++){
    if(ii==0) continue;
    for(int jj = -1*fabs(ii); jj<=(fabs(ii)); jj++){
      if( (jj<0 || fabs(jj)>1) && !SquareHasKing(p)){
        continue;
      }
      if(fabs(jj)!=fabs(ii)) continue;
      Position p(ii,jj);
      possibleMoves.push_back(p);
    }
  }
  for(std::vector<Position>::iterator pos=possibleMoves.begin();pos!=possibleMoves.end();){
    //no auto increment in for loop because vector.erase() already returns iterator pointing to next element
    if(!PositionExists(*pos) || SquareIsOccupied(*pos)){
      //printf("erasing %s from movelist\n",(*pos).toString().c_str());
      pos = possibleMoves.erase(pos);
      continue;
    }
    else pos++;
  }
  //printf("accepted moves:\n");
  for(std::vector<Position>::iterator pos=possibleMoves.begin();pos!=possibleMoves.end();pos++){
    //printf("%s\n",(*pos).toString().c_str());
  }
  return possibleMoves;
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
