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
    m_board.at(ii).resize(size);
  }
  
}