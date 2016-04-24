//
//  ai.h
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef __checkyrs__draughtsai__
#define __checkyrs__draughtsai__

#include <stdio.h>
#include <string>
#include <fstream>

#include "boost/random.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "draughts.h"

typedef std::pair<std::vector<Position>,double> moveEval;
typedef boost::mt19937 boost_rng;

/**
 *  CheckyrsAI class
 *
 *  AI that plays Checkyrs. Can generate new AIs with random parameters, this often produces Artificial Idiots.
 *  Chooses move based on Negamax algorithm with alpha-beta pruning of decision tree.
 */
class CheckyrsAI {
  friend class boost::serialization::access;
  int m_player;
  
  //preference for attacking opponent's pieces vs maintaining own pieces
  double m_aggression;
  double m_possession;

  //point in game at which to encourage moving men forward/Weight & offset for doing so
  int m_pushMen;
  double m_pushWeight;
  double m_pushOffset;
  int m_pushMax; //point at which to no longer force that
  
  //Weight for kings vs normal pieces
  double m_kingWeight;
  double m_normWeight;

  //Weights for position: advancement, distance from sides/ends/corner of board
  double m_advWeight;
  double m_sideWeight;
  double m_endWeight;
  double m_cornerWeight;
  
  //offsets for each of the above Weights
  double m_advOffset;
  double m_sideOffset;
  double m_endOffset;
  double m_cornerOffset;
  
  //thresholds for where position bonuses are applied
  int m_advMax;
  int m_advMin;
  int m_sideMin;
  int m_sideMax;
  int m_endMin;
  int m_endMax;
  int m_cornerMin;
  int m_cornerMax;
  
  //Weights for situational bonuses/maluses
  double m_threatWeight_cancapture;
  double m_threatWeight_limited;
  double m_threatWeight;
  double m_threatWeight_extreme;
  double m_captureWeight;
  double m_crownWeight;
  double m_defWeight;
  double m_defOffset;
  int m_defMax;
  
  double m_materialBonus;
  double m_kingBonus;
  
  mutable boost_rng m_rng;
  
  double getRandomDouble(const double min=-1, const double max=1) const;
  int getRandomInt(const int min=0, const int max=8) const;
  
  void randomiseDouble(double *var, const double min=-1, const double max=1);
  void randomiseDoubles(std::vector<double*> &vars, const double min=-1, const double max=1);
  
  /**
   *  Evolves an int gene from two parents
   *
   *  @param gene pointer to where this gene will be held
   *  @param p1 this gene in the first parent
   *  @param p2 this gene in the second parent
   *  @param min minimum value
   *  @param max maximum value
   *  @param mutate chance of mutating (i.e. taking random value rather than either parent)
   */
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
  
  /**
   *  Evolves a double gene from two parents
   *
   *  @param gene pointer to where this gene will be held
   *  @param p1 this gene in the first parent
   *  @param p2 this gene in the second parent
   *  @param min minimum value
   *  @param max maximum value
   *  @param mutate chance of mutating (i.e. taking random value rather than either parent)
   */
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

  /**
   *  Evolves an std::pair<int,int> gene from two parents
   *
   *  @param gene pointer to where this gene will be held
   *  @param p1 this gene in the first parent
   *  @param p2 this gene in the second parent
   *  @param min minimum value
   *  @param max maximum value
   *  @param mutate chance of mutating (i.e. taking random value rather than either parent)
   */
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

  double negamax(Draughts g, const int depth, double alpha, double beta) const;
  double evalNode(const Draughts &g) const;
  
  double evaluateGameOver(const Draughts &g) const;
  double evaluateDraw(const Draughts &g) const;
  
public:
  CheckyrsAI(const int player=1);
  
  CheckyrsAI breed(const CheckyrsAI &p2, float mutate=0.05);
  
  void initialise(bool random=false);
  
  void randomiseAI();
  
  double eval(const Draughts &g) const;
  moveEval rootNegamax(const Draughts &g, const int depth) const;
  
  /**
   *  Set Player
   *
   *  Which player this AI should be.
   *
   *  @param player '1' if first player, '-1' if second player
   */
  void setPlayer(const int player){ m_player=player; }
  
  /**
   *  Get Player
   *
   *  Which player is this AI?
   *
   *  @return '1' if first player, '-1' if second player
   */
  int getPlayer(){ return m_player; }

  template<class Archive>  void serialize(Archive &ar, const unsigned int version);
  void save(const std::string &filename);
  void load(const std::string &filename);
};
#endif /* defined(__checkyrs__draughtsai__) */
