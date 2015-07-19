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
  for(int ii=-2;ii<=2;ii++){
    if(ii==0) continue;
    for(int jj = -1*fabs(ii); jj<=(fabs(ii)); jj++){
      if( (jj<0 || fabs(jj)>1) && !_board.SquareHasKing(p)){
        continue;
      }
      if(fabs(jj)!=fabs(ii)) continue;
      Position p(ii,jj);
      possibleMoves.push_back(p);
    }
  }
  for(std::vector<Position>::iterator pos=possibleMoves.begin();pos!=possibleMoves.end();){
    //no auto increment in for loop because vector.erase() already returns iterator pointing to next element
    if(!_board.PositionExists(*pos) || _board.SquareIsOccupied(*pos)){
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
