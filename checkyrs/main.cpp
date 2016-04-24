//
//  main.cpp
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <iostream>
#include "draughtsrunner.h"

int main(int argc, const char * argv[]) {

  std::cout << "\n\n\n\n C H E C K Y R S \n\n\n\n";
  
  CLInterface cli;
  bool play=true;
  if(cli.yn("Display help? (y/n)")){
    cli.showMenuHelp();
  }
  while(play){
    try{
      DraughtsRunner runner;
      runner.initialise();
      while(runner.continueGame()){      };
      play = cli.yn("\n\nnew game? (y/n)");
    }
    catch(std::exception &e){
      std::cout << "unexpected exception in main: " << e.what();
      return -1;
    }
  }
  
  return 0;
}
