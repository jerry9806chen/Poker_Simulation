//
// Created by Vikash Gilja on 5/5/20.
//

#include "Deck.hpp"

namespace ECE17 {

    Deck::Deck() {
      for (int suit = SuitEnum::SPADE; suit <= SuitEnum::CLUB; suit++)
        for (int face = FaceEnum::ACE; face <= FaceEnum::KING; face++)
          this->addCard(Card((FaceEnum) face, (SuitEnum) suit));
    }

    void Deck::Shuffle() {
      CardPile tempCards[2];
      srand(time(nullptr));
      for (int num_splits = 0; num_splits < 123; num_splits++) {
        while (!cards.empty()) tempCards[rand() % 2].addCard(this->getCard());
        while (!tempCards[0].isEmpty()) this->addCard(tempCards[0].getCard());
        while (!tempCards[1].isEmpty()) this->addCard(tempCards[1].getCard());
      }
    };

}