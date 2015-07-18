//
//  piece.h
//  checkyrs
//
//  Created by Chris Hengler on 17/07/2015.
//  Copyright (c) 2015 chrysics. All rights reserved.
//

#ifndef __checkyrs__piece__
#define __checkyrs__piece__

#include <stdio.h>
class Piece {
  int _player;
  bool _isKing;

public:
  Piece(){_player=1; _isKing=false;}
  Piece(int p, bool k=false){_player=p; _isKing=k;}
  
  bool isKing(){return _isKing;}
};
#endif /* defined(__checkyrs__piece__) */
