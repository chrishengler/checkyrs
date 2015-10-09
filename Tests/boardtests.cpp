//
//  boardtests.cpp
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <unordered_map>

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
  board.addPiece(p);
  REQUIRE(board.squareIsOccupied(p));
}

TEST_CASE("Can add multiple pieces at once"){
  Board board;
  Position p(0,0);
  Position p2(1,1);
  std::vector<Position> pieces = {p,p2};
  board.addPieces(pieces);
  REQUIRE(board.squareIsOccupied(p));
  REQUIRE(board.squareIsOccupied(p2));
}

TEST_CASE("Piece can be moved"){
  Board board;
  Position initial(0,0);
  Position destination(1,1);
  board.addPiece(initial);
  board.movePiece(initial,destination);
  REQUIRE(board.squareIsOccupied(destination) == true);
  REQUIRE(board.squareIsOccupied(initial) == false);
}

TEST_CASE("Exception thrown if piece added off board"){
  Board board;
  REQUIRE_THROWS_AS(board.addPiece(Position(-2,-2)),std::out_of_range);
}

TEST_CASE("Out of range exception thrown if piece moved off board"){
  Board board;
  Position initial(0,0);
  board.addPiece(initial);
  Position destination(-2,-2);
  REQUIRE_THROWS_AS(board.movePiece(initial,destination), std::out_of_range);
}

TEST_CASE("Can check for occupation of square"){
  Board board;
  Position p(0,0);
  board.addPiece(p);
  REQUIRE(board.squareIsOccupied(p) == true);
}

TEST_CASE("Cannot add piece to occupied square"){
  Board board;
  Position p(0,0);
  board.addPiece(p);
  REQUIRE_THROWS_AS(board.addPiece(p),std::runtime_error);
}

TEST_CASE("Cannot move piece that does not exist"){
  Board board;
  Position initial(0,0);
  Position destination(1,1);
  REQUIRE_THROWS_AS(board.movePiece(initial,destination),std::runtime_error);
}

TEST_CASE("Cannot move piece to occupied square"){
  Board board;
  Position firstpiece(0,0);
  board.addPiece(firstpiece);
  Position secondpiece(1,1);
  board.addPiece(secondpiece);
  REQUIRE_THROWS_AS(board.movePiece(firstpiece,secondpiece),std::runtime_error);
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
  REQUIRE(board.getJump(p1,p3).m_x == 1);
  REQUIRE(board.getJump(p1,p3).m_y ==1);
}

TEST_CASE("check number of kings"){
  Board board;
  REQUIRE( board.getNumKingsPlayer(1) == 0 );
  REQUIRE( board.getNumKingsPlayer(-1) == 0);
  Position p1(0,0);
  Position p2(2,2);
  Position p3(4,6);
  board.addPiece(p1);
  board.addPiece(p2,1,true);
  board.addPiece(p3,-1,true);
  REQUIRE( board.getNumKingsPlayer(1) == 1 );
  REQUIRE( board.getNumKingsPlayer(-1) == 1);
  board.removePiece(p3);
  REQUIRE( board.getNumKingsPlayer(-1) == 0);
}

TEST_CASE("test equality of boards"){
  Board b1;
  Board b2;
  REQUIRE( b1 == b2 );
  REQUIRE_FALSE( b1!=b2 );
  Position p1(0,0);
  b1.addPiece(p1);
  REQUIRE_FALSE( b1==b2 );
  REQUIRE( b1!=b2 );
  b2.addPiece(p1);
  REQUIRE( b1==b2 );
  REQUIRE_FALSE( b1!=b2 );
}


TEST_CASE("create/fill/examine unordered map of games"){
  Board b1;
  Board b2;
  Board b3;
  Board b4;
  Position p1(0,0);
  Position p2(2,2);
  Position p3(3,3);
  b1.addPiece(p1);
  b2.addPiece(p2);
  b3.addPiece(p3);
  b4.addPiece(p1);
  std::unordered_map<Board, int> boards;
  REQUIRE_NOTHROW( boards.emplace(b1,1) );
  REQUIRE_NOTHROW( boards.emplace(b2,1) );
  REQUIRE( boards.find(b3) == boards.end() );
  REQUIRE( boards.find(b1) != boards.find(b2) );
  REQUIRE( boards.find(b4) == boards.find(b1) );
}