//
//  output.cpp
//  checkyrs
//
//  Created by Chris Hengler on 07/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <iostream>
#include "boost/algorithm/string.hpp"
#include "draughtsclinterface.h"

#define KNRM   "\x1B[0m"
#define KRED   "\x1B[31m"
#define KGRN   "\x1B[32m"
#define KYEL   "\x1B[33m"
#define KBLU   "\x1B[34m"
#define KMAG   "\x1B[35m"
#define KCYN   "\x1B[36m"
#define KWHT   "\x1B[37m"
#define BOLD   "\e[1m"
#define RESET  "\033[0m"

/**
 *  Pause display
 *
 *  Waits for keyboard input
 */
void DraughtsCLInterface::pauseDisplay() const{
  std::cout << "\npush enter to continue";
  std::cin.get();
  return;
}

/**
 *  Show help for game
 */
void DraughtsCLInterface::showGameHelp() const{
  DraughtsBoard b(3);
  Position p1p(1,0);
  Position p1k(1,1);
  Position p2p(2,0);
  Position p2k(2,1);
  b.addPiece(p1p);
  b.addPiece(p1k,1,true);
  b.addPiece(p2p,-1);
  b.addPiece(p2k,-1,true);
  std::cout << "\nThe board is displayed on screen at the start of each move\n";
  std::cout << "Empty squares are displayed as blue or white dots.\n";
  std::cout << "Player 1's pieces are displayed in cyan, and player 2's pieces are displayed in red\n";
  pauseDisplay();
  std::cout << "Normal pieces look like this: ";
  printSquare(b.getSquare(p1p));
  std::cout << " and like this: ";
  printSquare(b.getSquare(p2p));
  std::cout << "\nKings look like this: ";
  printSquare(b.getSquare(p1k));
  std::cout << " and like this: ";
  printSquare(b.getSquare(p2k));
  pauseDisplay();
  std::cout << "\nMoves are input in the following format:\n";
  std::cout << BOLD << "<current square> <target square>\n" << RESET;
  std::cout << "for example: \n";

  DraughtsBoard example(5);
  example.addPiece(Position(0,0));
  example.addPiece(Position(3,1));
  example.addPiece(Position(4,2),-1,true);
  Draughts g(example);
  printBoard(g.getBoard());
  std::cout << "player 1: " << BOLD << "a1 b2" << RESET << "\n";
  g.executeMove(interpretMove("a1 b2"));
  printBoard(g.getBoard());
  pauseDisplay();
  std::cout << "\nTo make longer moves when capturing multiple pieces, include all subsequent squares in the same single input. For example:";
  printBoard(g.getBoard());
  std::cout << "player 2: " << BOLD << "e3 c1 a3\n" << RESET;
  g.executeMove(interpretMove("e3 c1 a3"));
  printBoard(g.getBoard());
}

/**
 *  Show rules
 *
 *  There are a surprising number of variations on this game\n
 *  Version played in Checkyrs detailed here.
 */
void DraughtsCLInterface::showRules() const{
  std::cout << "\n\nRules\n\n";
  std::cout << "There are many variations on draughts/checkers.\n";
  std::cout << "The rules used by Checkyrs are detailed here\n";
  std::cout << "Each player starts with 12 pieces\n";
  std::cout << "Their pieces start on the dark squares of the three rows closest to that player's home row\n";
  std::cout << "At each turn, the player must move one of their pieces\n";
  std::cout << "Pieces move diagonally, one square at a time\n";
  std::cout << "Initially, pieces may only move towards the opponent's home row\n";
  std::cout << "Once a piece reaches the opponent's home row, it is crowned and may move backwards\n";
  std::cout << "An opposing piece is removed from the board by jumping over it\n";
  std::cout << "This is only possible from a neighbouring square, and when the square immediately beyond the captured piece is unoccupied\n";
  std::cout << "The capturing piece is then placed on the square immediately beyond the captured piece, thus moving two squares diagonally\n";
  std::cout << "If a capturing piece lands in a location where one or more further captures can be made by the same piece, the move must be extended until the piece is no longer able to capture any opposing pieces\n";
  std::cout << "If a player has moves available which capture one or more opposing pieces, one of those moves must be chosen\n";
  std::cout << "It is " << BOLD << "not" << RESET << " necessary to select the longest of all possible moves, but the chosen move must be extended until the piece is in a position from which it can make no further captures\n";
  std::cout << "Turns may not be skipped, the player is obliged to move when it is their turn\n";
  std::cout << "\n\nEnd of the Game\n\n";
  std::cout << "If a player captures all opposing pieces, they win the game\n";
  std::cout << "If the current player has no legal moves available, they lose the game\n";
  std::cout << "If no pieces are taken and no pieces are crowned for 50 turns, the game is declared a draw\n";
  std::cout << "A draw is also declared on the 4th repetition of the same game state\n";
}

/**
 *  Print the Square
 *  
 *  prints the Square (either appropriate piece or empty)
 *
 *  @param s Square to be printed
 *  @param bs bool indicating dark (blue) or light (white) square
 */
void DraughtsCLInterface::printSquare(const Square &s, const bool bs) const{
  std::string col;
  if(s.isOccupied()){
    col = (s.getPlayer() == 1 ? KCYN : KRED );
  }
  else{
    col = (bs ? KBLU : KWHT );
  }
  if(!s.isOccupied()){
    std::cout << col << "." << RESET;
    return;
  }
  if(s.isKing()){
    std::cout << col << "\u25CF" << RESET ;
    return;
  }
  std::cout << col << "o" << RESET;
  return;
}

/**
 *  Print board
 *
 *  Prints the board to console
 *
 *  @param b Board object to print
 */
void DraughtsCLInterface::printBoard(const Board &b) const{
  printf("\n\n"); //make sure we have a little clear space
  for(int ii=b.getSize()-1;ii>=0;ii--){ // prefer player 1 to have home at bottom of board
    std::cout << (ii+1) << (ii>=9 ? "    " : "     ");
    for(int jj=0;jj<b.getSize();jj++){
      Position p(jj,ii); //use outer loop as y
      printSquare( b.getSquare(p) , (ii%2==jj%2) );
      std::cout << "   ";
    }
    std::cout << "\n\n";
  }
  std::cout << "\n      ";
  for(int ii=0;ii<b.getSize();ii++){
    std::cout << "abcdefghijklmnopqrstuvwxyz"[ii] << "   ";
  }
  std::cout << "\n\n\n";
  return;
}

/**
 *  Prints move
 *
 *  Prints list of squares occupied by move\n
 *  Prints positions in intuitive format (eg. a3 b4 rather than (0,2) (1,3))\n
 *  Includes start, end, and any intermediate positions
 *
 *  @param p vector of Position objects, defining move
 */
void DraughtsCLInterface::printMove(const std::vector<Position> &p) const{
  for(std::vector<Position>::const_iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
    std::cout << (*p_iter).toString() << " ";
  }
  std::cout << "\n";
  return;
}

/**
 *  Prints multiple moves
 *
 *  calls printMove for each member of vector p
 *
 *  @param p vector of vectors defining moves
 */
void DraughtsCLInterface::printMoves(const std::vector<std::vector<Position> > &p) const{
  if(p.size()==0){
    std::cout << "No possible moves!\n";
    return;
  }
  for(int ii=0;ii<p.size();ii++){
    std::cout << "\nMove " << ii+1 << ": ";
    printMove(p.at(ii));
  }
  return;
}
/**
 *  Interprets move input
 *  
 *  Interprets sequence of Positions as move
 *
 *  @throw invalid_argument if unable to interpret as valid move
 *  @param s string detailing intended move
 *  @return std::vector<Position> giving each Position occupied in move, in order
 */
std::vector<Position> DraughtsCLInterface::interpretMove(const std::string &s) const{
  std::vector<Position> move;
  
  boost::tokenizer<> tok(s);

  for(boost::tokenizer<>::iterator t_iter=tok.begin(); t_iter!=tok.end(); t_iter++){
    try{
      std::string p(*t_iter);
      move.push_back(interpretSquare(p));
    }catch(std::exception &e){
      std::string errmsg("unable to interpret move:\n");
      errmsg+=e.what();
      throw std::invalid_argument( errmsg );
    }
  }
  if(move.size()<2){
    std::string errmsg("invalid move: must specify at least two squares. Input: \"");
    errmsg+=s;
    errmsg+="\"\nInterpretation: \"";
    for(int ii=0;ii<move.size();ii++){
      errmsg+=move.at(ii).toString();
    }
    errmsg+="\"\n";
    throw std::invalid_argument(errmsg);
  }
  return move;
}

/**
 *  Validates that a move is legal
 *
 *  Validates that an entered move is legal
 *
 *  @throw runtime_error if no legal moves available
 *  @param p move to be validated
 *  @param g Game in which move is to be played
 *  @return true if move is legal, false otherwise
 */
bool DraughtsCLInterface::validateMove(const std::vector<Position> &p, Game *g) const{
  std::vector<std::vector<Position> > legalmoves = g->getMovesForPlayer(g->getCurrentPlayer());
  if(legalmoves.size()==0){
    std::string errmsg("no legal moves!\n");
    throw std::runtime_error(errmsg);
  }
  for(int ii=0;ii<legalmoves.size();ii++){
    if(legalmoves.at(ii) == p) return true;
  }
  return false;
}

/**
 *  Get next move
 *
 *  Asks player for input, calls validateMove to ensure move is legal\n
 *  Keeps asking until valid move given
 *
 *  @param g Game for which move is being requested
 *  @return valid move
 */
std::vector<Position> DraughtsCLInterface::getMove(Game *g) const{
  std::string input;
  std::vector<Position> move;
  bool valid=false;
  do{
    try{
      std::cout << "Input move for player " << (g->getCurrentPlayer()==1 ? "1" : "2" ) << "\n:> ";
      std::getline(std::cin,input);
      boost::algorithm::to_lower(input);
      if(!strncmp(input.c_str(),"help",4)){
        CLInterface::showMenuHelp();
        std::cout << "\n";
        continue;
      }
      move = interpretMove(input);
      if( !(valid = validateMove(move,g) ) ){
        std::cout << "Not a valid move!\n";
      }
    }
    catch(std::exception &e){
      std::cout << e.what();
    }
  }while(!valid);
  return move;
}
