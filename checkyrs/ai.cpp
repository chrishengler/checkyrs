//
//  ai.cpp
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include "ai.h"


bool sortMoveEvals(const moveEval &lhs, const moveEval &rhs){
  return lhs.second < rhs.second;
}

double CheckyrsAI::eval(const Board &b) const{
  double value=0;
  
  for(int ii=0;ii<b.getSize();ii++){
    for(int jj=0;jj<b.getSize();jj++){
      Position p = (m_player==1 ? Position(ii,jj) : Position(7-ii,7-jj));
      if(b.SquareIsOccupied(p)){
        if(b.getPlayer(p)==m_player){
          value+=(b.SquareHasKing(p) ? jj*2*m_possession : jj*m_possession);
        }
        else{
          value-=(b.SquareHasKing(p) ? jj*2*m_aggression : jj*m_aggression);
        }
      }
    }
  }
  return value;
}

std::vector<moveEval> CheckyrsAI::evalMoves(const Board &b, const std::vector<std::vector<Position> > p, const bool opp){
  std::vector<moveEval> evals;
  for(std::vector<std::vector<Position> >::const_iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
    double value=0;
    Game newstate(b);
    newstate.ExecuteMove(*p_iter);
    value=eval(newstate.getBoard());
    evals.push_back(std::make_pair(*p_iter, value));
  }
  std::sort(evals.begin(),evals.end(),sortMoveEvals);
  return evals;
}

