//
//  gamerunner.h
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef __checkyrs__draughtsrunner__
#define __checkyrs__draughtsrunner__

#include <stdio.h>

#include "gamerunner.h"

#include "draughts.h"
#include "draughtsai.h"
#include "draughtsclinterface.h"

/**
 *  Gamerunner class
 *
 *  Instances of this class contain a Game, CLInterface, and potentially CheckyrsAI objects\n
 *  Class creates CheckysAIs as needed, controls user interaction, manages game progress
 */
class DraughtsRunner : public Gamerunner {
  
  CheckyrsAI createAI(const int &player=1);
  
  void prepareGame();
};

#endif /* defined(__checkyrs__draughtsrunner__) */
