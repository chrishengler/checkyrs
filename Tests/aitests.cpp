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

TEST_CASE("eval prefers fewer opposition pieces"){
  Game g1;
  Game g2;
  Position p1(0,0);
  Position p2(1,1);
  Position p3(7,7);
  Position p4(6,6);
  CheckyrsAI ai;
  g1.AddPiece(p1);
  g1.AddPiece(p2);
  g1.AddPiece(p3,2);
  g1.AddPiece(p4,2);
  g2.AddPiece(p1);
  g2.AddPiece(p2);
  g2.AddPiece(p3,2);
  REQUIRE( ai.eval(g1.getBoard()) < ai.eval(g2.getBoard()));
}

TEST_CASE("can evaluate all available moves, taking multiple pieces preferred to taking one"){
  Game g1;
  Position p1(2,2);
  Position p2(3,3);
  Position p3(6,2);
  Position p4(5,3);
  Position p5(5,5);
  CheckyrsAI ai;
  g1.AddPiece(p1);
  g1.AddPiece(p2,2);
  g1.AddPiece(p3);
  g1.AddPiece(p4,2);
  g1.AddPiece(p5,2);
  std::vector<std::vector<Position> > possibleMoves = g1.getMovesForPlayer(1);
  REQUIRE( ai.evalMoves(g1.getBoard(), possibleMoves, false).at(0).first.size()==3);
}