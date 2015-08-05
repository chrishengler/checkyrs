//
//  ai.cpp
//  checkyrs
//
//  Created by Chris Hengler on 04/08/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include "ai.h"

double CheckyrsAI::eval(const Board &b){
  double value=0;
  
  for(int ii=0;ii<b.getSize();ii++){
    for(int jj=0;jj<b.getSize();jj++){
      Position p = (m_player==1 ? Position(ii,jj) : Position(7-ii,7-jj));
      if(b.SquareIsOccupied(p)){
        if(b.getPlayer(p)==m_player){
          value+=(b.SquareHasKing(p) ? jj*2*m_possession : jj*m_possession);
        }
        else{
          value-=(b.SquareHasKing(p) ? jj*2*m_aggression : jj*m_aggression);
        }
      }
    }
  }
  return value;
}