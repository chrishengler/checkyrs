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

/**
 *  Sort function to order moves by number of pieces taken
 *
 *  @param lhs first move to compare
 *  @param rhs second move to compare
 *  @return true if first move captures more pieces than second move
 */
bool sortMovesByLength(const std::vector<Position> &lhs, const std::vector<Position> &rhs){
  if(lhs.size()>rhs.size()) return true;
  if(rhs.size()>rhs.size()) return false;
  else return ( fabs(lhs.at(0).m_x - lhs.at(1).m_x) > 1 );
}

/**
 *  Prepare the board
 *
 *  Adds all of the pieces for each player
 */
void Game::prepareBoard(){
  std::vector<Position> p1, p2;
  for(int ii=0;ii<m_board.getSize();ii++){
    for(int jj=0;jj<3;jj++){
      if(ii%2==jj%2){
        p1.push_back(Position(ii,jj));
        p2.push_back(Position(m_board.getSize()-(ii+1),m_board.getSize()-(jj+1)));
      }
    }
  }
  m_board.addPieces(p1);
  m_board.addPieces(p2,-1);
}

/**
 *  Decide if a piece is under threat
 *
 *  A piece is considered threatened if there is an opposing piece that can take it.
 *  Returns false if no piece exists at this location.
 *
 *  @param p the location to test
 *  @return true if threatened
 */
bool Game::pieceIsThreatened(const Position &p) const{
  try{
    if(!m_board.squareIsOccupied(p)) return false;
    for(int yy=-1;yy<2;yy+=2){
      for(int xx=-1;xx<2;xx+=2){
        Position testpos(p.m_x+xx, p.m_y+yy);
        if(!m_board.squareExists(testpos)) continue;
        if(!m_board.squareIsOccupied(testpos)) continue;
        else if(m_board.getPlayer(testpos)!=m_board.getPlayer(p)){
          if(yy/m_board.getPlayer(testpos) == -1 || m_board.squareHasKing(testpos)){
            Position jumptarget(p.m_x-xx,p.m_y-yy);
            if(!m_board.squareExists(jumptarget)) continue;
            if(!m_board.squareIsOccupied(jumptarget)) return true;
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

/**
 *  Piece defence value
 *
 *  Piece defence counts the number of surrounding friendly pieces.
 *  Returns 0 if no piece at this location
 *
 *  @param p the Position to test
 *  @return the number of friendly pieces surrounding the piece at p
 */
int Game::pieceDefence(const Position &p) const{
  try{
    int def=0;
    if(!m_board.squareIsOccupied(p)) return false;
    for(int yy=-1;yy<2;yy+=2){
      for(int xx=-1;xx<2;xx+=2){
        Position testpos(p.m_x+xx, p.m_y+yy);
        if(!m_board.squareExists(testpos)){
          def++;
          continue;
        }
        if(!m_board.squareIsOccupied(testpos)) continue;
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

/**
 *  Can a piece be promoted to king on its next move?
 *
 *  @param p the Position to test
 *  @return true if this piece can be promoted on its next move
 */
bool Game::pieceCanCrown(const Position &p) const{
  try{
    if(!m_board.squareExists(p)) return false;
    if(!m_board.squareIsOccupied(p)) return false;
    if(!m_board.squareHasKing(p)) return false;
    std::vector<std::vector<Position> > moves = getMovesFrom(p);
    for(int ii=0;ii<moves.size();ii++){
      for(int jj=0;jj<moves.at(ii).size();jj++){
        if(moves.at(ii).at(jj).m_y == (m_board.getPlayer(p) == 1 ? m_board.getSize()-1 : 0)) return true;
      }
    }
  }
  catch(std::exception &e){
    std::cout << "unexpected exception when checking if square " << p.toString() << " can be crowned\n" << e.what();
  }
  return false;
}

/**
 *  Whether this piece can capture another on next move
 *
 *  @param p the Position to test
 *  @return true if the piece can capture another
 */
bool Game::pieceCanCapture(const Position &p) const{
  try{
    if(!m_board.squareExists(p)) return false;
    if(!m_board.squareIsOccupied(p)) return false;
    std::vector<std::vector<Position> > moves = getMovesFrom(p);
    if(moves.size()!=0 && fabs(moves.at(0).at(0).m_y - moves.at(0).at(1).m_y)!=1) return true;
    else return false;
  }
  catch(std::exception &e){
    std::cout << "unexpected exception when checking if square " << p.toString() << " can be crowned\n" << e.what();
  }
  return false;
}

/**
 *  Get all possible moves for this piece
 *
 *  When taking a piece, calls extendMove to see if move can be extended with further captures.
 *
 *  @param p Position of the piece to move
 *  @return vector of all possible moves
 */
std::vector<std::vector<Position> > Game::getMovesFrom(const Position &p) const{
  std::vector<std::vector<Position> > possibleMoves;
  
  std::vector<Position> moveStarts = getJumpsFrom(p);
  if(moveStarts.size()>0){
    for(int ii=0;ii<moveStarts.size();ii++){
      std::vector<Position> thismove;
      thismove.push_back(p);
      thismove.push_back(moveStarts.at(ii));
      Game newstate(*this);
      newstate.executeMove(thismove);
      std::vector<std::vector<Position> > extendedMoves = newstate.extendMove(thismove);
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

/**
 *  Extend a move
 *
 *  After capturing, test to see if the move can be extended with further captures.
 *
 *  @param p the current Position of the piece being moved
 *  @return vector of possible move extensions
 */
std::vector<std::vector<Position> > Game::extendMove(const std::vector<Position> &p) const{
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
    newstate.m_board.removePiece(m_board.getJump(p.back(),extensions.at(ii)));
    newstate.m_board.movePiece(p.back(),extensions.at(ii));
    std::vector<std::vector<Position> > nextstep = newstate.extendMove(extended);
    possibleMoves.insert(possibleMoves.end(), nextstep.begin(), nextstep.end());
  }
  
  return possibleMoves;
}

/**
 *  Get single moves (i.e. no capture) from a position on the board
 *
 *  @param p the Position to test
 *  @return the positions to which that piece can move without jumping
 */
std::vector<Position> Game::getSingleMovesFrom(const Position &p) const{
  std::vector<Position> possibleMoves;
  for(int ii=-1;ii<=1;ii+=2){
    for(int jj=-1;jj<=1;jj+=2){
      if( (jj*m_board.getPlayer(p))<0 && !m_board.squareHasKing(p)) continue;
      Position newpos(p.m_x+ii,p.m_y+jj);
      if(!m_board.squareExists(newpos)) continue;
      if(m_board.squareIsOccupied(newpos)) continue;
      else possibleMoves.push_back(newpos);
    }
  }
  return possibleMoves;
}

/**
 *  Get jump moves (i.e. capture) from a position on the board
 *
 *  @param p the Position to test
 *  @return the positions to which that piece can move by capturing
 */
std::vector<Position> Game::getJumpsFrom(const Position &p) const{
  std::vector<Position> possibleMoves;
  for(int ii=-1;ii<=1;ii+=2){
    for(int jj=-1;jj<=1;jj+=2){
      if( (jj*m_board.getPlayer(p))<0 && !m_board.squareHasKing(p)) continue;
      Position newpos(p.m_x+ii,p.m_y+jj); //the square to jump over
      if(!m_board.squareExists(newpos)) continue;
      else if(m_board.squareIsOccupied(newpos)){ //can't jump over empty square
        if(m_board.getSquare(newpos).getPlayer()==m_board.getSquare(p).getPlayer()) continue;
        newpos=Position(p.m_x+(2*ii),p.m_y+(2*jj));
        if(!m_board.squareExists(newpos)) continue;
        if(m_board.squareIsOccupied(newpos)) continue;
        possibleMoves.push_back(newpos);
      }
    }
  }
  return possibleMoves;
}

/**
 *  Get all possible moves for player
 *
 *  @param player which player's moves to search for
 *  @return vector of all possible moves
 */
std::vector<std::vector<Position> > Game::getMovesForPlayer(const int player) const{
  std::vector<std::vector<Position> > possibleMoves;
  bool jumpFound = false;
  for(int ii=0;ii<m_board.getSize();ii++){
    for(int jj=0;jj<m_board.getSize();jj++){
      Position p(ii,jj);
      if(m_board.squareIsOccupied(p) && m_board.getPlayer(p)==player){
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
    m_gameOver=true;  //if player has no legal moves, opponent has won
    m_winner = player*-1;
  }
  std::sort(possibleMoves.begin(),possibleMoves.end(),sortMovesByLength);
  return possibleMoves;
}

/**
 *  Get the squares jumped over during a sequence move
 *
 *  @param p the starting point and all successive locations in the move
 *  @return vector of the squares which were jumped over
 */
std::vector<Position> Game::getJumpedSquares(const std::vector<Position> &p) const{
  std::vector<Position> jumped;
  for(int ii=0;ii<p.size()-1;ii++){
    jumped.push_back(m_board.getJump(p.at(ii), p.at(ii+1)));
  }
  return jumped;
}

/**
 *  Execute a move
 *
 *  Executes a move. Piece is moved from initial position to final position, 
 *  and any pieces captured are removed from the board. Increments move counter,
 *  checks game status for win/draw.
 *  
 *  @param move vector the starting point of the piece to move, and all its subsequent locations
 */
void Game::executeMove(const std::vector<Position> &move){
  bool stale=true;
  for(int ii=0;ii<move.size()-1;ii++){
    if(fabs(move.at(ii).m_y - move.at(ii+1).m_y) != 1){
      m_board.removePiece(m_board.getJump(move.at(ii),move.at(ii+1)));
      stale = false;
    }
    m_board.movePiece(move.at(ii), move.at(ii+1));
    if( !m_board.squareHasKing(move.at(ii+1)) ){
      if( m_currentPlayer == 1 ){
        if(move.at(ii+1).m_y == m_board.getSize()-1){
          m_board.setKing(move.at(ii+1));
          stale = false;
        }
      }
      else{
        if(move.at(ii+1).m_y == 0){
          m_board.setKing(move.at(ii+1));
          stale = false;
        }
      }
    }
  }
  if(m_board.getNumPiecesPlayer(1)==0){
    m_gameOver = true;
    m_winner = -1;
  }
  else if(m_board.getNumPiecesPlayer(-1)==0){
    m_gameOver = true;
    m_winner = 1;
  }
  
  m_currentPlayer *= -1;
  if(getMovesForPlayer(m_currentPlayer).size()==0){
    m_gameOver = true;
    m_winner = -1*m_currentPlayer;
  }
  if(!stale){
    m_staleness=0;
    m_paststates.clear();
  }
  else{
    if(m_paststates.find(m_board)!=m_paststates.end()){
      m_paststates[m_board]++;
    }
    else{
      m_paststates.emplace(m_board,1);
    }
    if(m_paststates[m_board]>3){
      m_gameOver = true;
    }
    
    if(!m_gameOver){
      m_staleness++;
    }
    if(m_staleness>=m_maxStaleness){
      m_gameOver = true;
      m_stale = true;
    }

  }
  m_turn++;
}

