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

//not real unit tests, but building specific game situations to inspect how evaluation function works

TEST_CASE("sacrifice king for win"){
  Game g;
  CLInterface cli;
  g.AddPiece(Position(1,1),-1,true);
  g.AddPiece(Position(4,2),-1);
  g.AddPiece(Position(5,3),-1,true);
  g.AddPiece(Position(7,5),-1);
  g.AddPiece(Position(7,7),-1);
  g.AddPiece(Position(1,3),1,true);
  CheckyrsAI ai(-1);
  ai.Initialise(false);
  cli.printBoard(g.getBoard());
  moveEval chosenMove = ai.rootNegamax(g,4);
  REQUIRE( chosenMove.first.at(1)._y == 2);
  REQUIRE( chosenMove.first.at(1)._x == 2);
}

TEST_CASE("exception"){
  Game g = Game();
  CLInterface cli;
  CheckyrsAI ai(-1);
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
