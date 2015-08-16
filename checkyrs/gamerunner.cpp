//
//  gamerunner.cpp
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <iostream>
#include "gamerunner.h"

void Gamerunner::initialise(){
  m_game = Game();
  m_game.PrepareBoard();
  m_cli = CLInterface();
  
  if(m_players==0){
    m_p1ai = true; m_p2ai = true;
    m_ai1 = CheckyrsAI(3, 7, 1);
    m_ai2 = CheckyrsAI(7, 3, -1);
  }
  else if(m_players==2){
    m_p1ai = false; m_p2ai = false;
  }
  
}

std::vector<Position> Gamerunner::getNextPlayerMove() const{
  m_cli.printBoard(m_game.getBoard());
  try{
    if(m_game.getCurrentPlayer() == 1){
      if(m_p1ai){
        return m_ai1.rootNegamax(m_game, 4).first;
      }
      else{
        return m_cli.getMove(m_game);
      }
    }
    else if(m_game.getCurrentPlayer() == -1){
      if(m_p2ai){
        return m_ai2.rootNegamax(m_game, 4).first;
      }
      else{
        return m_cli.getMove(m_game);
      }
    }
    else{
      std::string errmsg("cannot get move, no active player! current player = ");
      errmsg+=m_game.getCurrentPlayer();
      throw std::logic_error(errmsg);
    }
  }catch(std::exception &e){
    throw e;
  }
}

bool Gamerunner::continueGame(){
  if(m_game.gameOver()){
    std::cout << "\n\ngame over\nresult: ";
    if(m_game.getWinner()==0) std::cout << "draw\n";
    else std::cout << "player " << (m_game.getWinner()==1 ? "1" : "2") << " won\n";
    std::cout << "final board:\n";
    m_cli.printBoard(m_game.getBoard());
    return false;
  }
  std::vector<Position> move;
  try{
    move = getNextPlayerMove();
  }
  catch(std::exception &e){
    std::cout << "caught exception when trying to continue game:\n" << e.what() << "\n";
    std::cout << "could not continue\n\n\n";
    return false;
  }
  m_game.ExecuteMove(move);
  if(move.size()<2){
    std::cout << "no valid move found\n";
  }
  return true;
}