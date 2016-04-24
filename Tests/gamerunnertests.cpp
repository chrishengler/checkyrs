//
//  gamerunnertests.cpp
//  checkyrs
//
//  Created by Chris Hengler on 09/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <stdio.h>
#include "catch.hpp"
#include "draughtsrunner.h"

TEST_CASE("Initialise game with provided AIs"){
  CheckyrsAI ai1;
  CheckyrsAI ai2;
  ai1.initialise();
  ai2.initialise();
  DraughtsRunner gr;
  REQUIRE_NOTHROW( gr.initialise(ai1,ai2) );
}

TEST_CASE("initialise 2 player game and play first moves"){
  DraughtsRunner gr;
  REQUIRE_NOTHROW( gr.initialise() );
  REQUIRE_NOTHROW( gr.continueGame() );
  REQUIRE( gr.getGame().getCurrentPlayer() == -1 );
  REQUIRE_NOTHROW( gr.continueGame() );
  REQUIRE( gr.getGame().getCurrentPlayer() == 1 );
}