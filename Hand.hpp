//
//  Hand.hpp
//  assignment2
//
//  Created by rick gessner on 10/6/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef Hand_hpp
#define Hand_hpp

#include <vector>
#include "Card.hpp"
#include "CardPile.hpp"

namespace ECE17 {

  //you don't have to use these, but they might come in handy...
  enum class HandTypes {
    unknown=0, high_card=1, one_pair=2, two_pair=3, three_kind=4, straight=5,
    flush=6, full_house=7, four_kind=8, straight_flush=9, royal_flush=10
  };

  using CardList = std::vector<Card>;

  //student -- declare the Hand class here...
  class Hand : public CardPile{
  public:
    
    Hand(const CardList &aCards); //ctor from a vector of cards...


    bool      empty(); // drop all your cards (i.e. empty your hand)
    bool      discard(const Card &aCard);  // remove a specific card from your hand, true if successful
    HandTypes determineRank(); //analyze the cards in the hand, and determine what type of logical hand it is (pair, full-house, etc)

    size_t    count(); // returns the number of cards in the hand
    
  };

}

#endif /* Hand_hpp */
