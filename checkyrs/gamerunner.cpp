//
//  gamerunner.cpp
//  checkyrs
//
//  Created by Chris Hengler on 24/04/16.
//  Copyright (c) 2016 chrysics. All rights reserved.
//

#include "gamerunner.h"

CheckyrsAI Gamerunner::createAI(const int &player){
  CheckyrsAI ai(player);
  ai.initialise(CLInterface::yn("use random AI?"));
  return ai;
}

/**
 *  Initialise Gamerunner
 *
 *  Gamerunner created, no args.\n
 *  Will ask whether each player is human or CheckyrsAI\n
 *  Creates CheckyrsAIs if needed
 */
void Gamerunner::initialise(){
  
  m_players=0;
  std::string input;
  std::cout << "Setting up game\n";
  
  if(CLInterface::yn("player one human? (y/n)")){
    m_p1ai=false;
    m_players++;
  }
  else{
    m_p1ai=true;
    m_ai1=this->createAI(1);
  }
  
  if(CLInterface::yn("player two human? (y/n)")){
    m_p2ai=false;
    m_players++;
  }
  else{
    m_p2ai=true;
    m_ai2=this->createAI(-1);
  }
  
  prepareGame();
  
}

/**
 * initialise Gamerunner for CheckyrsAI vs CheckyrsAI
 *
 * @param ai1 CheckyrsAI to move first
 * @param ai2 CheckyrsAI to move second
 */
void Gamerunner::initialise(CheckyrsAI &ai1, CheckyrsAI &ai2){
  
  m_players=0;
  std::string input;
  std::cout << "Setting up game\n";
  
  m_p1ai=true;
  m_ai1 = ai1;
  m_ai1.setPlayer(1);
  
  m_p2ai=true;
  m_ai2 = ai2;
  m_ai2.setPlayer(-1);
  
  prepareGame();
  
}