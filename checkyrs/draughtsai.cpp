//
//  ai.cpp
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#define likeatrillion 1000000000000

#include <iostream>
#include <random>
#include "math.h"
#include "draughtsai.h"

/**
 *  serialize for Boost library Archive
 *
 *  Holds all of the various weights for use in AI position evaluation
 *
 *  @param ar the Archive for serializing
 *  @param version the version being serialized (currently unused)
 */
template<class Archive>  void CheckyrsAI::serialize(Archive &ar, const unsigned int version){
  //preference for attacking opponent's pieces vs maintaining own pieces
  ar & m_aggression;
  ar & m_possession;
  
  //point in game at which to encourage moving men forward/Weight & offset for doing so
  ar & m_pushMen;
  ar & m_pushWeight;
  ar & m_pushOffset;
  ar & m_pushMax; //point at which to no longer force that
  
  //Weight for kings vs normal pieces
  ar & m_kingWeight;
  ar & m_normWeight;
  
  //Weights for position: advancement, distance from sides/ends/corner of board
  ar & m_advWeight;
  ar & m_sideWeight;
  ar & m_endWeight;
  ar & m_cornerWeight;
  
  //offsets for each of the above Weights
  ar & m_advOffset;
  ar & m_sideOffset;
  ar & m_endOffset;
  ar & m_cornerOffset;
  
  //thresholds for where position bonuses are applied
  ar & m_advMax;
  ar & m_advMin;
  ar & m_sideMin;
  ar & m_sideMax;
  ar & m_endMin;
  ar & m_endMax;
  ar & m_cornerMin;
  ar & m_cornerMax;
  
  //Weights for situational bonuses/maluses
  ar & m_threatWeight_cancapture;
  ar & m_threatWeight_limited;
  ar & m_threatWeight;
  ar & m_threatWeight_extreme;
  ar & m_captureWeight;
  ar & m_crownWeight;
  ar & m_defWeight;
  ar & m_defOffset;
  ar & m_defMax;
  
  ar & m_materialBonus;
  ar & m_kingBonus;

}

/**
 *  Save to file
 *
 *  @param filename the file to save to
 */
void CheckyrsAI::save(const std::string &filename){
  std::ofstream ofs(filename.c_str());
  boost::archive::text_oarchive oa(ofs);
  oa << *this;
}
/**
 *  Load from file
 *
 *  @param filename the file to load from
 */
void CheckyrsAI::load(const std::string &filename){
  std::ifstream ifs(filename.c_str());
  boost::archive::text_iarchive ia(ifs);
  ia >> *this;
}

/**
 *  Sort for moveEval
 *
 *  @param lhs first of the moveEvals to sort
 *  @param rhs second of the moveEvals to sort
 *  @return lhs > rhs
 */
bool sortMoveEvals(const moveEval &lhs, const moveEval &rhs){
  return (lhs.second > rhs.second);
}

/**
 *  Reverse sort for moveEval
 *
 *  @param lhs first of the moveEvals to sort
 *  @param rhs second of the moveEvals to sort
 *  @return lhs < rhs
 */
bool sortMoveEvalsReverse(const moveEval &lhs, const moveEval &rhs){
  return (lhs.second < rhs.second);
}

/**
 *  Get random double within range
 *
 *  Get a random double within a given range. Uses boost random library for generation.
 *
 *  @param min the minimum possible value
 *  @param max the maximum possible value
 *  @return chosen random double
 */
double CheckyrsAI::getRandomDouble(const double min, const double max) const{
  boost::uniform_real<> real_dist(min,max);
  boost::variate_generator<boost_rng&, boost::uniform_real<> > gen(m_rng,real_dist);
  return gen();
}

/**
 *  Get random int within range
 *
 *  Get a random int within a given range. Uses boost random library for generation.
 *
 *  @param min the minimum possible value
 *  @param max the maximum possible value
 *  @return chosen random int
 */
int CheckyrsAI::getRandomInt(const int min, const int max) const{
  boost::uniform_int<> int_dist(min,max);
  boost::variate_generator<boost_rng&, boost::uniform_int<> > gen(m_rng,int_dist);
  return gen();
}

/**
 *  Randomise double
 *
 *  Randomises existing double in-place, using getRandomDouble(min,max)
 *
 *  @param var pointer to the double to be randomised
 *  @param min the minimum possible value
 *  @param max the maximum possible value
 */
void CheckyrsAI::randomiseDouble(double *var, const double min, const double max){
  *var = getRandomDouble(min, max);
}

/**
 *  Randomise multiple doubles
 *
 *  Randomises multiple existing doubles in-place, using randomiseDouble() on each
 *
 *  @param vars vector of pointers to the doubles to be randomised
 *  @param min the minimum possible value
 *  @param max the maximum possible value
 */
void CheckyrsAI::randomiseDoubles(std::vector<double*> &vars, const double min, const double max){
  for(int ii=0;ii<vars.size();ii++){
    randomiseDouble(vars.at(ii),min,max);
  }
}

/**
 *  Randomise int
 *
 *  Randomises existing int in-place, using getRandomInt(min,max)
 *
 *  @param var pointer to the int to be randomised
 *  @param min the minimum possible value
 *  @param max the maximum possible value
 */
void CheckyrsAI::randomiseInt(int *var, const int min, const int max){
  *var = getRandomInt(min,max);
}

/**
 *  Randomise multiple ints
 *
 *  Randomises multiple existing ints in-place, using randomiseInt() on each
 *
 *  @param vars vector of pointers to the ints to be randomised
 *  @param min the minimum possible value
 *  @param max the maximum possible value
 */
void CheckyrsAI::randomiseInts(std::vector<int*> &vars, const int min, const int max){
  for(int ii=0;ii<vars.size();ii++){
    randomiseInt(vars.at(ii),min,max);
  }
}

/**
 *  Gets a random ordered int pair
 *
 *  Randomises existing ints in-place, using results of getRandomInt(min,max) and ensuring ordering
 *
 *  @param vars pair of pointers to the ints to be randomised
 *  @param min the minimum possible value
 *  @param max the maximum possible value
 */
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

/**
 *  CheckyrsAI constructor
 *  
 *  @param player is this AI taking the first turn ('1') or second turn ('-1')
 */
CheckyrsAI::CheckyrsAI(const int player){
  m_player=player;
}

/**
 *  Initialise AI parameters
 *
 *  Either with default (not particularly optimised) values, or random
 *
 *  @param random whether AI parameters should be randomised rather than taking default
 */
void CheckyrsAI::initialise(bool random){
  if(!random){
    m_aggression=5;
    m_possession=5;

    //set default values as used in initial 'hand-tuned' AI
    m_pushMen=30;
    m_pushOffset=1;
    m_pushWeight=0.1;

    m_kingWeight=3000;
    m_normWeight=1250;

    m_advWeight=50;
    m_sideWeight=0.05;
    m_endWeight=0.05;
    m_cornerWeight=0.05;

    m_advOffset=0;
    m_sideOffset=0.9;
    m_endOffset=0.9;
    m_cornerOffset=1;

    m_advMin=0;
    m_advMax=8;
    m_sideMin=1;
    m_sideMax=4;
    m_endMin=0;
    m_endMax=4;
    m_cornerMin=0;
    m_cornerMax=4;

    m_threatWeight_cancapture=0.4;
    m_threatWeight_limited=-0.5;
    m_threatWeight=-1.3;
    m_threatWeight_extreme=-2;
    m_captureWeight=1.5;
    m_crownWeight=500;
    m_defWeight=0.025;
    m_defOffset=1;
    m_defMax=2;

    m_materialBonus=200;
    m_kingBonus=200;
  }
  else randomiseAI();
}

/**
 *  Generate random AI parameters
 *
 *  Minimal restrictions, even in sign. Capable of creating genuine AI (artificial idiot)
 */
void CheckyrsAI::randomiseAI(){
  std::cout << "generating random AI:\n";
  std::random_device rdev;
  m_rng = boost_rng((unsigned int)rdev());

  std::vector<double*> apParams;
  apParams.push_back(&m_aggression); apParams.push_back(&m_possession);

  std::vector<double*> pieceWeights;
  pieceWeights.push_back(&m_kingWeight); pieceWeights.push_back(&m_normWeight);

  std::vector<double*> multiOffsets;
  multiOffsets.push_back(&m_pushOffset); multiOffsets.push_back(&m_sideOffset);
  multiOffsets.push_back(&m_endOffset); multiOffsets.push_back(&m_cornerOffset);
  multiOffsets.push_back(&m_defOffset);

  std::vector<double*> multiWeights;
  multiWeights.push_back(&m_pushWeight); multiWeights.push_back(&m_sideWeight);
  multiWeights.push_back(&m_endWeight); multiWeights.push_back(&m_cornerWeight);
  multiWeights.push_back(&m_defWeight);

  std::vector<double*> threatenedWeights;
  threatenedWeights.push_back(&m_threatWeight_cancapture); threatenedWeights.push_back(&m_threatWeight_limited);
  threatenedWeights.push_back(&m_threatWeight); threatenedWeights.push_back(&m_threatWeight_extreme);

  std::vector<double*> materialWeights;
  materialWeights.push_back(&m_materialBonus); materialWeights.push_back(&m_kingBonus);

  std::pair<int*,int*> advBounds = std::make_pair(&m_advMin, &m_advMax);
  std::pair<int*,int*> sideBounds = std::make_pair(&m_sideMin, &m_sideMax);
  std::pair<int*,int*> endBounds = std::make_pair(&m_endMin, &m_endMax);
  std::pair<int*,int*> cornerBounds = std::make_pair(&m_cornerMin, &m_cornerMax);

  randomiseDoubles( apParams, 0, 10 );
  randomiseInt(&m_pushMen,0,100);
  randomiseDoubles(pieceWeights,0,5000);

  randomiseDouble(&m_advOffset,-1000,1000);
  randomiseDouble(&m_advWeight,-1000,1000);

  randomiseDoubles(multiOffsets,-2,2);
  randomiseDoubles(multiWeights,-1,1);

  randomOrderedIntPair(advBounds);
  randomOrderedIntPair(sideBounds);
  randomOrderedIntPair(endBounds);
  randomOrderedIntPair(cornerBounds);

  randomiseDoubles(threatenedWeights,-5,5);
  randomiseDouble(&m_captureWeight,-5,5);
  randomiseDouble(&m_crownWeight,-1000,1000);
  randomiseDoubles(materialWeights,-1000,1000);

  randomiseInt(&m_defMax,0,4);

}

/**
 *  Breed this AI with another
 *
 *  For each AI parameter, select randomly either this AI's value or the value from p2.\n
 *  Additionally, each parameter has some chance of 'mutating' (randomly generating entirely new value).
 *
 *  @param p2 CheckyrsAI to breed with
 *  @param mutate each parameter's chance of mutating
 *  @return the 'child' CheckyrsAI
 */
CheckyrsAI CheckyrsAI::breed(const CheckyrsAI &p2, float mutate){
  //what I wouldn't have given for some proper reflection in C++ while writing this...
  std::random_device rdev;
  m_rng = boost_rng((unsigned int)rdev());
  CheckyrsAI offspring;

  gene(&offspring.m_aggression, this->m_aggression, p2.m_aggression, 0, 10, mutate);
  gene(&offspring.m_possession, this->m_possession, p2.m_possession, 0, 10, mutate);
  gene(&offspring.m_pushWeight, this->m_pushWeight, p2.m_pushWeight, 0, 5000, mutate);
  gene(&offspring.m_kingWeight, this->m_kingWeight, p2.m_kingWeight, 0, 5000, mutate);

  gene(&offspring.m_advOffset, this->m_advOffset, p2.m_advOffset, -1000, 1000, mutate);
  gene(&offspring.m_advWeight, this->m_advWeight, p2.m_advWeight, -1000, 1000, mutate);

  gene(&offspring.m_pushOffset, this->m_pushOffset, p2.m_pushOffset, -2, 2, mutate);
  gene(&offspring.m_sideOffset, this->m_sideOffset, p2.m_sideOffset, -2, 2, mutate);
  gene(&offspring.m_endOffset, this->m_endOffset, p2.m_endOffset, -2, 2, mutate);
  gene(&offspring.m_cornerOffset, this->m_cornerOffset, p2.m_cornerOffset, -2, 2, mutate);
  gene(&offspring.m_defOffset, this->m_defOffset, p2.m_defOffset, -2, 2, mutate);

  gene(&offspring.m_pushWeight, this->m_pushWeight, p2.m_pushWeight, -1, 1, mutate);
  gene(&offspring.m_sideWeight, this->m_sideWeight, p2.m_sideWeight, -1, 1, mutate);
  gene(&offspring.m_endWeight, this->m_endWeight, p2.m_endWeight, -1, 1, mutate);
  gene(&offspring.m_cornerWeight, this->m_cornerWeight, p2.m_cornerWeight, -1, 1, mutate);
  gene(&offspring.m_defWeight, this->m_defWeight, p2.m_defWeight, -1, 1, mutate);

  gene(std::make_pair(&offspring.m_advMin,&offspring.m_advMax), std::make_pair(this->m_advMin,this->m_advMax), std::make_pair(p2.m_advMin,p2.m_advMax), 0, 8, mutate);
  gene(std::make_pair(&offspring.m_sideMin,&offspring.m_sideMax), std::make_pair(this->m_sideMin,this->m_sideMax), std::make_pair(p2.m_sideMin,p2.m_sideMax), 0, 8, mutate);
  gene(std::make_pair(&offspring.m_endMin,&offspring.m_endMax), std::make_pair(this->m_endMin,this->m_endMax), std::make_pair(p2.m_endMin,p2.m_endMax), 0, 8, mutate);
  gene(std::make_pair(&offspring.m_cornerMin,&offspring.m_cornerMax), std::make_pair(this->m_cornerMin,this->m_cornerMax), std::make_pair(p2.m_cornerMin,p2.m_cornerMax), 0, 8, mutate);

  gene(&offspring.m_threatWeight_cancapture, this->m_threatWeight_cancapture, p2.m_threatWeight_cancapture, -5, 5, mutate);
  gene(&offspring.m_threatWeight_limited, this->m_threatWeight_limited, p2.m_threatWeight_limited, -5, 5, mutate);
  gene(&offspring.m_threatWeight, this->m_threatWeight, p2.m_threatWeight, -5, 5, mutate);
  gene(&offspring.m_threatWeight_extreme, this->m_threatWeight_extreme, p2.m_threatWeight_extreme, -5, 5, mutate);

  gene(&offspring.m_captureWeight, this->m_captureWeight, p2.m_captureWeight, -5, 5, mutate);
  gene(&offspring.m_crownWeight, this->m_crownWeight, p2.m_crownWeight, -1000, 1000, mutate);
  gene(&offspring.m_materialBonus, this->m_materialBonus, p2.m_materialBonus, -1000, 1000, mutate);
  gene(&offspring.m_kingBonus, this->m_kingBonus, p2.m_kingBonus, -1000, 1000, mutate);

  gene(&offspring.m_defMax, this->m_defMax, p2.m_defMax, 0, 4, mutate);

  return offspring;
}

/**
 *  Evaluate game position
 *
 *  Evaluates how favourable the current position is
 *
 *  @param g the Game which is to be evaluated
 *  @return evaluation of this position
 */
double CheckyrsAI::eval(const Game &g) const{
  if(g.gameOver()){
    return evaluateGameOver(g);
  }
  Board b=g.getBoard();
  double value = 0;
  double thissquare = 0;

  int boardsize = b.getSize();

  bool limitedthreat = true; //player to move doesn't need to be overly concerned if they only have one takeable piece - just move it

  for(int ii=0;ii<boardsize;ii++){
    for(int jj=0;jj<boardsize;jj++){
      Position p = (m_player==1 ? Position(ii,jj) : Position( (boardsize-1)-ii , (boardsize-1)-jj )); //loop rows in reverse order if p2
      if(b.squareIsOccupied(p)){
        //get a bunch of values once rather than calling methods over and over
        bool isKing = b.squareHasKing(p);
        bool isCurrentPlayer = (b.getPlayer(p) == g.getCurrentPlayer());
        bool isAI = (b.getPlayer(p) == m_player);
        int adv = (isCurrentPlayer ? jj+1 : boardsize-jj); //how advanced an AI pawn is
        int distanceToSide = b.distanceToSide(p);
        int distanceToEnd = b.distanceToEnd(p);
        int distanceToCorner = (distanceToSide>distanceToEnd ? distanceToSide : distanceToEnd);
        bool isThreatened = g.pieceIsThreatened(p);
        bool canCapture = g.pieceCanCapture(p);
        bool canCrown = g.pieceCanCrown(p);
        int def = g.pieceDefence(p);

        //correct values where a min/max is set:
        if(adv>m_advMax) adv=m_advMax;
        else if(adv<m_advMin) adv=m_advMin;

        if(def>m_defMax) def=m_defMax;

        if(distanceToSide<m_sideMin) distanceToSide=m_sideMin;
        else if(distanceToSide>m_sideMax) distanceToSide=m_sideMax;

        if(distanceToEnd<m_endMin) distanceToEnd=m_endMin;
        else if(distanceToEnd>m_endMax) distanceToEnd=m_endMax;

        if(distanceToCorner<m_cornerMin) distanceToCorner=m_cornerMin;
        else if(distanceToCorner>m_cornerMax) distanceToCorner=m_cornerMax;

        if(isCurrentPlayer){
          if(isKing) thissquare = m_possession*m_kingWeight;
          else thissquare = m_possession*(m_normWeight+(m_advWeight*adv)) ;
        }
        else{
          if(isKing) thissquare = -1*(m_aggression*m_kingWeight);
          else thissquare = -1*(m_aggression*(m_normWeight+(m_advWeight*adv)) );
        }
        if( g.getCurrentTurn()>m_pushMen && !isKing && m_pushMax ){
            thissquare *= (m_pushOffset - m_pushWeight*adv/boardsize);
        }
        thissquare *= (m_sideOffset + m_sideWeight*distanceToSide);
        thissquare *= (m_endOffset + m_endWeight*distanceToEnd);
        thissquare *= (m_cornerOffset + m_cornerWeight*distanceToCorner);
        if(isThreatened){ //occasional suicidal moves with no apparent benefit, let's apply heavy penalties
          if(limitedthreat && isCurrentPlayer ){
            if(canCapture){ //under threat, but can capture another piece right now, just reduce score
              limitedthreat = false;
              thissquare *= m_threatWeight_cancapture;
            }
            else if(g.getMovesFrom(p).size()>0){ //can't capture but can be captured if not moved: bad thing
              limitedthreat = false;
              thissquare *= m_threatWeight_limited;
            }
            else thissquare *= m_threatWeight; //under threat, no possibility to capture or move to safety
          }
          else{
            limitedthreat = false;
            thissquare *= m_threatWeight_extreme; //highly penalise multiple threatened pieces/threatened pieces when opponent to move
          }
        }
        else if(canCapture){
          limitedthreat = false;
          if(isCurrentPlayer || !isThreatened){//no bonus for capture chance if it'll be taken before doing so
            thissquare *= m_captureWeight;
          }
        }
        if(canCrown){
          thissquare += (isAI ? m_crownWeight : -m_crownWeight);
        }
        thissquare *= m_defOffset+(m_defWeight*def);
        value += thissquare;
        thissquare = 0;
      }
    }
  }

  int ai_mat = g.getBoard().getNumPiecesPlayer(g.getCurrentPlayer());
  int opp_mat = g.getBoard().getNumPiecesPlayer(g.getCurrentPlayer()*-1);

  int ai_kings = g.getBoard().getNumKingsPlayer(g.getCurrentPlayer());
  int opp_kings = g.getBoard().getNumKingsPlayer(g.getCurrentPlayer()*-1);

  double matratio = opp_mat==0? ai_mat+1 : (double)ai_mat/opp_mat; //check for zero only for unit tests, method won't be called with gameover in actual usage
  double kingratio = opp_kings==0? ai_kings+1 : (double)ai_kings/opp_kings;

  value += m_player*m_materialBonus*(matratio);
  value += m_player*m_kingBonus*(kingratio);

  return value;
}

/**
 *  Evaluate whether a 'game over' state is good or bad
 *
 *  Winning is very good, losing is very bad.\n 
 *  Opinion of draw decided by evaluateDraw(g)
 *
 *  @param g the Game to be evaluated
 *  @return the evaluation of this game over state
 */
double CheckyrsAI::evaluateGameOver(const Game &g) const{
  if(g.getWinner()==0){
    return evaluateDraw(g);
  }
  return (g.getWinner()==g.getCurrentPlayer()) ? likeatrillion : -likeatrillion;
}

/**
 *  Evaluate whether a 'draw' state is good or bad
 *
 *  Consider number of pieces, number of kings, and try to determine relative strength of players.\n
 *  If opponent seems stronger, draw is good. Otherwise, draw is bad.
 *
 *  @param g the Game to be evaluated
 *  @return the evaluation of this draw
 */
double CheckyrsAI::evaluateDraw(const Game &g) const{
  int ai_mat = g.getBoard().getNumPiecesPlayer(g.getCurrentPlayer());
  int opp_mat = g.getBoard().getNumPiecesPlayer(g.getCurrentPlayer()*-1);
  
  int ai_kings = g.getBoard().getNumKingsPlayer(g.getCurrentPlayer());
  int opp_kings = g.getBoard().getNumKingsPlayer(g.getCurrentPlayer()*-1);
  
  int materialadv = ((ai_mat-opp_mat)*m_materialBonus + (ai_kings-opp_kings)*m_kingBonus);
  if( materialadv < 0 ) return likeatrillion/2.;
  else return -1*likeatrillion;
}

/**
 *  Evaluate this node of tree
 *
 *  Determines which evaluation method is appropriate for this game's state, and performs any required actions
 *  before returning the eval value for this node on game tree
 *
 *  @param g the Game to be evaluated
 *  @return the evaluation of this node
 */
double CheckyrsAI::evalNode(const Game &g) const{
  try{
    std::vector<std::vector<Position> > p = g.getMovesForPlayer( g.getCurrentPlayer() );
    std::vector<moveEval> evals;
    if(g.gameOver()){
      if(g.getWinner()==0){
        return evaluateDraw(g);
      }
      return (g.getWinner()==g.getCurrentPlayer()) ? likeatrillion : -likeatrillion;
    }
    for(std::vector<std::vector<Position> >::iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
      double value=0;
      Game newstate(g);
      newstate.executeMove(*p_iter);

      value=eval(newstate);
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

/**
 *  Start negamax search for best move
 *
 *  Searches ahead for the best available move, some moves ahead of current position.
 *  Negamax is essentially a special case of minimax, moves are chosen to minimize the worst-case loss.
 *  Alpha-Beta pruning is implemented to speed up search.
 *  \n\n Starts the tree, calls negamax() method which recursivel calls itself to produce evaluation
 *  of favourability of each move.
 *  Returns whichever move is considered best.
 *
 *  @param g the Game to be evaluated
 *  @param depth how many moves ahead to look
 *  @return the chosen move and its score
 */
moveEval CheckyrsAI::rootNegamax(const Game &g, const int depth) const{
  std::vector<moveEval> moves;
  double alpha = -INFINITY;
  double beta = INFINITY;
  try{
    std::vector<std::vector<Position> > p = g.getMovesForPlayer(m_player);
    std::cout << "AI player " << (m_player==1? "1" : "2") << " is thinking: " << p.size() << " initial moves from this point\n";
    if(p.size()==1) return std::make_pair(p.at(0),likeatrillion); //don't waste time evaluating tree if there's only one branch
    for(std::vector<std::vector<Position> >::iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
      Game newstate(g);
      newstate.executeMove((*p_iter));
      double value;
      if(newstate.gameOver()) value = -evaluateGameOver(newstate);
      else value = -negamax(newstate, depth-1, -beta, -alpha);
      moves.push_back(std::make_pair((*p_iter),value));
    }
  }
  catch(std::exception &e){
    throw e;
  }

  std::sort(moves.begin(),moves.end(),sortMoveEvals);

  return moves.at(0);
}

/**
 *  Extend negamax search
 *
 *  Recursively calls itself, iterating a tree of all possible game paths for the chosen depth.
 *  Finds path with best outcome for this AI, assuming opponent always chooses their best move.
 *
 *  @param g the Game to be evaluated
 *  @param depth how many moves ahead to search
 *  @param alpha parameter for alpha-beta pruning
 *  @param beta parameter for alpha-beta pruning
 *  @return ultimate evaluation of this node
 */
double CheckyrsAI::negamax(Game g, const int depth, double alpha, double beta) const{
  try{
    if(depth<=0){
      return evalNode(g);
    }
    std::vector<std::vector<Position> > p = g.getMovesForPlayer( g.getCurrentPlayer() );
    if(g.gameOver()){
      return evaluateDraw(g);
    }
    for(std::vector<std::vector<Position> >::iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
      Game newstate(g);
      newstate.executeMove(*p_iter);
      alpha = fmax(alpha,-negamax(newstate,depth-1, -beta, -alpha));
      if(alpha>=beta) break;
    }
    return alpha;
  }catch(std::exception &e){
    throw e;
  }
}
