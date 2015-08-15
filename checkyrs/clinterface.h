//
//  clinterface.h
//  checkyrs
//
//  Created by Chris Hengler on 07/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef __checkyrs__clinterface__
#define __checkyrs__clinterface__

#include <stdio.h>
#include <boost/tokenizer.hpp>
#include "game.h"

class CLInterface {
  
public:
  void printSquare(const Square &s, const bool bs) const;
  void printBoard(const Board &b) const;
  
  void printMove(const std::vector<Position> &p) const;
  void printMoves(const std::vector<std::vector<Position> > &p) const;
  
  Position interpretSquare(std::string &s) const;
  std::vector<Position> interpretMove(const std::string &s) const;
};
#endif /* defined(__checkyrs__clinterface__) */
