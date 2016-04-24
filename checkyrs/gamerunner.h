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
  Game m_game;
  int m_players;
  CheckyrsAI m_ai1;
  CheckyrsAI m_ai2;
  bool m_p1ai;
  bool m_p2ai;
  CLInterface m_cli;
  
public:
  Gamerunner() {};
  
  virtual std::vector<Position> getNextPlayerMove() const;
  virtual CheckyrsAI createAI(const int &player=1);
  virtual void prepareGame();
  
  /**
   *  Get the Game object
   *
   *  @return Game managed by this Gamerunner
   */
  const Game getGame() const{return m_game;}
  
  /**
   *  Check if player is AI
   *
   *  @param p which player to check\n'1' is first player, anything else is second player
   *  @return whether the player is AI
   */
  bool playerIsAI(const int p){ if(p==1) return m_p1ai; return m_p2ai; }
  
  void initialise();
  void initialise(CheckyrsAI &ai1, CheckyrsAI &ai2);
  
  bool continueGame();
  bool gameOver() const;
  bool isDraw() const;
  int getWinner() const;
  
  int getPiecesPlayer(const int player=1) const;

  virtual ~Gamerunner() {};
};

#endif /* defined(__checkyrs__gamerunner__) */
