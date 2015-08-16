//
//  aidebug.cpp
//  checkyrs
//
//  Created by Chris Hengler on 16/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <stdio.h>
#include "catch.hpp"
#include "game.h"
#include "ai.h"
#include "clinterface.h"
#include "position.h"

TEST_CASE("exception"){
  Game g = Game();
  CLInterface cli;
  CheckyrsAI ai(7, 3, -1);
  g.AddPiece(Position(1,1),-1);
  g.AddPiece(Position(2,2),-1,true);
  g.AddPiece(Position(5,1));
  g.AddPiece(Position(6,0));
  g.AddPiece(Position(7,1));
  g.AddPiece(Position(5,3));
  g.AddPiece(Position(0,4),1,true);
  g.AddPiece(Position(1,7),1,true);
  g.AddPiece(Position(5,7),1,true);
  REQUIRE_NOTHROW( ai.rootNegamax(g, 3) );
  REQUIRE_NOTHROW( ai.rootNegamax(g, 4) );
}