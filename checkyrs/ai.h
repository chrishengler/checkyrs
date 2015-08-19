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
#include "board.h"
#include "game.h"
#include "position.h"

typedef std::pair<std::vector<Position>,double> moveEval;


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
  
public:
  CheckyrsAI(const int player=1);
  
  void Initialise(bool random=false);
  
  void randomiseAI();
  
  double eval(const Game &g) const;
  double evalNode(const Game &g, const bool opp=false) const;
  moveEval rootNegamax(const Game &g, const int depth) const;
  double negamax(Game g, const int depth, const bool ownTurn) const;
  
  void randomiseDouble(double &var, const double min=-1, const double max=1){ var = min+(rand()*max); }
  void randomiseDoubles(std::vector<double> &vars, const double min=-1, const double max=1);
  
  void randomiseInt(int &var, const int min=0, const int max=7){ var = min+(ceil(rand()*max)); }
  void randomiseInts(std::vector<int> &vars, const int min=0, const int max=7);
  void randomOrderedIntPair(std::pair<int,int> &vars, const int min=0, const int max=7);
  
};
#endif /* defined(__checkyrs__ai__) */
