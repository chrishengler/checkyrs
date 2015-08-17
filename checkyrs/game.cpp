//
//  game.cpp
//  checkyrs
//
//  Created by Chris Hengler on 19/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <iostream>
#include <math.h>

#include "game.h"

void Game::AddPiece(const Position &pos, const int player, const bool isKing){
  m_board.AddPiece(pos,player,isKing);
  
}

void Game::AddPieces(const std::vector<Position> &pos, const int player, const bool isKing){
  m_board.AddPieces(pos,player,isKing);
}

void Game::AddPieces(const std::vector<Position> &p1, const std::vector<Position> &p2, const bool isKing){
  m_board.AddPieces(p1,1,isKing);
  m_board.AddPieces(p2,-1,isKing);
}

void Game::MovePiece(const Position &oldp, const Position &newp){
  m_board.MovePiece(oldp, newp);
}

void Game::RemovePiece(const Position &p){
  m_board.RemovePiece(p);
}

void Game::PrepareBoard(){
  std::vector<Position> p1, p2;
  for(int ii=0;ii<m_board.getSize();ii++){
    for(int jj=0;jj<3;jj++){
      if(ii%2==jj%2){
        p1.push_back(Position(ii,jj));
        p2.push_back(Position(m_board.getSize()-(ii+1),m_board.getSize()-(jj+1)));
      }
    }
  }
  AddPieces(p1);
  AddPieces(p2,-1);
}

bool Game::PieceIsThreatened(const Position &p) const{
  try{
    if(!m_board.SquareIsOccupied(p)) return false;
    for(int yy=-1;yy<2;yy+=2){
      for(int xx=-1;xx<2;xx+=2){
        Position testpos(p._x+xx, p._y+yy);
        if(!m_board.SquareExists(testpos)) continue;
        if(!m_board.SquareIsOccupied(testpos)) continue;
        else if(m_board.getPlayer(testpos)!=m_board.getPlayer(p)){
          if(yy/m_board.getPlayer(testpos) == -1 || m_board.SquareHasKing(testpos)){
            Position jumptarget(p._x-xx,p._y-yy);
            if(!m_board.SquareExists(jumptarget)) continue;
            if(!m_board.SquareIsOccupied(jumptarget)) return true;
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

int Game::PieceDefence(const Position &p) const{
  try{
    int def=0;
    if(!m_board.SquareIsOccupied(p)) return false;
    for(int yy=-1;yy<2;yy+=2){
      for(int xx=-1;xx<2;xx+=2){
        Position testpos(p._x+xx, p._y+yy);
        if(!m_board.SquareExists(testpos)) continue;
        if(!m_board.SquareIsOccupied(testpos)) continue;
        else if(m_board.getPlayer(testpos)==m_board.getPlayer(p)){
          def++;
        }
      }
    }
    return def;
  }
  catch(std::exception &e){
    std::cout << "unexpected exception when checking if square " << p.toString() << " is defended\n" << e.what();
    throw e;
  }
}

bool Game::PieceCanCrown(const Position &p) const{
  try{
    if(!m_board.SquareExists(p)) return false;
    if(!m_board.SquareIsOccupied(p)) return false;
    if(!m_board.SquareHasKing(p)) return false;
    std::vector<std::vector<Position> > moves = getMovesFrom(p);
    for(int ii=0;ii<moves.size();ii++){
      for(int jj=0;jj<moves.at(ii).size();jj++){
        if(moves.at(ii).at(jj)._y == (m_board.getPlayer(p) == 1 ? m_board.getSize()-1 : 0)) return true;
      }
    }
  }
  catch(std::exception &e){
    std::cout << "unexpected exception when checking if square " << p.toString() << " can be crowned\n" << e.what();
  }
  return false;
}

std::vector<std::vector<Position> > Game::getMovesFrom(const Position &p, const bool alreadyMoved) const{
  std::vector<std::vector<Position> > possibleMoves;
  
  std::vector<Position> moveStarts = getJumpsFrom(p);
  if(moveStarts.size()>0){
    for(int ii=0;ii<moveStarts.size();ii++){
      std::vector<Position> thismove;
      thismove.push_back(p);
      thismove.push_back(moveStarts.at(ii));
      Game newstate(*this);
      newstate.ExecuteMove(thismove);
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

std::vector<std::vector<Position> > Game::ExtendMove(const std::vector<Position> &p) const{
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


std::vector<Position> Game::getSingleMovesFrom(const Position &p) const{
  std::vector<Position> possibleMoves;
  for(int ii=-1;ii<=1;ii+=2){
    for(int jj=-1;jj<=1;jj+=2){
      if( (jj*m_board.getPlayer(p))<0 && !m_board.SquareHasKing(p)) continue;
      Position newpos(p._x+ii,p._y+jj);
      if(!m_board.SquareExists(newpos)) continue;
      if(m_board.SquareIsOccupied(newpos)) continue;
      else possibleMoves.push_back(newpos);
    }
  }
  return possibleMoves;
}

std::vector<Position> Game::getJumpsFrom(const Position &p) const{
  std::vector<Position> possibleMoves;
  for(int ii=-1;ii<=1;ii+=2){
    for(int jj=-1;jj<=1;jj+=2){
      if( (jj*m_board.getPlayer(p))<0 && !m_board.SquareHasKing(p)) continue;
      Position newpos(p._x+ii,p._y+jj); //the square to jump over
      if(!m_board.SquareExists(newpos)) continue;
      else if(m_board.SquareIsOccupied(newpos)){ //can't jump over empty square
        if(m_board.getSquare(newpos).getPlayer()==m_board.getSquare(p).getPlayer()) continue;
        newpos=Position(p._x+(2*ii),p._y+(2*jj));
        if(!m_board.SquareExists(newpos)) continue;
        if(m_board.SquareIsOccupied(newpos)) continue;
        possibleMoves.push_back(newpos);
      }
    }
  }
  return possibleMoves;
}

std::vector<std::vector<Position> > Game::getMovesForPlayer(const int player) const{
  std::vector<std::vector<Position> > possibleMoves;
  bool jumpFound = false;
  for(int ii=0;ii<m_board.getSize();ii++){
    for(int jj=0;jj<m_board.getSize();jj++){
      Position p(ii,jj);
      if(m_board.SquareIsOccupied(p) && m_board.getPlayer(p)==player){
        std::vector<std::vector<Position> > thispiece = getMovesFrom(p);
        if(thispiece.size()==0) continue;
        if(jumpFound){
          if(m_board.wasJump(thispiece.at(0).at(0),thispiece.at(0).at(1))){
            possibleMoves.insert(possibleMoves.end(),thispiece.begin(),thispiece.end());
          }
        }
        else{
          if(m_board.wasJump(thispiece.at(0).at(0),thispiece.at(0).at(1))){
            possibleMoves.clear();
            jumpFound = true;
          }
          possibleMoves.insert(possibleMoves.end(),thispiece.begin(),thispiece.end());
        }
      }
    }
  }
  if(possibleMoves.size()==0){
    m_gameover=true;  //if player has no legal moves, opponent has won
    m_winner = player*-1;
  }
  return possibleMoves;
}

std::vector<Position> Game::getJumpedSquares(const std::vector<Position> &p) const{
  std::vector<Position> jumped;
  for(int ii=0;ii<p.size()-1;ii++){
    jumped.push_back(m_board.getJump(p.at(ii), p.at(ii+1)));
  }
  return jumped;
}

void Game::ExecuteMove(const std::vector<Position> &move){
  bool stale=true;
  for(int ii=0;ii<move.size()-1;ii++){
    if(fabs(move.at(ii)._y - move.at(ii+1)._y) != 1){
      RemovePiece(m_board.getJump(move.at(ii),move.at(ii+1)));
      stale = false;
    }
    MovePiece(move.at(ii), move.at(ii+1));
    if( !m_board.SquareHasKing(move.at(ii+1)) ){
      if( m_currentplayer == 1 ){
        if(move.at(ii+1)._y == m_board.getSize()-1){
          m_board.setKing(move.at(ii+1));
          stale = false;
        }
      }
      else{
        if(move.at(ii+1)._y == 0){
          m_board.setKing(move.at(ii+1));
          stale = false;
        }
      }
    }
  }
  if(getNumPiecesPlayer(1)==0){
    m_gameover = true;
    m_winner = -1;
  }
  else if(getNumPiecesPlayer(-1)==0){
    m_gameover = true;
    m_winner = 1;
  }
  m_currentplayer *= -1;
  if(getMovesForPlayer(m_currentplayer).size()==0){
    m_gameover = true;
    m_winner = -1*m_currentplayer;
  }
  if(!stale){
    m_staleness=0;
  }
  else if(!m_gameover){
    m_staleness++;
  }
  if(m_staleness>=m_maxstaleness){
    m_gameover = true;
    m_stale = true;
  }
}

