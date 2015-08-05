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

class CheckyrsAI {
  double m_aggression;
  double m_possession;
  int m_player;

public:
  CheckyrsAI(){m_aggression=5;m_possession=5;m_player=1;}
  CheckyrsAI(double agg,double pos,int player){m_aggression=agg,m_possession=pos;m_player=player;}
  
  double eval(const Board &b);
};
#endif /* defined(__checkyrs__ai__) */
