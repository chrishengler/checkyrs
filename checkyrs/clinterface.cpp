//
//  output.cpp
//  checkyrs
//
//  Created by Chris Hengler on 07/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <iostream>
#include "clinterface.h"

#define KNRM   "\x1B[0m"
#define KRED   "\x1B[31m"
#define KGRN   "\x1B[32m"
#define KYEL   "\x1B[33m"
#define KBLU   "\x1B[34m"
#define KMAG   "\x1B[35m"
#define KCYN   "\x1B[36m"
#define KWHT   "\x1B[37m"
#define RESET  "\033[0m"


void CLInterface::printSquare(const Square &s, const bool bs) const{
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
    std::cout << col << "0" << RESET ;
    return;
  }
  std::cout << col << "o" << RESET;
  return;
}

void CLInterface::printBoard(const Board &b) const{
  printf("\n\n\n"); //make sure we have a little clear space
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
  std::cout << "\n";
  return;
}

void CLInterface::printMove(const std::vector<Position> &p) const{
  for(std::vector<Position>::const_iterator p_iter=p.begin();p_iter!=p.end();p_iter++){
    std::cout << (*p_iter).toString() << " ";
  }
  std::cout << "\n";
  return;
}

void CLInterface::printMoves(const std::vector<std::vector<Position> > &p) const{
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

Position CLInterface::interpretSquare(std::string &s) const{
  int x=-1;
  int y;
  if(s.size()==0){
    std::string err("trying to intepret string as square, but string is empty!");
    throw std::runtime_error(err);
  }
  else if(s.size()==1){
    std::string err("trying to interpret string as square, but string only has one character!");
    throw std::runtime_error(err);
  }
  for(int ii=0;ii<26;ii++){
    if(s.at(0) == "abcdefghijklmnopqrstuvwxyz"[ii]){
      x = ii;
      break;
    }
  }
  if(x==-1){
    std::string err("could not understand \"");
    err+=s.at(0);
    err+="\" as column\n";
    throw std::runtime_error(err);
  }
  s.erase(0,1);
  try{
    y = (std::stoi(s) - 1);
    if(y<0) throw std::runtime_error(std::string("invalid position: negative rank"));
  }
  catch(std::invalid_argument){
    std::string errmsg("could not understand \"");
    errmsg+=s;
    errmsg+="\" as row number\n";
    throw std::invalid_argument(errmsg);
  }
  catch(std::exception &e){
    throw e;
  }
  return Position(x,y);
}

std::vector<Position> CLInterface::interpretMove(const std::string &s) const{
  std::vector<Position> move;
  
  boost::tokenizer<> tok(s);

  for(boost::tokenizer<>::iterator t_iter=tok.begin(); t_iter!=tok.end(); t_iter++){
    try{
      std::string p(*t_iter);
      move.push_back(interpretSquare(p));
    }catch(std::exception &e){
      std::string errmsg("unable to interpret move:\n");
      errmsg+=e.what();
      throw std::runtime_error( errmsg );
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
    throw std::runtime_error(errmsg);
  }
  return move;
}

bool CLInterface::validateMove(const std::vector<Position> &p, const Game &g) const{
  std::vector<std::vector<Position> > legalmoves = g.getMovesForPlayer(g.getCurrentPlayer());
  if(legalmoves.size()==0){
    std::string errmsg("no legal moves!\n");
    throw std::runtime_error(errmsg);
  }
  for(int ii=0;ii<legalmoves.size();ii++){
    if(legalmoves.at(ii) == p) return true;
  }
  return false;
}

std::vector<Position> CLInterface::getMove(const Game &g) const{
  std::string input;
  std::vector<Position> move;
  bool valid=false;
  do{
    std::cout << "input move for player " << (g.getCurrentPlayer()==1 ? "1" : "2" ) << "\n";
    std::getline(std::cin,input);
    try{
      move = interpretMove(input);
      if( !(valid = validateMove(move,g) ) ){
        std::cout << "not a valid move\n";
      }
    }
    catch(std::exception &e){
      std::cout << e.what();
    }
  }while(!valid);
  return move;
}
