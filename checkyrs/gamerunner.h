//
//  gamerunner.h
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef __checkyrs__gamerunner__
#define __checkyrs__gamerunner__

#include <stdio.h>
#include "game.h"
#include "ai.h"
#include "clinterface.h"

/**
 *  Gamerunner class
 *
 *  Instances of this class contain a Game, CLInterface, and potentially CheckyrsAI objects\n
 *  Class creates CheckysAIs as needed, controls user interaction, manages game progress
 */
class Gamerunner {
  Game m_game;
  int m_players;
  CheckyrsAI m_ai1;
  CheckyrsAI m_ai2;
  bool m_p1ai;
  bool m_p2ai;
  CLInterface m_cli;
  
public:
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
  
  std::vector<Position> getNextPlayerMove() const;
  bool continueGame();
  
  bool gameOver() const;
  bool isDraw() const;
  int getWinner() const;
  
  int getPiecesPlayer(const int player=1) const;
};

#endif /* defined(__checkyrs__gamerunner__) */
