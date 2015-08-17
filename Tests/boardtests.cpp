//
//  boardtests.cpp
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include "catch.hpp"
#include "board.h"

TEST_CASE("Board can be created"){
  
  Board board;
  REQUIRE(&board!=0);
}

TEST_CASE("Board has correct size"){
  Board board(6);
  REQUIRE(board.getSize()==6);
}

TEST_CASE("Piece can be placed on board"){
  Board board;
  Position p(0,0);
  board.AddPiece(p);
  REQUIRE(board.SquareIsOccupied(p));
}

TEST_CASE("Can add multiple pieces at once"){
  Board board;
  Position p(0,0);
  Position p2(1,1);
  std::vector<Position> pieces = {p,p2};
  board.AddPieces(pieces);
  REQUIRE(board.SquareIsOccupied(p));
  REQUIRE(board.SquareIsOccupied(p2));
}

TEST_CASE("Piece can be moved"){
  Board board;
  Position initial(0,0);
  Position destination(1,1);
  board.AddPiece(initial);
  board.MovePiece(initial,destination);
  REQUIRE(board.SquareIsOccupied(destination) == true);
  REQUIRE(board.SquareIsOccupied(initial) == false);
}

TEST_CASE("Exception thrown if piece added off board"){
  Board board;
  REQUIRE_THROWS_AS(board.AddPiece(Position(-2,-2)),std::out_of_range);
}

TEST_CASE("Out of range exception thrown if piece moved off board"){
  Board board;
  Position initial(0,0);
  board.AddPiece(initial);
  Position destination(-2,-2);
  REQUIRE_THROWS_AS(board.MovePiece(initial,destination), std::out_of_range);
}

TEST_CASE("Can check for occupation of square"){
  Board board;
  Position p(0,0);
  board.AddPiece(p);
  REQUIRE(board.SquareIsOccupied(p) == true);
}

TEST_CASE("Cannot add piece to occupied square"){
  Board board;
  Position p(0,0);
  board.AddPiece(p);
  REQUIRE_THROWS_AS(board.AddPiece(p),std::runtime_error);
}

TEST_CASE("Cannot move piece that does not exist"){
  Board board;
  Position initial(0,0);
  Position destination(1,1);
  REQUIRE_THROWS_AS(board.MovePiece(initial,destination),std::runtime_error);
}

TEST_CASE("Cannot move piece to occupied square"){
  Board board;
  Position firstpiece(0,0);
  board.AddPiece(firstpiece);
  Position secondpiece(1,1);
  board.AddPiece(secondpiece);
  REQUIRE_THROWS_AS(board.MovePiece(firstpiece,secondpiece),std::runtime_error);
}

TEST_CASE("Can check if move involves jump"){
  Board board;
  Position p1(0,0);
  Position p2(1,1);
  Position p3(2,2);
  REQUIRE(board.wasJump(p1,p2) == false);
  REQUIRE(board.wasJump(p1,p3) == true);
}

TEST_CASE("Can check which square was jumped"){
  Board board;
  Position p1(0,0);
  Position p2(1,1);
  Position p3(2,2);
  REQUIRE_THROWS(board.getJump(p1,p2));
  REQUIRE(board.getJump(p1,p3)._x == 1);
  REQUIRE(board.getJump(p1,p3)._y ==1);
}

TEST_CASE("check number of kings"){
  Board board;
  REQUIRE( board.getNumKingsPlayer(1) == 0 );
  REQUIRE( board.getNumKingsPlayer(-1) == 0);
  Position p1(0,0);
  Position p2(2,2);
  Position p3(4,6);
  board.AddPiece(p1);
  board.AddPiece(p2,1,true);
  board.AddPiece(p3,-1,true);
  REQUIRE( board.getNumKingsPlayer(1) == 1 );
  REQUIRE( board.getNumKingsPlayer(-1) == 1);
  board.RemovePiece(p3);
  REQUIRE( board.getNumKingsPlayer(-1) == 0);
}