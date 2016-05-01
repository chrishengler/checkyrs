//
//  clinterface.cpp
//  checkyrs
//
//  Created by Chris Hengler on 01/05/16.
//  Copyright (c) 2016 chrysics. All rights reserved.
//

#include <iostream>
#include "boost/algorithm/string.hpp"
#include "clinterface.h"

/**
 *  Get yes/no from player
 *
 *  Asks player a yes/no question until receiving a valid answer\n
 *
 *  @param s Question for player
 *  @return true if yes, false if no
 */
bool CLInterface::yn(const std::string &s) {
  std::cout << s << "\n> ";
  bool valid = false;
  bool retval = false;
  try{
    while(!valid){
      std::string input;
      std::cin >> input;
      std::cin.ignore();
      boost::algorithm::to_lower(input);
      if(!strncmp(input.c_str(),"help",4)){
        showMenuHelp();
        std::cout << s << "\n> ";
        continue;
      }
      if(!strncmp(input.c_str(),"y",1)){
        return true;
      }
      else if(!strncmp(input.c_str(),"n",1)){
        valid = true;
      }
      if(!valid) std::cout << "invalid response, provide \"y\" or \"n\"\n> ";
    }
  }
  catch(std::exception &e){
    std::cout << "unexpected exception while trying to get answer to y/n question: " << e.what() << "\n";
  }
  return retval;
}


/**
 *  Show help for menus
 */
void CLInterface::showMenuHelp() {
  std::cout << "\n\nHelp for Checkyrs menu screens:\n\n";
  std::cout << "Follow the instructions on screen to set up a game.\n";
  std::cout << "Type 'y' or 'n' for yes/no questions, and input numbers with the number keys.\n\n";
  std::cout << "Type 'help' at any input prompt to show this message again\n\n\n";
}

/**
 *  No game help available in abstract class - we're not a game yet!
 */
void CLInterface::showGameHelp() {
  std::cout << "Can't show game help yet, I don't know what type of game I am!\nWhen a game is started, a description of how to play will be offered here.";
  return;
}

/**
 *  Interpret string as board position
 *
 *  Take user input (e.g. a3) and return corresponding Position
 *
 *  @throw runtime_error if string is too short
 *  @throw invalid_argument if interpreted Position is invalid
 *  @param s user input to be interpreted as Position
 *  @return Position corresponding to user input
 */
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
    if(std::tolower(s.at(0)) == "abcdefghijklmnopqrstuvwxyz"[ii]){
      x = ii;
      break;
    }
  }
  if(x==-1){
    std::string err("could not understand \"");
    err+=s.at(0);
    err+="\" as column\n";
    throw std::invalid_argument(err);
  }
  s.erase(0,1);
  try{
    y = (std::stoi(s) - 1);
    if(y<0) throw std::invalid_argument(std::string("invalid position: negative rank"));
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

