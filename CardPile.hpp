//
// Created by Vikash Gilja on 5/5/20.
//

#ifndef SP20_MIDTERM_WARGAME_CARDPILE_HPP
#define SP20_MIDTERM_WARGAME_CARDPILE_HPP

#include "Card.hpp"
#include <list>

namespace ECE17 {

    class CardPile {
    public:
        bool isEmpty();

        void addCard(Card card);

        Card getCard();

        void printPile();

    protected:
        std::list<Card> cards;
    };

}
#endif //SP20_MIDTERM_WARGAME_CARDPILE_HPP
