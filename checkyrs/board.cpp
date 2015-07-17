//
//  board.cpp
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <string>
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
  if(x>=0 && x<=m_size && y>=0 && y<=m_size){
    m_board[x][y] = new Piece();
  }
  else{
    std::string errmsg("Attempted to add piece out of bounds:");
    errmsg+=x;
    errmsg+=",";
    errmsg+=y;
    throw std::out_of_range(errmsg);
  }
}

void Board::MovePiece(int oldx, int oldy, int newx, int newy){
  if(newx>=0 && newx <= m_size && newy>=0 && newy<=m_size){
    m_board[newx][newy]=m_board[oldx][oldy];
    m_board[oldx][oldy]=NULL;
  }
  else{
    std::string errmsg("Attempted to move piece out of bounds:");
    errmsg+=newx;
    errmsg+=",";
    errmsg+=newy;
    throw std::out_of_range(errmsg);
  }
}
