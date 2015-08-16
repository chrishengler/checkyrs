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

  std::string input;
  std::cout << "Setting up game\nHow many human players? (0,1,2)\n";
  
  m_players=-1;
  do{
    try{
      std::cin >> input;
      m_players=stoi(input);
    }
    catch(std::invalid_argument &e){
      std::cout << "could not interpret \"" << input << "\" as integer, try again:\n";
      continue;
    }
    catch(std::exception &e){
      std::cout << "unexpected exception: " << e.what();
      throw e;
    }
    if(m_players>2){
      std::cout << "draughts is a two player game, idiot. Enter something better this time.\n";
    }
    else if(m_players<0){
      std::cout << "I don't know what you think negative players means but it's Not A Thing\u2122, try again\n";
    }
  }while(m_players<0 || m_players>2);
    
  if(m_players==0){
    std::cout << "preparing AI vs. AI game\n";
    m_p1ai = true; m_p2ai = true;
    m_ai1 = CheckyrsAI(3, 7, 1);
    m_ai2 = CheckyrsAI(7, 3, -1);
  }
  else if(m_players==2){
    std::cout << "preparing human vs. human game\n";
    m_p1ai = false; m_p2ai = false;
  }
  else if(m_players==1){
    std::cout << "preparing human vs. AI game\n";
    std::cout << "which player is human? (1,2)\n";
    int human=-1;
    bool valid=false;
    do{
      try{
        std::cin >> input;
        human = stoi(input);
      }
      catch(std::invalid_argument &e){
        std::cout << "could not interpret \"" << input << "\" as int, try again:\n";
        continue;
      }
      catch(std::exception &e){
        std::cout << "unexpected exception: " << e.what() << "\n";
      }
      if(human==1){
        valid = true;
        m_p1ai = false;
        m_p2ai = true;
        m_ai2  = CheckyrsAI(5,5,-1);
      }
      else if(human==2){
        valid = true;
        m_p1ai = true;
        m_p2ai = false;
        m_ai1  = CheckyrsAI(5,5,-1);
      }
      else{
        std::cout << "not an option, idiot, try again:\n";
      }
    }while(!valid);
  }
  
  m_game = Game();
  m_game.PrepareBoard();
  m_cli = CLInterface();
  
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