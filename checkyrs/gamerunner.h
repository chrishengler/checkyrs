//
//  gamerunner.h
//  checkyrs
//
//  Created by Chris Hengler on 24/04/16.
//  Copyright (c) 2016 chrysics. All rights reserved.
//

#ifndef __checkyrs__gamerunner__
#define __checkyrs__gamerunner__

#include <stdio.h>

#include "draughts.h"
#include "draughtsai.h"
#include "draughtsclinterface.h"




class Gamerunner {
protected:
  int m_players;
  CheckyrsAI m_ai1;
  CheckyrsAI m_ai2;
  bool m_p1ai;
  bool m_p2ai;
  CLInterface m_cli;
  
public:
  Gamerunner() {};
  
  virtual std::vector<Position> getNextPlayerMove() const = 0;
  virtual bool continueGame() = 0;
  virtual CheckyrsAI createAI(const int &player=1) = 0;
  virtual void prepareGame() = 0;
  
  /**
   *  Get the Game object
   *
   *  @return Game managed by this Gamerunner
   */
  //virtual const Game* getGame() const = 0;
  
  /**
   *  Check if player is AI
   *
   *  @param p which player to check\n'1' is first player, anything else is second player
   *  @return whether the player is AI
   */
  bool playerIsAI(const int p){ if(p==1) return m_p1ai; return m_p2ai; }
  
  void initialise();
  void initialise(CheckyrsAI &ai1, CheckyrsAI &ai2);

  virtual int getCurrentPlayer() const  = 0;
  virtual bool gameOver() const         = 0;
  virtual bool isDraw() const           = 0;
  virtual int getWinner() const         = 0;
  
  virtual int getPiecesPlayer(const int &player=1) const = 0;

  virtual ~Gamerunner() {};
};

#endif /* defined(__checkyrs__gamerunner__) */
