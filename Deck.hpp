//
// Created by Vikash Gilja on 5/5/20.
//

#ifndef Deck_hpp
#define Deck_hpp

#include "CardPile.hpp"
#include <algorithm>
#include <list>

namespace ECE17 {

    class Deck : public CardPile {
    public:
        Deck();

        void Shuffle();

    };

}
#endif //Deck_hpp
