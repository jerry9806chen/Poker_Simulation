//
// Created by Vikash Gilja on 5/5/20.
//

#include "CardPile.hpp"

namespace ECE17 {

    bool CardPile::isEmpty() {
      return cards.empty();
    }

    void CardPile::addCard(Card card) {
      cards.push_front(card);
    }

    Card CardPile::getCard() {
      if (cards.empty()) return Card(FaceEnum::INVALID_FACE, SuitEnum::INVALID_SUIT);
      Card card = cards.back();
      cards.pop_back();
      return card;
    }

    void CardPile::printPile() {
      for (auto it = cards.begin(); it != cards.end(); it++) {
        std::cout << it->CardString() << " ";
      }
      std::cout << std::endl;
    }

}