//
//  boardtests.cpp
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include "catch.hpp"
#include "board.h"

TEST_CASE("Board can be created"){
  
  Board newboard;
  REQUIRE(&newboard!=0);
}