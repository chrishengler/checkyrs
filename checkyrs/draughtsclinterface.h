//
//  clinterface.h
//  checkyrs
//
//  Created by Chris Hengler on 07/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef __checkyrs__draughtsclinterface__
#define __checkyrs__draughtsclinterface__

#include <stdio.h>
#include <boost/tokenizer.hpp>

#include "clinterface.h"
#include "draughts.h"

/**
 *  CLInterface class
 *
 *  provides command line interface for displaying games, related info, and getting input
 */
class DraughtsCLInterface : public CLInterface {
  
public:

  void showGameHelp() const;
  void showRules() const;
  void pauseDisplay() const;
  
  void printSquare(const Square &s, const bool bs=true) const;
  void printBoard(const Board &b) const;
  
  void printMove(const std::vector<Position> &p) const;
  void printMoves(const std::vector<std::vector<Position> > &p) const;
  
  std::vector<Position> interpretMove(const std::string &s) const;
  bool validateMove(const std::vector<Position> &p, Game *g) const;
  
  std::vector<Position> getMove(Game *g) const;
};
#endif /* defined(__checkyrs__draughtsclinterface__) */
