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
#include "clinterface.h"

TEST_CASE("eval prefers advanced pieces"){
  Game g1;
  Game g2;
  Position p1(0,0);
  Position p2(1,1);
  Position p3(2,2);
  Position p4(2,0);
  CheckyrsAI ai;
  ai.initialise(false);
  std::vector<Position> home = {p1,p2,p4};
  std::vector<Position> advanced = {p1,p2,p3};
  g1.addPieces(home);
  g2.addPieces(advanced);
  REQUIRE( ai.eval(g1) < ai.eval(g2) );
}

TEST_CASE("eval prefers fewer opposition pieces"){
  Game g1;
  Game g2;
  Position p1(0,0);
  Position p2(1,1);
  Position p3(7,7);
  Position p4(6,6);
  CheckyrsAI ai;
  ai.initialise(false);
  std::vector<Position> player1 = {p1,p2};
  std::vector<Position> player2 = {p3,p4};
  g1.addPieces(player1,player2);
  
  g2.addPieces(player1);
  g2.addPiece(p3,-1);
  REQUIRE( ai.eval(g1) < ai.eval(g2) );
}

TEST_CASE("can evaluate all available moves, taking multiple pieces preferred to taking one"){
  Game g1;
  Position p1(2,2);
  Position p2(3,3);
  Position p3(6,2);
  Position p4(5,3);
  Position p5(5,5);
  CheckyrsAI ai;
  ai.initialise(false);
  std::vector<Position> player1 = {p1,p3};
  std::vector<Position> player2 = {p2,p4,p5};
  g1.addPieces(player1,player2);
  std::vector<std::vector<Position> > possibleMoves = g1.getMovesForPlayer(1);
  REQUIRE( ai.rootNegamax(g1,1).first.size()==3);
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
  g1.addPieces(player1,player2);
  CheckyrsAI ai(1);
  ai.initialise(false);
  moveEval bestMove = ai.rootNegamax(g1,3);
  CLInterface m_cli;
  m_cli.printBoard(g1.getBoard());
  std::cout << "AI best move: \n";
  m_cli.printMove(bestMove.first);
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
  g1.addPieces(player1,player2);
  CheckyrsAI ai1(1);
  ai1.initialise(false);
  CheckyrsAI ai2(-1);
  ai2.initialise(false);
  moveEval bestMove;
  REQUIRE_NOTHROW( bestMove = ai1.rootNegamax(g1,10) ); //ensure no exceptions when iterating deeper than remaining game path
  REQUIRE( bestMove.first.at(0)._x == 4 ); //ensure early termination doesn't mess up move selection
  REQUIRE_NOTHROW( bestMove = ai2.rootNegamax(g1,10) ); //same test for losing player
}

TEST_CASE("create default and randomised AIs"){
  Game g1;
  g1.prepareBoard();
  
  CheckyrsAI ai1(1);
  REQUIRE_NOTHROW( ai1.initialise(true) );
  
  CheckyrsAI ai2(-1);
  REQUIRE_NOTHROW( ai2.initialise(false) );
  
  REQUIRE_NOTHROW( g1.ExecuteMove(ai1.rootNegamax(g1, 4).first) );
  REQUIRE_NOTHROW( g1.ExecuteMove(ai2.rootNegamax(g1, 4).first) );
}

TEST_CASE("serialize and deserialize AI"){
  Game g1;
  g1.prepareBoard();
  
  CheckyrsAI ai1(1);
  ai1.initialise(false);
  
  REQUIRE_NOTHROW(ai1.save("/tmp/ai1.cai") );
  
  CheckyrsAI ai2(1);

  std::vector<Position> ai1opener = ai1.rootNegamax(g1, 4).first;
  std::vector<Position> ai2opener;
  
  REQUIRE_NOTHROW( ai2.load("/tmp/ai1.cai") );
  REQUIRE_NOTHROW( ai2opener = ai2.rootNegamax(g1, 4).first );
  REQUIRE( ai2opener.at(0) == ai1opener.at(0) );
  REQUIRE( ai2opener.at(1) == ai1opener.at(1) );
}