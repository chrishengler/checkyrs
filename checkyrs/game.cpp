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

void Game::RemovePiece(const Position &p){
  m_board.RemovePiece(p);
}

std::vector<std::vector<Position> > Game::getMovesFrom(const Position &p, const bool &alreadyMoved){
  std::vector<std::vector<Position> > possibleMoves;
  
  std::vector<Position> moveStarts = getJumpsFrom(p);
  if(moveStarts.size()>0){
    for(int ii=0;ii<moveStarts.size();ii++){
      std::vector<Position> thismove;
      thismove.push_back(p);
      thismove.push_back(moveStarts.at(ii));
      Game newstate(*this);
      newstate.RemovePiece(m_board.getJump(p,moveStarts.at(ii)));
      newstate.MovePiece(p,moveStarts.at(ii));
      std::vector<std::vector<Position> > extendedMoves = newstate.ExtendMove(thismove);
      possibleMoves.insert(possibleMoves.end(), extendedMoves.begin(), extendedMoves.end());
    }
  }
  else{
    std::vector<Position> single=getSingleMovesFrom(p);
    for(int ii=0;ii<single.size();ii++){
      std::vector<Position> thismove;
      thismove.push_back(p);
      thismove.push_back(single.at(ii));
      possibleMoves.push_back(thismove);
    }
  }
  
  return possibleMoves;
}

std::vector<std::vector<Position> > Game::ExtendMove(const std::vector<Position> &p){
  std::vector<std::vector<Position> > possibleMoves;
  std::vector<Position> extensions = getJumpsFrom(p.back());
  if(extensions.size()==0){
    possibleMoves.push_back(p);
    return possibleMoves;
  }
  for(int ii=0;ii<extensions.size();ii++){
    std::vector<Position> extended = p;
    extended.push_back(extensions.at(ii));
    Game newstate(*this);
    newstate.RemovePiece(m_board.getJump(p.back(),extensions.at(ii)));
    newstate.MovePiece(p.back(),extensions.at(ii));
    std::vector<std::vector<Position> > nextstep = newstate.ExtendMove(extended);
    possibleMoves.insert(possibleMoves.end(), nextstep.begin(), nextstep.end());
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

std::vector<Position> Game::getJumpedSquares(const std::vector<Position> &p){
  std::vector<Position> jumped;
  for(int ii=0;ii<p.size()-1;ii++){
    jumped.push_back(m_board.getJump(p.at(ii), p.at(ii+1)));
  }
  return jumped;
}


