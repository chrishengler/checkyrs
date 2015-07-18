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
  board.AddPiece(1,1);
  REQUIRE(board.SquareIsOccupied(1,1));
  
}

TEST_CASE("Piece can be moved"){
  Board board;
  board.AddPiece(1, 1);
  board.MovePiece(1,1,2,2);
  REQUIRE(board.SquareIsOccupied(2, 2) == true);
  REQUIRE(board.SquareIsOccupied(1,1) == false);
}

TEST_CASE("Exception thrown if piece added off board"){
  Board board;
  REQUIRE_THROWS_AS(board.AddPiece(-2,-2),std::out_of_range);
}

TEST_CASE("Out of range exception thrown if piece moved off board"){
  Board board;
  board.AddPiece(1, 1);
  REQUIRE_THROWS_AS(board.MovePiece(1, 1, -2, -2), std::out_of_range);
}