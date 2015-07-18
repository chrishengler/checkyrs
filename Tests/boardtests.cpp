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

TEST_CASE("Can get list of possible moves"){
  Board board;
  Position firstpiece(0,0);
  board.AddPiece(firstpiece);
  REQUIRE(board.getMovesFrom(firstpiece).size() == 1);
}

TEST_CASE("List only includes positions on board"){
  Board board;
  Position firstpiece(0,0);
  board.AddPiece(firstpiece);
  REQUIRE(board.getMovesFrom(firstpiece).size() == 1);
}

TEST_CASE("Movelist does not include occupied squares"){
  Board board;
  Position firstpiece(0,0);
  board.AddPiece(firstpiece);
  Position secondpiece(1,1);
  board.AddPiece(secondpiece);
  REQUIRE(board.getMovesFrom(firstpiece).size()==0);
}