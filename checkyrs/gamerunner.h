//
//  gamerunner.h
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef __checkyrs__gamerunner__
#define __checkyrs__gamerunner__

#include <stdio.h>
#include "game.h"
#include "ai.h"
#include "clinterface.h"

class Gamerunner {
  Game m_game;
  int m_players;
  CheckyrsAI m_ai1;
  CheckyrsAI m_ai2;
  bool m_p1ai;
  bool m_p2ai;
  CLInterface cli;
  
public:
  Gamerunner(const int np=1);
  
  void initialise();
  
  void getNextPlayerMove();
  
};

#endif /* defined(__checkyrs__gamerunner__) */
