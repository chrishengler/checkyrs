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

  m_players=0;
  std::string input;
  std::cout << "Setting up game\n";
  
  if(m_cli.yn("player one human? (y/n)")){
    m_p1ai=false;
  }
  else{
    m_p1ai=true;
    m_ai1 = CheckyrsAI(1);
    m_ai1.Initialise(m_cli.yn("use random AI? (y/n)"));
  }
  
  if(m_cli.yn("player two human? (y/n)")){
    m_p2ai=false;
  }
  else{
    m_p2ai=true;
    m_ai2 = CheckyrsAI(-1);
    m_ai2.Initialise(m_cli.yn("use random AI? (y/n)"));
  }
  
  m_game = Game();
  m_game.PrepareBoard();
  m_cli = CLInterface();
  
}

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
  
  m_game = Game();
  m_game.PrepareBoard();
  m_cli = CLInterface();
  
}


std::vector<Position> Gamerunner::getNextPlayerMove() const{
  m_cli.printBoard(m_game.getBoard());
  try{
    int evaldepth;
    int piecesremaining = m_game.getNumPiecesPlayer(m_game.getCurrentPlayer());
    if(piecesremaining < 5) evaldepth=6;
    else if(piecesremaining < 10) evaldepth=5;
    else evaldepth=4;
    if(m_game.getCurrentPlayer() == 1){
      if(m_p1ai){
        std::vector<Position> move = m_ai1.rootNegamax(m_game, evaldepth).first;
        std::cout << "AI player 1 move:\n";
        m_cli.printMove(move);
        return move;
      }
      else{
        return m_cli.getMove(m_game);
      }
    }
    else if(m_game.getCurrentPlayer() == -1){
      if(m_p2ai){
        std::vector<Position> move = m_ai2.rootNegamax(m_game, evaldepth).first;
        std::cout << "AI player 2 move:\n";
        m_cli.printMove(move);
        return move;
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
    std::cout << "\n\nGame over after " << m_game.getCurrentTurn() << " turns\nResult: ";
    if(m_game.getWinner()==0){
      if(m_game.isStale()){
        std::cout << "draw declared due to no pieces being taken or crowned in last " << m_game.getMaxStaleness() << " moves\n";
      }
      std::cout << "draw\n";
    }
    else std::cout << "player " << (m_game.getWinner()==1 ? "1" : "2") << " won\n";
    std::cout << "final board:\n";
    m_cli.printBoard(m_game.getBoard());
    return false;
  }
  std::cout << " - - - - - - - - - - - - - - -";
  std::cout << "\nTurn " << m_game.getCurrentTurn()+1 << " (player " << (m_game.getCurrentPlayer()==1 ? "1" : "2") << " to move):";
  std::cout << "\nMoves with no pieces taken or crowned: " << m_game.getStaleness() << " (draw at " << m_game.getMaxStaleness() << ")\n";
  std::vector<Position> move;
  try{
    move = getNextPlayerMove();
  }
  catch(std::exception &e){
    std::cout << "caught exception when trying to continue game:\n" << e.what() << "\n";
    std::cout << "could not continue\n\n\n";
    return false;
  }
  if(move.size()<2){
    std::cout << "No valid move found\n";
  }
  m_game.ExecuteMove(move);

  return true;
}

bool Gamerunner::gameOver() const{
  return m_game.gameOver();
}

bool Gamerunner::isDraw() const{
  return m_game.isStale();
}

int Gamerunner::getWinner() const{
  return m_game.getWinner();
}

int Gamerunner::getPiecesPlayer(const int player) const{
  return m_game.getNumPiecesPlayer(player);
}