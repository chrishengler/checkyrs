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

double CheckyrsAI::getRandomDouble(const double min, const double max) const{
  boost::uniform_real<> real_dist(min,max);
  boost::variate_generator<boost_rng&, boost::uniform_real<> > gen(m_rng,real_dist);
  return gen();
}

int CheckyrsAI::getRandomInt(const int min, const int max) const{
  boost::uniform_int<> int_dist(min,max);
  boost::variate_generator<boost_rng&, boost::uniform_int<> > gen(m_rng,int_dist);
  return gen();
}

void CheckyrsAI::randomiseDouble(double *var, const double min, const double max){
  *var = getRandomDouble(min, max);
}

void CheckyrsAI::randomiseDoubles(std::vector<double*> &vars, const double min, const double max){
  for(int ii=0;ii<vars.size();ii++){
    randomiseDouble(vars.at(ii),min,max);
  }
}

void CheckyrsAI::randomiseInt(int *var, const int min, const int max){
  *var = getRandomInt(min,max);
}

void CheckyrsAI::randomiseInts(std::vector<int*> &vars, const int min, const int max){
  for(int ii=0;ii<vars.size();ii++){
    randomiseInt(vars.at(ii),min,max);
  }
}

void CheckyrsAI::randomOrderedIntPair(std::pair<int*,int*> &vars, const int min, const int max){
  int x,y;
  randomiseInt(&x,min,max);
  randomiseInt(&y,min,max);
  if(x < y){
    *(vars.first)=x; *(vars.second)=y;
  }
  else{
    *(vars.first)=y; *(vars.second)=x;
  };
  
}


CheckyrsAI::CheckyrsAI(const int player){
  m_player=player;
}

void CheckyrsAI::Initialise(bool random){
  if(random){
    randomiseAI();
    return;
  }

  m_aggression=5;
  m_possession=5;
  
  //set default values as used in initial 'hand-tuned' AI
  m_pushmen=30;
  m_push_offset=1;
  m_pushweight=0.1;
  
  m_kingweight=3000;
  m_normweight=1250;
  
  m_advweight=50;
  m_sideweight=0.05;
  m_endweight=0.05;
  m_cornerweight=0.05;
  
  m_adv_offset=0;
  m_side_offset=0.9;
  m_end_offset=0.9;
  m_corner_offset=1;
  
  m_adv_min=0;
  m_adv_max=8;
  m_side_min=1;
  m_side_max=4;
  m_end_min=0;
  m_end_max=4;
  m_corner_min=0;
  m_corner_max=4;
  
  m_threatweight_cancapture=0.4;
  m_threatweight_limited=-0.5;
  m_threatweight=-1.3;
  m_threatweight_extreme=-2;
  m_captureweight=1.5;
  m_crownweight=500;
  m_defweight=0.025;
  m_def_offset=1;
  m_def_max=2;
  
  m_material_bonus=200;
  m_king_bonus=200;
}

void CheckyrsAI::randomiseAI(){
  m_rng = boost_rng((uint)std::time(0));
  
  std::vector<double*> ap_params = {&m_aggression, &m_possession};
  std::vector<double*> multi_offsets = {&m_push_offset, &m_side_offset, &m_end_offset, &m_corner_offset, &m_def_offset};
  std::vector<double*> multi_weights = {&m_pushweight, &m_sideweight, &m_endweight, &m_cornerweight, &m_defweight};
  std::vector<double*> threatened_weights = {&m_threatweight_cancapture, &m_threatweight_limited, &m_threatweight, &m_threatweight_extreme};
  std::vector<double*> material_weights = {&m_material_bonus, &m_king_bonus};
  std::pair<int*,int*> adv_bounds = std::make_pair(&m_adv_min, &m_adv_max);
  std::pair<int*,int*> side_bounds = std::make_pair(&m_side_min, &m_side_max);
  std::pair<int*,int*> end_bounds = std::make_pair(&m_end_min, &m_end_max);
  std::pair<int*,int*> corner_bounds = std::make_pair(&m_corner_min, &m_corner_max);
  
  randomiseDoubles( ap_params, 0, 10 );
  randomiseInt(&m_pushmen,0,100);
  
  randomiseDouble(&m_adv_offset,-1000,1000);
  randomiseDouble(&m_advweight,-1000,1000);
  
  randomiseDoubles(multi_offsets,-2,2);
  randomiseDoubles(multi_weights,-0.5,0.5);
  
  randomOrderedIntPair(adv_bounds);
  randomOrderedIntPair(side_bounds);
  randomOrderedIntPair(end_bounds);
  randomOrderedIntPair(corner_bounds);
  
  randomiseDoubles(threatened_weights,-5,5);
  randomiseDouble(&m_captureweight,-5,5);
  randomiseDouble(&m_crownweight,-1000,1000);
  randomiseDoubles(material_weights,-1000,1000);
  
  randomiseInt(&m_def_max);
  
}

double CheckyrsAI::eval(const Game &g) const{
  Board b=g.getBoard();
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
        int adv = (isCurrentPlayer ? jj+1 : boardsize-jj); //how advanced an AI pawn is
        int distanceToSide = b.DistanceToSide(p);
        int distanceToEnd = b.DistanceToEnd(p);
        int distanceToCorner = (distanceToSide>distanceToEnd ? distanceToSide : distanceToEnd);
        bool isThreatened = g.PieceIsThreatened(p);
        bool canCapture = g.PieceCanCapture(p);
        bool canCrown = g.PieceCanCrown(p);
        int def = g.PieceDefence(p);
        
        //correct values where a min/max is set:
        if(adv>m_adv_max) adv=m_adv_max;
        else if(adv<m_adv_min) adv=m_adv_min;
    
        if(def>m_def_max) def=m_def_max;
        
        if(distanceToSide<m_side_min) distanceToSide=m_side_min;
        else if(distanceToSide>m_side_max) distanceToSide=m_side_max;
        
        if(distanceToEnd<m_end_min) distanceToEnd=m_end_min;
        else if(distanceToEnd>m_end_max) distanceToEnd=m_end_max;
        
        if(distanceToCorner<m_corner_min) distanceToCorner=m_corner_min;
        else if(distanceToCorner>m_corner_max) distanceToCorner=m_corner_max;
        
        if(isCurrentPlayer){
          if(isKing) thissquare = m_possession*m_kingweight;
          else thissquare = m_possession*(m_normweight+(m_advweight*adv)) ;
        }
        else{
          if(isKing) thissquare = -1*(m_aggression*m_kingweight);
          else thissquare = -1*(m_aggression*(m_normweight+(m_advweight*adv)) );
        }
        if( g.getCurrentTurn()>m_pushmen && !isKing && m_push_max ){
            thissquare *= (m_push_offset - m_pushweight*adv/boardsize);
        }
        thissquare *= (m_side_offset + m_sideweight*distanceToSide);
        thissquare *= (m_end_offset + m_endweight*distanceToEnd);
        thissquare *= (m_corner_offset + m_cornerweight*distanceToCorner);
        if(isThreatened){ //occasional suicidal moves with no apparent benefit, let's apply heavy penalties
          if(limitedthreat && isCurrentPlayer ){
            if(canCapture){ //under threat, but can capture another piece right now, just reduce score
              limitedthreat = false;
              thissquare *= m_threatweight_cancapture;
            }
            else if(g.getMovesFrom(p).size()>0){ //can't capture but can be captured if not moved: bad thing
              limitedthreat = false;
              thissquare *= m_threatweight_limited;
            }
            else thissquare *= m_threatweight; //under threat, no possibility to capture or move to safety
          }
          else{
            limitedthreat = false;
            thissquare *= m_threatweight_extreme; //highly penalise multiple threatened pieces/threatened pieces when opponent to move
          }
        }
        else if(canCapture){
          limitedthreat = false;
          if(isCurrentPlayer || !isThreatened){//no bonus for capture chance if it'll be taken before doing so
            thissquare *= m_captureweight;
          }
        }
        if(canCrown){
          thissquare += (isAI ? m_crownweight : -m_crownweight);
        }
        thissquare *= m_def_offset+(m_defweight*def);
        value += thissquare;
        thissquare = 0;
      }
    }
  }
  value += m_player*m_material_bonus*(g.getNumPiecesPlayer(1)-g.getNumPiecesPlayer(-1));
  value += m_player*m_king_bonus*(g.getNumKingsPlayer(1)-g.getNumKingsPlayer(-1));

  double staleness = g.getStaleness()/g.getMaxStaleness();
  if(staleness > 0.25 && (g.getNumPiecesPlayer(m_player)-g.getNumPiecesPlayer(-1*m_player))<2 ){
    //if a draw is approaching and this AI doesn't have significantly fewer pieces than opponent, risk trying something new
    if(staleness > 0.33){
      if(staleness > 0.66){
        value *= getRandomDouble(0.3,3); //very close to draw, think about doing something crazy
      }
      else value *= getRandomDouble(0.5,2); //getting close to draw, introduce bigger fluctuations
    }
    else value *= getRandomDouble(0.67,1.5); //plenty of time, only introduce small fuzziness
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