//
//  piece.cpp
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include "square.h"

/**
 *  Copy-constructor
 *
 *  @param s the Square to copy
 */
Square::Square(const Square &s){
  m_isKing = s.m_isKing;
  m_isOccupied = s.m_isOccupied;
  m_player = s.m_player;
}