//
//  board.cpp
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include "board.h"

Board::Board(int size){
  
  m_size=size;
  
  m_board.resize(size);
  for(int ii=0;ii<size;ii++){
    m_board[ii].resize(size);
    for(int jj=0;jj<size;jj++){
      m_board[ii][jj] = NULL;
    }
  }
}

bool Board::SquareIsOccupied(int x, int y){
  return (m_board[x][y]!=NULL);
}

void Board::AddPiece(int x, int y){
  m_board[x][y] = new Piece();
}

void Board::MovePiece(int oldx, int oldy, int newx, int newy){
  m_board[newx][newy]=m_board[oldx][oldy];
  m_board[oldx][oldy]=NULL;
}
