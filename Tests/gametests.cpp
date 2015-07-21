//
//  gametests.cpp
//  checkyrs
//
//  Created by Chris Hengler on 19/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include "catch.hpp"
#include "board.h"
#include "game.h"

TEST_CASE("Can copy game"){
  Game game;
  Position p1(0,0);
  game.AddPiece(p1);
  Game copy(game);
  REQUIRE(copy.getBoard().SquareIsOccupied(p1));
}

TEST_CASE("Can get list of possible moves"){
  Game game;
  Position firstpiece(0,0);
  game.AddPiece(firstpiece);
  REQUIRE(game.getMovesFrom(firstpiece).size() > 0);
}

TEST_CASE("List only includes positions on board"){
  Game game;
  Position firstpiece(0,0);
  game.AddPiece(firstpiece);
  REQUIRE(game.getMovesFrom(firstpiece).size() == 1);
}

TEST_CASE("Can jump over neighbouring pieces"){
  Game game;
  Position p1(0,0);
  game.AddPiece(p1);
  Position p2(1,1);
  game.AddPiece(p2,2,false);
  REQUIRE(game.getMovesFrom(p1).at(0).size()==1);
}

TEST_CASE("Can only jump over pieces of opponent"){
  Game game;
  Position p1(0,0);
  game.AddPiece(p1);
  Position p2(1,1);
  game.AddPiece(p2,1,false);
  REQUIRE(game.getMovesFrom(p1).size()==0);
}

