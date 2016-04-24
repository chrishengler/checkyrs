//
//  gamerunner.cpp
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <iostream>
#include "draughtsrunner.h"

CheckyrsAI DraughtsRunner::createAI(const int &player) {
  CheckyrsAI ai(player);
  ai.initialise(m_cli.yn("use random AI? (y/n)"));
  return ai;
}

void DraughtsRunner::prepareGame(){
    m_game = Draughts();
    m_game.prepareBoard();
    m_cli = CLInterface();
}

/**
 *  Continue game
 *
 *  Checks whether game is over, prints current board
 *
 *  @return true if game can continue, false if game complete
 */
bool DraughtsRunner::continueGame(){
  if(m_game.gameOver()){
    std::cout << "\n\nGame over after " << m_game.getCurrentTurn() << " turns\nResult: ";
    if(m_game.getWinner()==0){
      if(m_game.isStale()){
        std::cout << "draw declared due to no pieces being taken or crowned in last " << m_game.getMaxStaleness() << " moves\n";
      }
      else std::cout << "draw declared due to position repetition\n" << std::endl;
      std::cout << "draw\n";
    }
    else std::cout << "player " << (m_game.getWinner()==1 ? "1" : "2") << " won\n";
    std::cout << "final board:\n";
    m_cli.printBoard( (m_game.getBoard()) );
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
  m_game.executeMove(move);
  
  return true;
}

/**
 *  Gets next move
 *
 *  Gets next move for current player\n
 *  From player input via CLInterface if current player human\n
 *  Otherwise CheckyrsAI decides move
 */
std::vector<Position> DraughtsRunner::getNextPlayerMove() const{
  m_cli.printBoard( (m_game.getBoard()) );
  try{
    int evaldepth=5;
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

int DraughtsRunner::getCurrentPlayer() const{
  return m_game.getCurrentPlayer();
}

bool DraughtsRunner::gameOver() const{
  return m_game.gameOver();
}

bool DraughtsRunner::isDraw() const{
  return m_game.isStale();
}

int DraughtsRunner::getWinner() const{
  return m_game.Game::getWinner();
}

int DraughtsRunner::getPiecesPlayer(const int &player) const{
  return m_game.getNumPiecesPlayer(player);
}
