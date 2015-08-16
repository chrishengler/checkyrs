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
  std::vector<Position> home = {p1,p2,p4};
  std::vector<Position> advanced = {p1,p2,p3};
  g1.AddPieces(home);
  g2.AddPieces(advanced);
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
  std::vector<Position> player1 = {p1,p2};
  std::vector<Position> player2 = {p3,p4};
  g1.AddPieces(player1,player2);
  
  g2.AddPieces(player1);
  g2.AddPiece(p3,-1);
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
  std::vector<Position> player1 = {p1,p3};
  std::vector<Position> player2 = {p2,p4,p5};
  g1.AddPieces(player1,player2);
  std::vector<std::vector<Position> > possibleMoves = g1.getMovesForPlayer(1);
  REQUIRE( ai.evalNode(g1,false).first.size()==3);
}

TEST_CASE("can look ahead"){
  Game g1;
  Position p1(1,1);
  Position p2(3,3);
  Position p3(5,1);
  Position p4(6,2);
  Position p5(6,4);
  Position p6(6,6);
  Position p7(7,7);
  Position p8(2,4);
  std::vector<Position> player1 = {p1,p2,p3};
  std::vector<Position> player2 = {p4,p5,p6,p7,p8};
  g1.AddPieces(player1,player2);
  CheckyrsAI ai;
  moveEval bestMove = ai.rootNegamax(g1,2);
  REQUIRE( bestMove.first.size() == 2 );
  REQUIRE( bestMove.first.at(0)._x==3 );
  REQUIRE( bestMove.first.at(0)._y==3 );
  REQUIRE( bestMove.first.at(1)._x==1 );
  REQUIRE( bestMove.first.at(1)._y==5 );
}

TEST_CASE("look ahead terminates correctly when game is won"){
  Game g1;
  Position p1(2,0);
  Position p2(4,0);
  Position p3(2,2);
  Position p4(5,1);
  Position p5(5,3);
  std::vector<Position> player1 = {p1,p2};
  std::vector<Position> player2 = {p3,p4,p5};
  g1.AddPieces(player1,player2);
  CheckyrsAI ai1(5,5,1);
  CheckyrsAI ai2(5,5,-1);
  moveEval bestMove;
  REQUIRE_NOTHROW( bestMove = ai1.rootNegamax(g1,10) ); //ensure no exceptions when iterating deeper than remaining game path
  REQUIRE( bestMove.first.at(0)._x == 4 ); //ensure early termination doesn't mess up move selection
  REQUIRE_NOTHROW( bestMove = ai2.rootNegamax(g1,10) ); //same test for losing player
}