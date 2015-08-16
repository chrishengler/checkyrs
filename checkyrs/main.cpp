//
//  main.cpp
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <iostream>
#include "gamerunner.h"

int main(int argc, const char * argv[]) {

  std::cout << "\n\n\n\n C H E C K Y R S \n\n\n";
  
  CLInterface cli;
  bool play=true;
  while(play){
    try{
      Gamerunner gr;
      gr.initialise();
      while(gr.continueGame()){};
      play = cli.yn("\n\nnew game? (y/n)\n");
    }
    catch(std::exception &e){
      std::cout << "unexpected exception in main: " << e.what();
      return -1;
    }
  }
  
  return 0;
}
