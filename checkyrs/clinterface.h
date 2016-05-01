//
//  clinterface.h
//  checkyrs
//
//  Created by Chris Hengler on 01/05/16.
//  Copyright (c) 2016 chrysics. All rights reserved.
//

#ifndef __checkyrs__clinterface__
#define __checkyrs__clinterface__

#include <stdio.h>
#include <boost/tokenizer.hpp>
#include "draughts.h"

/**
 *  CLInterface class
 *
 *  abstract class to provide command line interface for displaying games, related info, and getting input
 */
class CLInterface {
  
public:
  
  static bool yn(const std::string &s);
  static void showMenuHelp();
  
  virtual void showGameHelp();
  virtual void showRules() const = 0;
  virtual void pauseDisplay() const = 0;
  
  virtual void printSquare(const Square &s, const bool bs=true) const = 0;
  virtual void printBoard(const Board &b) const = 0;
  
  virtual void printMove(const std::vector<Position> &p) const = 0;
  virtual void printMoves(const std::vector<std::vector<Position> > &p) const = 0;
  
  Position interpretSquare(std::string &s) const;
  virtual std::vector<Position> interpretMove(const std::string &s) const = 0;
  virtual bool validateMove(const std::vector<Position> &p, Game *g) const = 0;
  
  virtual std::vector<Position> getMove(Game *g) const = 0;
};

#endif /* defined(__checkyrs__clinterface__) */
