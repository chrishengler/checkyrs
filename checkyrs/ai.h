//
//  ai.h
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef __checkyrs__ai__
#define __checkyrs__ai__

#include <stdio.h>
#include "boost/random.hpp"

#include "board.h"
#include "game.h"
#include "position.h"

typedef std::pair<std::vector<Position>,double> moveEval;
typedef boost::mt19937 boost_rng;

class CheckyrsAI { //CheckyrsArtificialIdiot
  int m_player;
  
  //preference for attacking opponent's pieces vs maintaining own pieces
  double m_aggression;
  double m_possession;

  //point in game at which to encourage moving men forward/weight & offset for doing so
  int m_pushmen;
  double m_pushweight;
  double m_push_offset;
  int m_push_max; //point at which to no longer force that
  
  //weight for kings vs normal pieces
  double m_kingweight;
  double m_normweight;

  //weights for position: advancement, distance from sides/ends/corner of board
  double m_advweight;
  double m_sideweight;
  double m_endweight;
  double m_cornerweight;
  
  //offsets for each of the above weights
  double m_adv_offset;
  double m_side_offset;
  double m_end_offset;
  double m_corner_offset;
  
  //thresholds for where position bonuses are applied
  int m_adv_max;
  int m_adv_min;
  int m_side_min;
  int m_side_max;
  int m_end_min;
  int m_end_max;
  int m_corner_min;
  int m_corner_max;
  
  //weights for situational bonuses/maluses
  double m_threatweight_cancapture;
  double m_threatweight_limited;
  double m_threatweight;
  double m_threatweight_extreme;
  double m_captureweight;
  double m_crownweight;
  double m_defweight;
  double m_def_offset;
  int m_def_max;
  
  double m_material_bonus;
  double m_king_bonus;
  
  mutable boost_rng m_rng;
  
  double getRandomDouble(const double min=-1, const double max=1) const;
  int getRandomInt(const int min=0, const int max=8) const;
  
  void randomiseDouble(double *var, const double min=-1, const double max=1);
  void randomiseDoubles(std::vector<double*> &vars, const double min=-1, const double max=1);
  
  void gene(int *gene, const int p1, const int p2, int min, int max, const float mutate){
    boost::uniform_int<> int_dist(0,1);
    boost::variate_generator<boost_rng&, boost::uniform_int<> > gen(m_rng,int_dist);
    if(gen()<mutate){
      randomiseInt(gene,min,max);
      return;
    }
    else{
      if(gen() > 0.5) *gene = p1;
      else *gene = p2;
    }
    return;
  }
  
  void gene(double *gene, const double p1, const double p2, double min, double max, const float mutate){
    boost::uniform_int<> int_dist(0,1);
    boost::variate_generator<boost_rng&, boost::uniform_int<> > gen(m_rng,int_dist);
    if(gen()<mutate){
      randomiseDouble(gene,min,max);
    }
    else{
      if(gen() < 0.5) *gene = p1;
      else *gene = p2;
    }
    return;
  }

  void gene(std::pair<int*,int*> gene, const std::pair<int,int> p1, const std::pair<int,int> p2, const int min, const int max, const float mutate){
    boost::uniform_int<> int_dist(0,1);
    boost::variate_generator<boost_rng&, boost::uniform_int<> > gen(m_rng,int_dist);
    if(gen()<mutate){
      randomOrderedIntPair(gene);
    }
    else{
      if(gen() < 0.5){
        *(gene.first) = p1.first;
        *(gene.second) = p1.second;
      }
      else{
        *(gene.first) = p2.first;
        *(gene.second) = p2.second;
      }
    }
    return;
  }
  
  void randomiseInt(int *var, const int min=0, const int max=7);
  void randomiseInts(std::vector<int*> &vars, const int min=0, const int max=7);
  void randomOrderedIntPair(std::pair<int*,int*> &vars, const int min=0, const int max=7);

  double negamax(Game g, const int depth, double alpha, double beta) const;
  double evalNode(const Game &g) const;

public:
  CheckyrsAI(const int player=1);
  
  CheckyrsAI breed(const CheckyrsAI &p2, float mutate=0.05);
  
  void Initialise(bool random=false);
  
  void randomiseAI();
  
  double eval(const Game &g) const;
  moveEval rootNegamax(const Game &g, const int depth) const;
  
  void setPlayer(const int player){ m_player=player; }
  int getPlayer(){ return m_player; }
};
#endif /* defined(__checkyrs__ai__) */
