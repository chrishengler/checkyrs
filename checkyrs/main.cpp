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
  // insert code here...
  Gamerunner gr(0);
  gr.initialise();
  while(gr.continueGame()){};

  return 0;
}
