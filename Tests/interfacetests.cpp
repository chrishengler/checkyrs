//
//  display.cpp
//  checkyrs
//
//  Created by Chris Hengler on 07/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <stdio.h>
#include "catch.hpp"
#include "draughtsclinterface.h"

TEST_CASE("print square"){
  CLInterface interface;
  Draughts d;
  d.prepareBoard();
  Position p1(0,0);
  Position p2(1,1);
  Position p3(7,7);
  Position p4(5,5);
  Position p5(1,0);
  Position p6(6,6);
  
  REQUIRE_NOTHROW( interface.printSquare(d.getBoard().getSquare(p1), true)  );
  REQUIRE_NOTHROW( interface.printSquare(d.getBoard().getSquare(p2), true)  );
  REQUIRE_NOTHROW( interface.printSquare(d.getBoard().getSquare(p3), true)  );
  REQUIRE_NOTHROW( interface.printSquare(d.getBoard().getSquare(p4), true)  );
  REQUIRE_NOTHROW( interface.printSquare(d.getBoard().getSquare(p5), false) );
  REQUIRE_NOTHROW( interface.printSquare(d.getBoard().getSquare(p6), true)  );
}

TEST_CASE("print board"){
  CLInterface interface;
  Draughts d;
  Position p1(0,0);
  Position p2(1,1);
  Position p3(2,2);
  Position p4(3,3);
  Position p5(1,0);
  Position p6(2,0);
  d.getBoard().addPiece(p1);
  d.getBoard().addPiece(p2,1,true);
  d.getBoard().addPiece(p3,-1);
  d.getBoard().addPiece(p4,-1,true);
  
  REQUIRE_NOTHROW( interface.printBoard( d.getBoard() ) );
}

TEST_CASE("print board at game start"){
  CLInterface interface;
  Draughts g;
  g.prepareBoard();
  REQUIRE_NOTHROW( interface.printBoard( g.getBoard() ) );
}

TEST_CASE("correctly print oversized board"){
  CLInterface interface;
  Draughts g(12);
  g.prepareBoard();
  
  REQUIRE_NOTHROW( interface.printBoard( g.getBoard() ) );
}

TEST_CASE("print move"){
  CLInterface interface;
  Draughts g;
  g.prepareBoard();
  REQUIRE_NOTHROW( interface.printMove( g.getMovesForPlayer(1).at(0)));
}

TEST_CASE("print all possible moves for piece/player"){
  CLInterface interface;
  Draughts g;
  g.prepareBoard();
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
  Draughts g1;
  g1.prepareBoard();
  std::string singlemove("a3 b4");
  std::string jumpmove("a3 c5");
  std::string invalidmove("a1 a3");
  REQUIRE( interface.validateMove(interface.interpretMove(singlemove),g1) == true);
  REQUIRE( interface.validateMove(interface.interpretMove(invalidmove),g1) == false);
  g1.executeMove(interface.interpretMove(singlemove));
  interface.printBoard(g1.getBoard());
  g1.executeMove(interface.interpretMove("d6 c5"));
  REQUIRE( interface.validateMove(interface.interpretMove(singlemove),g1) == false);
  REQUIRE( interface.validateMove(interface.interpretMove("b4 d6"),g1) == true );
}

TEST_CASE("get user input for move"){
  CLInterface interface;
  Draughts g1;
  g1.prepareBoard();
  interface.printBoard( g1.getBoard() );
  REQUIRE_NOTHROW( interface.getMove(g1) );
}

TEST_CASE("display help"){
  CLInterface interface;
  REQUIRE_NOTHROW( interface.showMenuHelp() );
}

TEST_CASE("display game help"){
  CLInterface interface;
  REQUIRE_NOTHROW( interface.showGameHelp() );
}

TEST_CASE("display rules"){
  CLInterface interface;
  REQUIRE_NOTHROW( interface.showRules() );
}

