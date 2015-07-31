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
  REQUIRE(game.getMovesFrom(p1).at(0).size()==2);
}

TEST_CASE("Cannot jump over own pieces"){
  Game game;
  Position p1(0,0);
  game.AddPiece(p1);
  Position p2(1,1);
  game.AddPiece(p2,1,false);
  REQUIRE(game.getMovesFrom(p1).size()==0);
}

TEST_CASE("Can make multiple jumps in one move"){
  Game game;
  Position p1(0,0);
  game.AddPiece(p1);
  Position p2_1(1,1);
  Position p2_2(3,3);
  game.AddPiece(p2_1,2);
  game.AddPiece(p2_2,2);
  REQUIRE(game.getMovesFrom(p1).at(0).size()==3);
}

TEST_CASE("player 2 pieces move opposite direction"){
  Game game;
  Position p1(0,0);
  game.AddPiece(p1);
  Position p2(6,6);
  game.AddPiece(p2,2);
  REQUIRE( game.getMovesFrom(p2).at(0).at(1)._y==5);
}

TEST_CASE("Get a player's move options for multiple pieces"){
  Game game;
  Position p1(0,0);
  game.AddPiece(p1);
  Position p2(2,0);
  game.AddPiece(p2);
  REQUIRE( game.getMovesForPlayer(1).size()==3);
}

TEST_CASE("Executing move removes taken pieces"){
  Game game;
  Position p1(0,0);
  game.AddPiece(p1);
  Position p2(1,1);
  game.AddPiece(p2,2);
  REQUIRE( game.getNumPiecesPlayer(1)==1);
  REQUIRE( game.getNumPiecesPlayer(2)==1);
  std::vector<std::vector<Position> > possibleMoves = game.getMovesForPlayer(1);
  REQUIRE( game.getMovesForPlayer(1).size()==1);
  game.ExecuteMove( possibleMoves.at(0) );
  REQUIRE( game.getNumPiecesPlayer(1)==1);
  REQUIRE( game.getNumPiecesPlayer(2)==0);
}


