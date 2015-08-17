//
//  ai.cpp
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#define likeabillion 1000000000

#include <iostream>
#include "ai.h"


bool sortMoveEvals(const moveEval &lhs, const moveEval &rhs){
  return lhs.second < rhs.second;
}

bool sortMoveEvalsReverse(const moveEval &lhs, const moveEval &rhs){
  return lhs.second > rhs.second;
}

double CheckyrsAI::eval(const Game &g) const{
  Board b=g.getBoard();
  double kingweight = 2000;
  double normweight = 1000;
  double value = 0;
  double thissquare = 0;
  int boardsize = b.getSize();
  for(int ii=0;ii<boardsize;ii++){
    for(int jj=0;jj<boardsize;jj++){
      Position p = (m_player==1 ? Position(ii,jj) : Position( (boardsize-1)-ii , (boardsize-1)-jj )); //loop rows in reverse order if p2
      if(b.SquareIsOccupied(p)){
        if(b.getPlayer(p) == m_player){
          thissquare = (b.SquareHasKing(p) ? kingweight : normweight+(10*m_aggression*jj) );
        }
        else{
          thissquare = -1*(b.SquareHasKing(p) ? kingweight : normweight+(10*m_possession*(boardsize-1)-jj) );
        }
        if(b.SquareNearEdge(p)){
          thissquare *= 1.2;
        }
        if(g.PieceIsThreatened(p)){
          thissquare *= -0.8;
        }
        value += thissquare;
        thissquare = 0;
      }
    }
  }
  return value;
}

double CheckyrsAI::evalNode(const Game &g, const bool opp) const{
  try{
    std::vector<std::vector<Position> > p = g.getMovesForPlayer( opp ? m_player*-1 : m_player);
    std::vector<moveEval> evals;
    if(p.size()==0){
      return opp ? likeabillion : -likeabillion;
    }
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
    //std::cout << evals.size() << " possible moves at final branch node\n";
    return evals.at(0).second;
  }catch(std::exception &e){
    throw e;
  }
}

moveEval CheckyrsAI::rootNegamax(const Game &g, const int depth) const{
  //this method will find optimum move based on looking depth moves ahead from current position
  //finds all possible moves then calls negamax() method for each to determine value
  moveEval bestMove;
  try{
    std::vector<std::vector<Position> > p = g.getMovesForPlayer(m_player);
    std::cout << "AI player " << (m_player==1? "1" : "2") << " is thinking: " << p.size() << " initial moves from this point\n";
    double best = -2*likeabillion;
    double value = -3*likeabillion;
    for(std::vector<std::vector<Position> >::iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
      Game newstate(g);
      newstate.ExecuteMove((*p_iter));
      value = negamax(newstate, depth-1, true);
      if(value>best){
        best = value;
        bestMove = std::make_pair((*p_iter),value);
      }
    }
  }
  catch(std::exception &e){
    throw e;
  }
  return bestMove;
}

double CheckyrsAI::negamax(Game g, const int depth, const bool ownTurn) const{
  //negamax recursively calls itself, iterating a tree of all possible game paths for the next depth moves
  //find path with best outcome for AI, assuming opponent always chooses their best move
  try{
    if(depth<=0){
      if(g.gameOver()){
        return (g.getWinner()==m_player) ? likeabillion : -likeabillion;
      }
      else return evalNode(g);
    }
    double best = -2*likeabillion;
    double value = -3*likeabillion;;
    std::vector<std::vector<Position> > p = g.getMovesForPlayer( ownTurn ? m_player : m_player*-1 );
    if(g.gameOver()){
      return (g.getWinner()==m_player) ? likeabillion+depth : -(likeabillion+depth);
    }
    for(std::vector<std::vector<Position> >::iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
      Game newstate(g);
      newstate.ExecuteMove(*p_iter);
      value = negamax(newstate,depth-1,!ownTurn);
      if(value > best){
        best = value;
      }
    }
    return best;
  }catch(std::exception &e){
    throw e;
  }
}