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
