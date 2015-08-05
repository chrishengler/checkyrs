//
//  aitests.cpp
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include "catch.hpp"
#include "game.h"
#include "ai.h"

TEST_CASE("eval prefers advanced pieces"){
  Game g1;
  Game g2;
  Game g3;
  Position p1(0,0);
  Position p2(1,1);
  Position p3(2,2);
  Position p4(2,0);
  CheckyrsAI ai;
  g1.AddPiece(p1);
  g1.AddPiece(p2);
  g1.AddPiece(p4);
  g2.AddPiece(p1);
  g2.AddPiece(p2);
  g2.AddPiece(p3);
  REQUIRE( ai.eval(g1.getBoard()) < ai.eval(g2.getBoard()));
}

