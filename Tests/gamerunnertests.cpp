//
//  gamerunnertests.cpp
//  checkyrs
//
//  Created by Chris Hengler on 09/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <stdio.h>
#include "catch.hpp"
#include "gamerunner.h"

TEST_CASE("initialise AIvAI game"){
  Gamerunner gr(0);
  REQUIRE_NOTHROW( gr.initialise() );
  REQUIRE_NOTHROW( gr.continueGame() );
}

TEST_CASE("initialise 2 player game and play first moves"){
  Gamerunner gr(2);
  REQUIRE_NOTHROW( gr.initialise() );
  REQUIRE_NOTHROW( gr.continueGame() );
  REQUIRE( gr.getGame().getCurrentPlayer() == -1 );
  REQUIRE_NOTHROW( gr.continueGame() );
  REQUIRE( gr.getGame().getCurrentPlayer() == 1 );
  REQUIRE_NOTHROW( gr.continueGame() );
  REQUIRE_NOTHROW( gr.continueGame() );
}