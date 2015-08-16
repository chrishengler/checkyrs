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
#include "board.h"

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

bool Board::SquareExists(const Position &p) const{
  return (p._x >= 0 && p._x < m_size && p._y >=0 && p._y < m_size);
}

bool Board::wasJump(const Position &p1, const Position &p2) const{
  return ( fabs(p1._x - p2._x) == 2 && fabs(p1._y - p2._y) == 2 );
}

Position Board::getJump(const Position &p1, const Position &p2) const{
  if(!SquareExists(p1)){
    std::string errmsg("Position does not exist: ");
    errmsg+=p1.toString();
    throw std::logic_error(errmsg);
  }
  else if(!SquareExists(p2)){
    std::string errmsg("Position does not exist: ");
    errmsg+=p2.toString();
    throw std::logic_error(errmsg);
  }
  else if(!wasJump(p1,p2)){
    std::string errmsg("jump is not valid: ");
    errmsg+=p1.toString(); errmsg+=" to "; errmsg+=p2.toString();
    throw std::runtime_error(errmsg);
  }
  return Position( (p1._x+p2._x)/2 , (p1._y+p2._y)/2 );
}


bool Board::SquareIsOccupied(const Position &p) const{
  if(!SquareExists(p)){
    std::string errmsg("Trying to check occupation of invalid position:");
    errmsg+=p.toString();
    throw std::out_of_range(errmsg);
  }
  else return (m_board[p._x][p._y].isOccupied());
}

bool Board::SquareHasKing(const Position &p) const{
  if(!SquareExists(p)){
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

bool Board::SquareIsThreatened(const Position &p) const{
  try{
    if(!SquareIsOccupied(p)) return false;
    for(int yy=-1;yy<2;yy+=2){
      for(int xx=-1;xx<2;xx+=2){
        Position testpos(p._x+xx, p._y+yy);
        if(!SquareExists(testpos)) continue;
        if(!SquareIsOccupied(testpos)) continue;
        else if(getPlayer(testpos)!=getPlayer(p)){
          if(yy/getPlayer(testpos) == 1){
            Position jumptarget(p._x-xx,p._y-yy);
            if(!SquareExists(jumptarget)) continue;
            if(!SquareIsOccupied(jumptarget)) return true;
          }
        }
      }
    }
    return false;
  }
  catch(std::exception &e){
    std::cout << "unexpected exception when checking if square " << p.toString() << " is threatened\n" << e.what();
    throw e;
  }
}

int Board::getPlayer(const Position &p) const{
  if(!SquareExists(p)){
    std::string errmsg("Checking owner of non-existent position:");
    errmsg+=p.toString();
    throw std::out_of_range(errmsg);
  }
  if(!SquareIsOccupied(p)){
    std::string errmsg("Checking owner of empty square:");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }
  return m_board[p._x][p._y].getPlayer();
}

void Board::AddPiece(const Position &pos,const int player, const bool isKing){
  if(!SquareExists(pos) || !SquareExists(pos)){
    std::string errmsg("Attempted to add piece out of bounds:");
    errmsg+=pos.toString();
    throw std::out_of_range(errmsg);
  }
  else if(SquareIsOccupied(pos)){
    std::string errmsg("Square is occupied:");
    errmsg+=pos.toString();
    throw std::runtime_error(errmsg);
  }
  Square newpiece(player,isKing);
  m_board[pos._x][pos._y] = newpiece;
  player==1 ? m_piecesp1++ : m_piecesp2++;
}

void Board::AddPieces(const std::vector<Position> &p,const int player, const bool isKing){
  for(std::vector<Position>::const_iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
    AddPiece(*p_iter,player,isKing);
  }
}

void Board::MovePiece(const Position &oldp, const Position &newp){
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
    m_board[newp._x][newp._y]=m_board[oldp._x][oldp._y];
    m_board[oldp._x][oldp._y]=Square::Square();
  }
}

Square Board::getSquare(const Position &p) const{
  if(!SquareExists(p)){
    std::string errmsg("Tried to get piece from non-existent square");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }
  /*if(!SquareIsOccupied(p)){
    std::string errmsg("Tried to get piece from empty square");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }*/
  return m_board[p._x][p._y];
}

void Board::RemovePiece(const Position &p){
  if(!SquareExists(p)){
    std::string errmsg("Tried to remove piece from non-existent square");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }
  if(!SquareIsOccupied(p)){
    std::string errmsg("Tried to remove piece from empty square");
    errmsg+=p.toString();
    throw std::runtime_error(errmsg);
  }
  getPlayer(p) == 1 ? m_piecesp1-- : m_piecesp2--;
  m_board[p._x][p._y].removePiece();
}

