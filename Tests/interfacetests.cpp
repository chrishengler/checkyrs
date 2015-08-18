//
//  display.cpp
//  checkyrs
//
//  Created by Chris Hengler on 07/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <stdio.h>
#include "catch.hpp"
#include "clinterface.h"

TEST_CASE("print square"){
  CLInterface interface;
  Game g;
  Position p1(0,0);
  Position p2(1,1);
  Position p3(2,2);
  Position p4(3,3);
  Position p5(1,0);
  Position p6(2,0);
  g.AddPiece(p1);
  g.AddPiece(p2,1,true);
  g.AddPiece(p3,-1);
  g.AddPiece(p4,-1,true);
  
  REQUIRE_NOTHROW( interface.printSquare(g.getBoard().getSquare(p1), true));
  REQUIRE_NOTHROW( interface.printSquare(g.getBoard().getSquare(p2), true));
  REQUIRE_NOTHROW( interface.printSquare(g.getBoard().getSquare(p3), true));
  REQUIRE_NOTHROW( interface.printSquare(g.getBoard().getSquare(p4), true));
  REQUIRE_NOTHROW( interface.printSquare(g.getBoard().getSquare(p5), false));
  REQUIRE_NOTHROW( interface.printSquare(g.getBoard().getSquare(p6), true));
}

TEST_CASE("print board"){
  CLInterface interface;
  Game g;
  Position p1(0,0);
  Position p2(1,1);
  Position p3(2,2);
  Position p4(3,3);
  Position p5(1,0);
  Position p6(2,0);
  g.AddPiece(p1);
  g.AddPiece(p2,1,true);
  g.AddPiece(p3,-1);
  g.AddPiece(p4,-1,true);
  
  REQUIRE_NOTHROW( interface.printBoard(g.getBoard()) );
}

TEST_CASE("print board at game start"){
  CLInterface interface;
  Game g;
  g.PrepareBoard();
  REQUIRE_NOTHROW( interface.printBoard(g.getBoard()) );
}

TEST_CASE("correctly print oversized board"){
  CLInterface interface;
  Game g(12);
  g.PrepareBoard();
  
  REQUIRE_NOTHROW( interface.printBoard(g.getBoard()) );
}

TEST_CASE("print move"){
  CLInterface interface;
  Game g;
  g.PrepareBoard();
  REQUIRE_NOTHROW( interface.printMove( g.getMovesForPlayer(1).at(0)));
}

TEST_CASE("print all possible moves for piece/player"){
  CLInterface interface;
  Game g;
  g.PrepareBoard();
  REQUIRE_NOTHROW( interface.printMoves(g.getMovesFrom(Position(0,2))));
  REQUIRE_NOTHROW( interface.printMoves(g.getMovesForPlayer(1)));
}

TEST_CASE("interpret square"){
  CLInterface interface;
  std::string a2("a2");
  std::string a2upper("A2");
  std::string e5("e5");
  std::string ea("ea");
  std::string numfirst("6d");
  std::string empty("");
  REQUIRE( interface.interpretSquare(a2) == Position(0,1) );
  REQUIRE( interface.interpretSquare(a2upper) == Position(0,1) );
  REQUIRE( interface.interpretSquare(e5) == Position(4,4) );
  REQUIRE_THROWS( interface.interpretSquare(ea) );
  REQUIRE_THROWS( interface.interpretSquare(numfirst) );
  REQUIRE_THROWS( interface.interpretSquare(empty) );
}

TEST_CASE("interpret single move"){
  CLInterface interface;
  std::string validmove("a1 b2");
  std::string comma("a1, b2");
  std::string nospace("a1b2");
  std::string invalidpos("a1 bd");
  std::string invalidchar("!1 b2");
  std::string invalidnum("a! b2");
  std::string empty("");
  REQUIRE( interface.interpretMove(validmove).size() == 2 );
  REQUIRE( interface.interpretMove(validmove).at(0) == Position(0,0));
  REQUIRE( interface.interpretMove(validmove).at(1) == Position(1,1));
  REQUIRE( interface.interpretMove(comma).size() == 2 );
  REQUIRE( interface.interpretMove(validmove).at(0) == Position(0,0));
  REQUIRE( interface.interpretMove(validmove).at(1) == Position(1,1));
  REQUIRE_THROWS( interface.interpretMove(nospace) );
  REQUIRE_THROWS( interface.interpretMove(invalidpos) );
  REQUIRE_THROWS( interface.interpretMove(invalidchar) );
  REQUIRE_THROWS( interface.interpretMove(invalidnum) );
  REQUIRE_THROWS( interface.interpretMove(empty) );
}

TEST_CASE("interpret validity of move"){
  CLInterface interface;
  Game g1;
  g1.PrepareBoard();
  std::string singlemove("a3 b4");
  std::string jumpmove("a3 c5");
  std::string invalidmove("a1 a3");
  REQUIRE( interface.validateMove(interface.interpretMove(singlemove),g1) == true);
  REQUIRE( interface.validateMove(interface.interpretMove(invalidmove),g1) == false);
  g1.AddPiece( Position(1,3) , -1 );
  REQUIRE( interface.validateMove(interface.interpretMove(singlemove),g1) == false);
  REQUIRE( interface.validateMove(interface.interpretMove(jumpmove),g1) == true );
}

TEST_CASE("get user input for move"){
  CLInterface interface;
  Game g1;
  g1.PrepareBoard();
  interface.printBoard(g1.getBoard());
  REQUIRE_NOTHROW( interface.getMove(g1) );
}

TEST_CASE("display help"){
  CLInterface interface;
  REQUIRE_NOTHROW( interface.showMenuHelp() );
}


