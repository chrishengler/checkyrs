//
//  ai.cpp
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#define likeabillion 1000000000

#include "ai.h"


bool sortMoveEvals(const moveEval &lhs, const moveEval &rhs){
  return lhs.second < rhs.second;
}

bool sortMoveEvalsReverse(const moveEval &lhs, const moveEval &rhs){
  return lhs.second > rhs.second;
}

double CheckyrsAI::eval(const Board &b) const{
  double value=0;
  
  for(int ii=0;ii<b.getSize();ii++){
    for(int jj=0;jj<b.getSize();jj++){
      Position p = (m_player==1 ? Position(ii,jj) : Position(7-ii,7-jj)); //loop rows in reverse order if p2
      if(b.SquareIsOccupied(p)){
        value += b.getPlayer(p)*(10+(b.SquareHasKing(p) ? jj*2*m_possession : jj*m_possession));
      }
    }
  }
  return value*m_player;
}

moveEval CheckyrsAI::evalNode(const Game &g, const bool opp){
  std::vector<std::vector<Position> > p = g.getMovesForPlayer( opp ? m_player*-1 : m_player);
  std::vector<moveEval> evals;
  for(std::vector<std::vector<Position> >::iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
    double value=0;
    Game newstate(g);
    newstate.ExecuteMove(*p_iter);
    value=eval(newstate.getBoard());
    evals.push_back(std::make_pair(*p_iter, value));
  }
  if(opp){
    std::sort(evals.begin(),evals.end(),sortMoveEvalsReverse); //sort reverse, assume opponent makes best move
  }
  else std::sort(evals.begin(),evals.end(),sortMoveEvals); //if not opponent, pick our best move
  return evals.at(0);
}

moveEval CheckyrsAI::rootNegamax(const Game &g, const int depth){
  std::vector<std::vector<Position> > p = g.getMovesForPlayer(m_player);
  double best = -2*likeabillion;
  double value = -3*likeabillion;
  moveEval bestMove;
  for(std::vector<std::vector<Position> >::iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
    Game newstate(g);
    newstate.ExecuteMove((*p_iter));
    value = negamax(newstate, depth-1, true);
    if(value>best){
      best = value;
      bestMove = std::make_pair((*p_iter),value);
    }
  }
  return bestMove;
}

double CheckyrsAI::negamax(const Game &g, const int depth, const bool ownTurn){
  if(depth<=0){
    return evalNode(g).second;
  }
  double best = -2*likeabillion;
  double value = -3*likeabillion;;
  std::vector<std::vector<Position> > p = g.getMovesForPlayer( ownTurn ? m_player : m_player*-1 );
  for(std::vector<std::vector<Position> >::iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
    Game newstate(g);
    newstate.ExecuteMove(*p_iter);
    value = negamax(newstate,depth-1,!ownTurn);
    if(value > best){
      best = value;
    }
  }
  return best;
}