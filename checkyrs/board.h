//
//  board.h
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef __checkyrs__board__
#define __checkyrs__board__

#include <stdio.h>
#include <vector>
#include "piece.h"

class Board {
  int m_size;
  std::vector<std::vector<Piece> > m_board;
  
public:
  Board(int size=8);
  int getSize();
};

#endif /* defined(__checkyrs__board__) */
