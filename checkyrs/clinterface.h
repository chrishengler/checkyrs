//
//  clinterface.h
//  checkyrs
//
//  Created by Chris Hengler on 07/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef __checkyrs__clinterface__
#define __checkyrs__clinterface__

#include <stdio.h>
#include "game.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

class CLInterface {

public:
  void printBoard();
  
  void printMoves();
  
};
#endif /* defined(__checkyrs__clinterface__) */
