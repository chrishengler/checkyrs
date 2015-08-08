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
  REQUIRE(str == "Hello b2");
}

TEST_CASE("Check equality of Positions"){
  Position p1(1,1);
  Position p2(2,2);
  Position p3(1,1);
  REQUIRE_FALSE(p1==p2);
  REQUIRE(p1==p3);
  REQUIRE_FALSE(p1!=p3);
  REQUIRE(p1!=p2);
}