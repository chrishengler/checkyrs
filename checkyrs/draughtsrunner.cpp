//
//  gamerunner.cpp
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <iostream>
#include "draughtsrunner.h"

CheckyrsAI DraughtsRunner::createAI(const int &player) {
  CheckyrsAI ai(player);
  ai.initialise(m_cli.yn("use random AI? (y/n)"));
  return ai;
}

void DraughtsRunner::prepareGame(){
    m_game = Game();
    m_game.prepareBoard();
    m_cli = CLInterface();
}
