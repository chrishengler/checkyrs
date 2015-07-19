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
  _board.AddPiece(pos,player,isKing);
}

void Game::MovePiece(const Position &oldp, const Position &newp){
  _board.MovePiece(oldp, newp);
}

std::vector<Position> Game::getMovesFrom(const Position &p){
  std::vector<Position> possibleMoves;
  for(int ii=-1;ii<=1;ii+=2){
    if(ii<0 && !_board.SquareHasKing(p)) continue;
    for(int jj=-1;jj<=1;jj+=2){
      Position newpos(p._x+ii,p._y+jj);
      if(!_board.PositionExists(newpos)) continue;
      if(!_board.SquareIsOccupied(newpos)){
        possibleMoves.push_back(newpos);
      }
      else{
        if(_board.getPiece(newpos).getPlayer()==_board.getPiece(p).getPlayer()) continue;
        newpos=Position(p._x+(2*ii),p._y+(2*jj));
        if(!_board.PositionExists(newpos)) continue;
        if(_board.SquareIsOccupied(newpos)) continue;
        possibleMoves.push_back(newpos);
      }
    }
  }
  return possibleMoves;
}
