//
//  output.cpp
//  checkyrs
//
//  Created by Chris Hengler on 07/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <iostream>
#include "clinterface.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"


void CLInterface::printSquare(const Square &s, const bool bs) const{
  std::string col;
  if(s.isOccupied()){
    col = (s.getPlayer() == 1 ? KCYN : KRED );
  }
  else{
    col = (bs ? KBLU : KWHT );
  }
  if(!s.isOccupied()){
    printf( "%s" , (col+"."+RESET).c_str() );
    return;
  }
  if(s.isKing()){
    std::printf( "%s" , (col+"O"+RESET).c_str() );
    return;
  }
  printf( "%s" , (col+"o"+RESET).c_str() );
  return;
}

void CLInterface::printBoard(const Board &b) const{
  printf("\n\n\n"); //make sure we have a little clear space
  for(int ii=b.getSize()-1;ii>=0;ii--){ // prefer player 1 to have home at bottom of board
    printf( "%c  " , "abcdefghijklmnopqrstuvwxyz"[ii] );
    for(int jj=0;jj<b.getSize();jj++){
      Position p(jj,ii); //use outer loop as y
      printSquare( b.getSquare(p) , (ii%2==jj%2) );
      printf("   ");
    }
    printf("\n\n");
  }
  printf("");
  for(int ii=0;ii<b.getSize();ii++){
    printf( (ii>=9 ? "  %d" : "   %d") , ii+1 );
  }
  printf("\n");
  return;
}