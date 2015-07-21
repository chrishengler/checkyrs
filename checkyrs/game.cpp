//
//  game.cpp
//  checkyrs
//
//  Created by Chris Hengler on 19/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include "game.h"
#include <math.h>

void Game::AddPiece(const Position &pos, const int &player, const bool &isKing){
  m_board.AddPiece(pos,player,isKing);
}

void Game::MovePiece(const Position &oldp, const Position &newp){
  m_board.MovePiece(oldp, newp);
}

std::vector<std::vector<Position> > Game::getMovesFrom(const Position &p, const bool &alreadyMoved){
  std::vector<std::vector<Position> > possibleMoves;
  
  std::vector<Position> moveStarts = getJumpsFrom(p);
  if(moveStarts.size()>0){
    possibleMoves.push_back(moveStarts);
  }
  else{
    std::vector<Position> single=getSingleMovesFrom(p);
    if(single.size()>0) possibleMoves.push_back(single);
  }
  
  return possibleMoves;
}

std::vector<Position> Game::getSingleMovesFrom(const Position &p){
  std::vector<Position> possibleMoves;
  for(int ii=-1;ii<=1;ii+=2){
    if(ii<0 && !m_board.SquareHasKing(p)) continue;
    for(int jj=-1;jj<=1;jj+=2){
      Position newpos(p._x+ii,p._y+jj);
      if(!m_board.PositionExists(newpos)) continue;
      if(m_board.SquareIsOccupied(newpos)) continue;
      else possibleMoves.push_back(newpos);
    }
  }
  return possibleMoves;
}

std::vector<Position> Game::getJumpsFrom(const Position &p){
  std::vector<Position> possibleMoves;
  for(int ii=-1;ii<=1;ii+=2){
    if(ii<0 && !m_board.SquareHasKing(p)) continue;
    for(int jj=-1;jj<=1;jj+=2){
      Position newpos(p._x+ii,p._y+jj); //the square to jump over
      if(!m_board.PositionExists(newpos)) continue;
      else if(m_board.SquareIsOccupied(newpos)){ //can't jump over empty square
        if(m_board.getPiece(newpos).getPlayer()==m_board.getPiece(p).getPlayer()) continue;
        newpos=Position(p._x+(2*ii),p._y+(2*jj));
        if(!m_board.PositionExists(newpos)) continue;
        if(m_board.SquareIsOccupied(newpos)) continue;
        possibleMoves.push_back(newpos);
      }
    }
  }
  return possibleMoves;
}

