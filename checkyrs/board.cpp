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
  m_kingsp1=0;
  m_kingsp2=0;
  
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
  m_kingsp1=board.m_kingsp1;
  m_kingsp2=board.m_kingsp2;
  
  m_board.resize(m_size);
  for(int ii=0;ii<m_size;ii++){
    m_board[ii].resize(m_size);
    for(int jj=0;jj<m_size;jj++){
      m_board[ii][jj] = Square::Square(board.m_board[ii][jj]);
    }
  }
}

bool Board::squareExists(const Position &p) const{
  return (p.m_x >= 0 && p.m_x < m_size && p.m_y >=0 && p.m_y < m_size);
}

bool Board::wasJump(const Position &p1, const Position &p2) const{
  return ( fabs(p1.m_x - p2.m_x) == 2 && fabs(p1.m_y - p2.m_y) == 2 );
}

Position Board::getJump(const Position &p1, const Position &p2) const{
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

void Board::setKing(const Position &p, const bool isKing){
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

bool Board::squareIsOccupied(const Position &p) const{
  if(!squareExists(p)){
    std::string errmsg("Trying to check occupation of invalid position:");
    errmsg+=p.toString();
    throw std::out_of_range(errmsg);
  }
  else return (m_board[p.m_x][p.m_y].isOccupied());
}

bool Board::squareHasKing(const Position &p) const{
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

int Board::distanceToEdge(const Position &p) const{
  int lateral = distanceToSide(p);
  int longitudinal = distanceToEnd(p);
  return ( lateral>longitudinal ? longitudinal : lateral );
}

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

void Board::addPiece(const Position &pos,const int player, const bool isKing){
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
  Square newpiece(player,isKing);
  m_board[pos.m_x][pos.m_y] = newpiece;
  player==1 ? m_piecesp1++ : m_piecesp2++;
  if(isKing) player==1 ? m_kingsp1++ : m_kingsp2++;
}

void Board::addPieces(const std::vector<Position> &p,const int player, const bool isKing){
  for(std::vector<Position>::const_iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
    addPiece(*p_iter,player,isKing);
  }
}

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

void Board::removePiece(const Position &p){
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

