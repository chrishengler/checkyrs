//
//  ai.cpp
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#define likeatrillion 1000000000000

#include <iostream>
#include "math.h"
#include "ai.h"

template<class Archive>  void CheckyrsAI::serialize(Archive &ar, const unsigned int version){
  //preference for attacking opponent's pieces vs maintaining own pieces
  ar & m_aggression;
  ar & m_possession;
  
  //point in game at which to encourage moving men forward/weight & offset for doing so
  ar & m_pushmen;
  ar & m_pushweight;
  ar & m_push_offset;
  ar & m_push_max; //point at which to no longer force that
  
  //weight for kings vs normal pieces
  ar & m_kingweight;
  ar & m_normweight;
  
  //weights for position: advancement, distance from sides/ends/corner of board
  ar & m_advweight;
  ar & m_sideweight;
  ar & m_endweight;
  ar & m_cornerweight;
  
  //offsets for each of the above weights
  ar & m_adv_offset;
  ar & m_side_offset;
  ar & m_end_offset;
  ar & m_corner_offset;
  
  //thresholds for where position bonuses are applied
  ar & m_adv_max;
  ar & m_adv_min;
  ar & m_side_min;
  ar & m_side_max;
  ar & m_end_min;
  ar & m_end_max;
  ar & m_corner_min;
  ar & m_corner_max;
  
  //weights for situational bonuses/maluses
  ar & m_threatweight_cancapture;
  ar & m_threatweight_limited;
  ar & m_threatweight;
  ar & m_threatweight_extreme;
  ar & m_captureweight;
  ar & m_crownweight;
  ar & m_defweight;
  ar & m_def_offset;
  ar & m_def_max;
  
  ar & m_material_bonus;
  ar & m_king_bonus;

}

void CheckyrsAI::save(const std::string &filename){
  std::ofstream ofs(filename.c_str());
  boost::archive::text_oarchive oa(ofs);
  oa << *this;
}
void CheckyrsAI::load(const std::string &filename){
  std::ifstream ifs(filename.c_str());
  boost::archive::text_iarchive ia(ifs);
  ia >> *this;
}

//sort functions for moveEval
bool sortMoveEvals(const moveEval &lhs, const moveEval &rhs){
  return (lhs.second > rhs.second);
}

bool sortMoveEvalsReverse(const moveEval &lhs, const moveEval &rhs){
  return (lhs.second < rhs.second);
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
  if(!random){
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
  else randomiseAI();
}

void CheckyrsAI::randomiseAI(){
  std::cout << "generating random AI:\n";
  m_rng = boost_rng((unsigned)std::time(0));

  std::vector<double*> ap_params;
  ap_params.push_back(&m_aggression); ap_params.push_back(&m_possession);

  std::vector<double*> piece_weights;
  piece_weights.push_back(&m_kingweight); piece_weights.push_back(&m_normweight);

  std::vector<double*> multi_offsets;
  multi_offsets.push_back(&m_push_offset); multi_offsets.push_back(&m_side_offset);
  multi_offsets.push_back(&m_end_offset); multi_offsets.push_back(&m_corner_offset);
  multi_offsets.push_back(&m_def_offset);

  std::vector<double*> multi_weights;
  multi_weights.push_back(&m_pushweight); multi_weights.push_back(&m_sideweight);
  multi_weights.push_back(&m_endweight); multi_weights.push_back(&m_cornerweight);
  multi_weights.push_back(&m_defweight);

  std::vector<double*> threatened_weights;
  threatened_weights.push_back(&m_threatweight_cancapture); threatened_weights.push_back(&m_threatweight_limited);
  threatened_weights.push_back(&m_threatweight); threatened_weights.push_back(&m_threatweight_extreme);

  std::vector<double*> material_weights;
  material_weights.push_back(&m_material_bonus); material_weights.push_back(&m_king_bonus);

  std::pair<int*,int*> adv_bounds = std::make_pair(&m_adv_min, &m_adv_max);
  std::pair<int*,int*> side_bounds = std::make_pair(&m_side_min, &m_side_max);
  std::pair<int*,int*> end_bounds = std::make_pair(&m_end_min, &m_end_max);
  std::pair<int*,int*> corner_bounds = std::make_pair(&m_corner_min, &m_corner_max);

  randomiseDoubles( ap_params, 0, 10 );
  randomiseInt(&m_pushmen,0,100);
  randomiseDoubles(piece_weights,0,5000);

  randomiseDouble(&m_adv_offset,-1000,1000);
  randomiseDouble(&m_advweight,-1000,1000);

  randomiseDoubles(multi_offsets,-2,2);
  randomiseDoubles(multi_weights,-1,1);

  randomOrderedIntPair(adv_bounds);
  randomOrderedIntPair(side_bounds);
  randomOrderedIntPair(end_bounds);
  randomOrderedIntPair(corner_bounds);

  randomiseDoubles(threatened_weights,-5,5);
  randomiseDouble(&m_captureweight,-5,5);
  randomiseDouble(&m_crownweight,-1000,1000);
  randomiseDoubles(material_weights,-1000,1000);

  randomiseInt(&m_def_max,0,4);

}

CheckyrsAI CheckyrsAI::breed(const CheckyrsAI &p2, float mutate){
  //what I wouldn't have given for some proper reflection in C++ while writing this...
  m_rng = boost_rng((unsigned)std::time(0));

  CheckyrsAI offspring;

  gene(&offspring.m_aggression, this->m_aggression, p2.m_aggression, 0, 10, mutate);
  gene(&offspring.m_possession, this->m_possession, p2.m_possession, 0, 10, mutate);
  gene(&offspring.m_pushweight, this->m_pushweight, p2.m_pushweight, 0, 5000, mutate);
  gene(&offspring.m_kingweight, this->m_kingweight, p2.m_kingweight, 0, 5000, mutate);

  gene(&offspring.m_adv_offset, this->m_adv_offset, p2.m_adv_offset, -1000, 1000, mutate);
  gene(&offspring.m_advweight, this->m_advweight, p2.m_advweight, -1000, 1000, mutate);

  gene(&offspring.m_push_offset, this->m_push_offset, p2.m_push_offset, -2, 2, mutate);
  gene(&offspring.m_side_offset, this->m_side_offset, p2.m_side_offset, -2, 2, mutate);
  gene(&offspring.m_end_offset, this->m_end_offset, p2.m_end_offset, -2, 2, mutate);
  gene(&offspring.m_corner_offset, this->m_corner_offset, p2.m_corner_offset, -2, 2, mutate);
  gene(&offspring.m_def_offset, this->m_def_offset, p2.m_def_offset, -2, 2, mutate);

  gene(&offspring.m_pushweight, this->m_pushweight, p2.m_pushweight, -1, 1, mutate);
  gene(&offspring.m_sideweight, this->m_sideweight, p2.m_sideweight, -1, 1, mutate);
  gene(&offspring.m_endweight, this->m_endweight, p2.m_endweight, -1, 1, mutate);
  gene(&offspring.m_cornerweight, this->m_cornerweight, p2.m_cornerweight, -1, 1, mutate);
  gene(&offspring.m_defweight, this->m_defweight, p2.m_defweight, -1, 1, mutate);

  gene(std::make_pair(&offspring.m_adv_min,&offspring.m_adv_max), std::make_pair(this->m_adv_min,this->m_adv_max), std::make_pair(p2.m_adv_min,p2.m_adv_max), 0, 8, mutate);
  gene(std::make_pair(&offspring.m_side_min,&offspring.m_side_max), std::make_pair(this->m_side_min,this->m_side_max), std::make_pair(p2.m_side_min,p2.m_side_max), 0, 8, mutate);
  gene(std::make_pair(&offspring.m_end_min,&offspring.m_end_max), std::make_pair(this->m_end_min,this->m_end_max), std::make_pair(p2.m_end_min,p2.m_end_max), 0, 8, mutate);
  gene(std::make_pair(&offspring.m_corner_min,&offspring.m_corner_max), std::make_pair(this->m_corner_min,this->m_corner_max), std::make_pair(p2.m_corner_min,p2.m_corner_max), 0, 8, mutate);

  gene(&offspring.m_threatweight_cancapture, this->m_threatweight_cancapture, p2.m_threatweight_cancapture, -5, 5, mutate);
  gene(&offspring.m_threatweight_limited, this->m_threatweight_limited, p2.m_threatweight_limited, -5, 5, mutate);
  gene(&offspring.m_threatweight, this->m_threatweight, p2.m_threatweight, -5, 5, mutate);
  gene(&offspring.m_threatweight_extreme, this->m_threatweight_extreme, p2.m_threatweight_extreme, -5, 5, mutate);

  gene(&offspring.m_captureweight, this->m_captureweight, p2.m_captureweight, -5, 5, mutate);
  gene(&offspring.m_crownweight, this->m_crownweight, p2.m_crownweight, -1000, 1000, mutate);
  gene(&offspring.m_material_bonus, this->m_material_bonus, p2.m_material_bonus, -1000, 1000, mutate);
  gene(&offspring.m_king_bonus, this->m_king_bonus, p2.m_king_bonus, -1000, 1000, mutate);

  gene(&offspring.m_def_max, this->m_def_max, p2.m_def_max, 0, 4, mutate);

  return offspring;
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

  int ai_mat = g.getNumPiecesPlayer(g.getCurrentPlayer());
  int opp_mat = g.getNumPiecesPlayer(g.getCurrentPlayer()*-1);

  int ai_kings = g.getNumKingsPlayer(g.getCurrentPlayer());
  int opp_kings = g.getNumKingsPlayer(g.getCurrentPlayer());

  double matratio = opp_mat==0? ai_mat+1 : (double)ai_mat/opp_mat; //check for zero only for unit tests, method won't be called with gameover in actual usage
  double kingratio = opp_kings==0? ai_kings+1 : (double)ai_kings/opp_kings;

  value += m_player*m_material_bonus*(matratio);
  value += m_player*m_king_bonus*(kingratio);

  return value;
}

double CheckyrsAI::evalNode(const Game &g) const{
  try{
    std::vector<std::vector<Position> > p = g.getMovesForPlayer( g.getCurrentPlayer() );
    std::vector<moveEval> evals;
    if(g.gameOver()){
      return (g.getWinner()==g.getCurrentPlayer()) ? likeatrillion : -likeatrillion;
    }
    for(std::vector<std::vector<Position> >::iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
      double value=0;
      Game newstate(g);
      newstate.ExecuteMove(*p_iter);

      value=eval(newstate.getBoard());
      evals.push_back(std::make_pair(*p_iter, value));
    }
    if(g.getCurrentPlayer()!=m_player){
      std::sort(evals.begin(),evals.end(),sortMoveEvalsReverse); //sort reverse, assume opponent makes best move
    }
    else std::sort(evals.begin(),evals.end(),sortMoveEvals); //if not opponent, pick our best move
    return evals.at(0).second;
  }catch(std::exception &e){
    throw e;
  }
}

moveEval CheckyrsAI::rootNegamax(const Game &g, const int depth) const{
  //this method will find optimum move based on looking depth moves ahead from current position
  //finds all possible moves then calls negamax() method for each to determine value
  std::vector<moveEval> moves;
  double alpha = -INFINITY;
  double beta = INFINITY;
  double best = -INFINITY;
  try{
    std::vector<std::vector<Position> > p = g.getMovesForPlayer(m_player);
    std::cout << "AI player " << (m_player==1? "1" : "2") << " is thinking: " << p.size() << " initial moves from this point\n";
    if(p.size()==1) return std::make_pair(p.at(0),likeatrillion); //don't waste time evaluating tree if there's only one branch
    for(std::vector<std::vector<Position> >::iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
      Game newstate(g);
      newstate.ExecuteMove((*p_iter));
      double value = -negamax(newstate, depth-1, -beta, -alpha);
      if(value>best){
        best = value;
      }
      moves.push_back(std::make_pair((*p_iter),value));
    }
  }
  catch(std::exception &e){
    throw e;
  }

  std::sort(moves.begin(),moves.end(),sortMoveEvals);

  double staleness = g.getStaleness()/g.getMaxStaleness();
  moveEval bestMove;
  if(staleness<0.3 || (g.getNumPiecesPlayer(1)+g.getNumPiecesPlayer(-1) > 10)) bestMove = moves.at(0);
  else{    //if draw is getting close, try something new.
    int pick;
    if(staleness>0.6) pick=getRandomInt(0,(int)moves.size()); //try to stick to highly rated moves at first
    else pick=getRandomInt(0,floor(moves.size()/2));

    do{
      bestMove=moves.at(pick);
      pick--;
    }while(moves.at(pick).second> (-likeatrillion/2.)); //don't select an obvious losing piece
  }

  return bestMove;
}

double CheckyrsAI::negamax(Game g, const int depth, double alpha, double beta) const{
  //negamax recursively calls itself, iterating a tree of all possible game paths for the next depth moves
  //find path with best outcome for AI, assuming opponent always chooses their best move
  try{
    if(depth<=0){
      if(g.gameOver()){
        return (g.getWinner()==g.getCurrentPlayer()) ? likeatrillion : -likeatrillion;
      }
      else return evalNode(g);
    }
    std::vector<std::vector<Position> > p = g.getMovesForPlayer( g.getCurrentPlayer() );
    if(g.gameOver()){
      return (g.getWinner()==g.getCurrentPlayer()) ? likeatrillion+depth : -(likeatrillion+depth);
    }
    for(std::vector<std::vector<Position> >::iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
      Game newstate(g);
      newstate.ExecuteMove(*p_iter);
      alpha = fmax(alpha,-negamax(newstate,depth-1, -beta, -alpha));
      if(alpha>=beta) break;
    }
    return alpha;
  }catch(std::exception &e){
    throw e;
  }
}
