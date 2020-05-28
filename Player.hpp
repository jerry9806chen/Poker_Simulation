//
//  Player.hpp
//  assignment2
//
//  Created by rick gessner on 10/6/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "Card.hpp"
#include "Hand.hpp"

//STUDENT: This class is abstract, and serves as an interface

namespace ECE17 {

  class IPlayer {
  public:

    virtual std::string& getName()=0; //get the player name
    virtual void         willStartRound(size_t aRoundNum)=0; //tell player that round N is starting...`
    virtual bool         isFolding(const ECE17::Hand &aHand, size_t aPlayerCount, float aBalance)=0;
    virtual float        placeBet(const ECE17::Hand &aHand, size_t aPlayerCount, float aMaxBet)=0;
    virtual size_t       discardUnwanted(ECE17::Hand &aHand)=0;
  };

}

#endif /* Player_hpp */
