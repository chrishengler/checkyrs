//
//  ai.cpp
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#define likeabillion 1000000000

#include <iostream>
#include "math.h"
#include "ai.h"


bool sortMoveEvals(const moveEval &lhs, const moveEval &rhs){
  return lhs.second < rhs.second;
}

bool sortMoveEvalsReverse(const moveEval &lhs, const moveEval &rhs){
  return lhs.second > rhs.second;
}

double CheckyrsAI::eval(const Game &g) const{
  Board b=g.getBoard();
  int pushmen = 30; //after this number of turns, add incentive to move men away from home row
  double kingweight = 3000;
  double normweight = 1250;
  double value = 0;
  double thissquare = 0;
  
  int boardsize = b.getSize();
  
  bool limitedthreat = true; //player to move doesn't need to be overly concerned if they only have one takeable piece - just move it
  for(int ii=0;ii<boardsize;ii++){
    for(int jj=0;jj<boardsize;jj++){
      Position p = (m_player==1 ? Position(ii,jj) : Position( (boardsize-1)-ii , (boardsize-1)-jj )); //loop rows in reverse order if p2
      if(b.SquareIsOccupied(p)){
        //get a bunch of values once rather than calling methods over and over
        bool isKing = b.SquareHasKing(p);
        bool isCurrentPlayer = (b.getPlayer(p) == g.getCurrentPlayer());
        bool isAI = (b.getPlayer(p) == m_player);
        int ai_adv = jj+1; //how advanced an AI pawn is
        int opp_adv = boardsize-jj; //opponent
        int distanceToSide = b.DistanceToSide(p);
        int distanceToEnd = b.DistanceToEnd(p);
        bool isThreatened = g.PieceIsThreatened(p);
        bool canCapture = g.PieceCanCapture(p);
        bool canCrown = g.PieceCanCrown(p);
        int def = g.PieceDefence(p);
        
        if(isCurrentPlayer){
          thissquare = (isKing ? m_possession*kingweight : m_possession*(normweight+(50*ai_adv)) );
        }
        else{
          thissquare = -1*(isKing ? m_aggression*kingweight : m_aggression*(normweight+(50*opp_adv)) );
        }
        if( g.getCurrentTurn()>pushmen && !isKing && (isAI ? ai_adv<boardsize/2 : opp_adv<boardsize/2) ){
          if(isAI){
            thissquare *= 1-(0.1 * ai_adv/boardsize);
          }
          else thissquare *= 1-(0.1 * opp_adv/boardsize);
        }
        if(jj>0 && jj<boardsize-1){ //AI had tendency to never move pieces out of corner square
          thissquare *= (0.9 + 0.05*(distanceToSide>3 ? 3 : distanceToSide)); //prevent edge bonus being applied there
        }
        if(distanceToSide>1){ //try to control board centre
          thissquare *= (0.9 + 0.05*distanceToEnd);
        }
        if(isThreatened){ //occasional suicidal moves with no apparent benefit, let's apply heavy penalties
          if(limitedthreat && isCurrentPlayer ){
            if(canCapture){ //under threat, but can capture another piece right now
              limitedthreat = false;
              thissquare *= -0.2;
            }
            else if(g.getMovesFrom(p).size()>0){ //can't capture but can be captured if not moved
              limitedthreat = false;
              thissquare *= -0.7;
            }
            else thissquare *= -1.3; //under threat, no possibility to capture or move to safety
          }
          else{
            thissquare *= -2; //highly penalise multiple threatened pieces/threatened pieces when opponent to move
          }
        }
        else if(canCapture){
          if(isCurrentPlayer || !isThreatened){//no bonus for capture chance if it'll be taken before doing so
            thissquare *= 1.5;
          }
        }
        if(canCrown){
          thissquare += (isAI ? 500 : -500);
        }
        thissquare *= 1+0.025*( def>2 ? 2 : def);
        value += thissquare;
        thissquare = 0;
      }
    }
  }
  value += m_player*200*(g.getNumPiecesPlayer(1)-g.getNumPiecesPlayer(-1));
  value += m_player*200*(g.getNumKingsPlayer(1)-g.getNumKingsPlayer(-1));

  double staleness = g.getStaleness()/g.getMaxStaleness();
  if(staleness > 0.33 && (g.getNumPiecesPlayer(m_player)-g.getNumPiecesPlayer(-1*m_player))<2 ){
    //if a draw is approaching and this AI doesn't have significantly fewer pieces than opponent, risk trying something new
    if(staleness > 0.5){
      if(staleness > 0.75){
        value *= (rand()-0.5); //very close to draw, think about doing something crazy
      }
      else value *= (rand()-0.5)/2; //getting close to draw, introduce bigger fluctuations
    }
    else value *= (rand()-0.5)/4; //plenty of time, only introduce small fuzziness
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
    if(p.size()==1) return std::make_pair(p.at(0),likeabillion); //don't waste time evaluating tree if there's only one branch
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