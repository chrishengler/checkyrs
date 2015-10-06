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
  game.addPiece(p1);
  Game copy(game);
  REQUIRE(copy.getBoard().squareIsOccupied(p1));
}

TEST_CASE("test for game equality"){
  Game g1;
  g1.prepareBoard();
  Game g2;
  g2.prepareBoard();
  Game g3;
  REQUIRE( g1 == g1 );
  REQUIRE_FALSE( g1 != g1 );
  REQUIRE( g1 == g2 );
  REQUIRE_FALSE( g1 != g2 );
  REQUIRE( g1 != g3 );
  REQUIRE_FALSE( g1 == g3 );
}

TEST_CASE("Can get list of possible moves"){
  Game game;
  Position firstpiece(0,0);
  game.addPiece(firstpiece);
  REQUIRE(game.getMovesFrom(firstpiece).size() > 0);
}

TEST_CASE("List only includes positions on board"){
  Game game;
  Position firstpiece(0,0);
  game.addPiece(firstpiece);
  REQUIRE(game.getMovesFrom(firstpiece).size() == 1);
}

TEST_CASE("Can jump over neighbouring pieces"){
  Game game;
  Position p1(0,0);
  game.addPiece(p1);
  Position p2(1,1);
  game.addPiece(p2,-1,false);
  REQUIRE(game.getMovesFrom(p1).at(0).size()==2);
}

TEST_CASE("Cannot jump over own pieces"){
  Game game;
  Position p1(0,0);
  game.addPiece(p1);
  Position p2(1,1);
  game.addPiece(p2,1,false);
  REQUIRE(game.getMovesFrom(p1).size()==0);
}

TEST_CASE("Can make multiple jumps in one move"){
  Game game;
  Position p1(0,0);
  game.addPiece(p1);
  Position p2_1(1,1);
  Position p2_2(3,3);
  game.addPiece(p2_1,-1);
  game.addPiece(p2_2,-1);
  REQUIRE(game.getMovesFrom(p1).at(0).size()==3);
}

TEST_CASE("player 2 pieces move opposite direction"){
  Game game;
  Position p1(0,0);
  game.addPiece(p1);
  Position p2(6,6);
  game.addPiece(p2,-1);
  REQUIRE( game.getMovesFrom(p2).at(0).at(1).m_y==5);
}

TEST_CASE("Get a player's move options for multiple pieces"){
  Game game;
  Position p1(0,0);
  game.addPiece(p1);
  Position p2(2,0);
  game.addPiece(p2);
  REQUIRE( game.getMovesForPlayer(1).size()==3);
}

TEST_CASE("Executing move removes taken pieces"){
  Game game;
  Position p1(0,0);
  game.addPiece(p1);
  Position p2(1,1);
  game.addPiece(p2,-1);
  REQUIRE( game.getNumPiecesPlayer(1)==1);
  REQUIRE( game.getNumPiecesPlayer(-1)==1);
  std::vector<std::vector<Position> > possibleMoves = game.getMovesForPlayer(1);
  REQUIRE( game.getMovesForPlayer(1).size()==1);
  game.executeMove( possibleMoves.at(0) );
  REQUIRE( game.getNumPiecesPlayer(1)==1);
  REQUIRE( game.getNumPiecesPlayer(-1)==0);
}

TEST_CASE("Game ends when team has no pieces left"){
  Game game;
  Position p1(0,0);
  game.addPiece(p1);
  Position p2(1,1);
  game.addPiece(p2,-1);
  std::vector<std::vector<Position> > possibleMoves = game.getMovesForPlayer(1);
  game.executeMove(possibleMoves.at(0));
  REQUIRE(game.gameOver()  == true);
  REQUIRE(game.getWinner() == 1);
}

TEST_CASE("If jumps available, non-jumping moves not possible"){
  Game game;
  Position p1(2,2);
  game.addPiece(p1);
  Position p2(3,3);
  game.addPiece(p2,-1);
  REQUIRE(game.getMovesFrom(p1).size() == 1);
  Position p3(5,3);
  game.addPiece(p3);
  REQUIRE(game.getMovesForPlayer(1).size() == 1);
}

TEST_CASE("Get all moves for player 2"){
  Game game;
  Position p1(6,6);
  game.addPiece(p1,-1);
  Position p2(4,6);
  game.addPiece(p2,-1);
  Position p3(5,5);
  game.addPiece(p3);
  Position p4(2,6);
  game.addPiece(p4,-1);
  REQUIRE(game.getMovesForPlayer(-1).size()==2);
  REQUIRE(game.getMovesForPlayer(-1).at(0).at(1).m_y == 4);
}

TEST_CASE("pieces reaching final rank get kinged"){
  Game game;
  Position p1(6,6);
  Position p1target(7,7);
  Position p2(1,1);
  Position p2target(0,0);
  game.addPiece(p1);
  game.addPiece(p2,-1);
  std::vector<Position> p1move = {p1,p1target};
  std::vector<Position> p2move = {p2,p2target};
  game.executeMove(p1move);
  game.executeMove(p2move);
  REQUIRE( game.getBoard().squareHasKing(p1target) == true );
  REQUIRE( game.getBoard().squareHasKing(p2target) == true );
}

TEST_CASE("mid-move king promotion"){
  Game game;
  Position p1(5,5);
  Position p2(4,6);
  Position p3(2,6);
  game.addPiece(p1);
  game.addPiece(p2,-1);
  game.addPiece(p3,-1);
  std::vector<Position> move = game.getMovesForPlayer(1).at(0);
  REQUIRE( move.size() == 3 );
  game.executeMove(move);
  REQUIRE( game.gameOver() == true );
  REQUIRE( game.getWinner() == 1 );
}

TEST_CASE("prepare board"){
  Game game;
  game.prepareBoard();
  REQUIRE( game.getBoard().squareIsOccupied(Position(1,1)) == true );
  REQUIRE( game.getBoard().getPlayer(Position(1,1)) == 1 );
  REQUIRE( game.getBoard().squareIsOccupied(Position(6,6)) == true );
  REQUIRE( game.getBoard().getPlayer(Position(6,6)) == -1 );
  REQUIRE_FALSE( game.getBoard().squareIsOccupied(Position(4,4)) );
}

TEST_CASE("make and prepare game nonstandard size"){
  Game game(12);
  REQUIRE_NOTHROW( game.getBoard().getSquare(Position(10,10)) );
  game.prepareBoard();
  REQUIRE( game.getBoard().squareIsOccupied(Position(11,11)) == true );
}

TEST_CASE("check if piece is threatened"){
  Game game;
  game.addPiece(Position(0,0));
  game.addPiece(Position(1,1),-1);
  game.addPiece(Position(2,0));
  game.addPiece(Position(0,2));
  game.addPiece(Position(2,2));
  game.addPiece(Position(3,3),-1,true);
  game.addPiece(Position(2,4));
  REQUIRE( game.pieceIsThreatened(Position(0,0)) == false );
  REQUIRE( game.pieceIsThreatened(Position(1,1)) == false );
  REQUIRE( game.pieceIsThreatened(Position(2,0)) == false );
  REQUIRE( game.pieceIsThreatened(Position(0,2)) == false );
  REQUIRE( game.pieceIsThreatened(Position(2,2)) == false );
  REQUIRE( game.pieceIsThreatened(Position(3,3)) == true  );
  REQUIRE( game.pieceIsThreatened(Position(2,4)) == true  );
}

TEST_CASE("check piece defence"){
  Game game;
  game.addPiece(Position(0,0));
  game.addPiece(Position(1,1));
  game.addPiece(Position(2,2));
  game.addPiece(Position(0,2));
  game.addPiece(Position(6,6));
  game.addPiece(Position(3,3),-1);
  REQUIRE( game.pieceDefence(Position(0,0)) == 4 );
  REQUIRE( game.pieceDefence(Position(1,1)) == 3 );
  REQUIRE( game.pieceDefence(Position(2,2)) == 1 );
  REQUIRE( game.pieceDefence(Position(0,2)) == 3 );
  REQUIRE( game.pieceDefence(Position(6,6)) == 0 );
}