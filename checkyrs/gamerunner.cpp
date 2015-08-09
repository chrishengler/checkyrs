//
//  gamerunner.cpp
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include "gamerunner.h"

void Gamerunner::initialise(){
  m_game = Game();
  
  if(m_players==0){
    m_p1ai = true; m_p2ai = true;
    m_ai1 = CheckyrsAI(5, 5, 1);
    m_ai2 = CheckyrsAI(7, 3, -1);
  }
  
}