//
//  positiontests.cpp
//  checkyrs
//
//  Created by Chris Hengler on 18/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#include <stdio.h>
#include "catch.hpp"
#include "position.h"

TEST_CASE("Can append Position to string"){
  Position p(1,1);
  std::string str("Hello ");
  str+=p.toString();
  REQUIRE(str == "Hello 1,1");
}