//
// Created by Vikash Gilja on 5/5/20.
//

#include "Card.hpp"

namespace ECE17 {

    // Destructor
    Card::~Card() {
      if (debug_print) {
        std::cout << "DESTROYED: " << CardString() << std::endl;
      }
    }

    std::string Card::GetFaceName() {
      switch (face) {
        case (ACE):
          return "A";
        case (JACK):
          return "J";
        case (QUEEN):
          return "Q";
        case (KING):
          return "K";
        case (INVALID_FACE):
          return "I";
        default:
          // enums are integers and we made sure the values for card 2-10 were correct
          // when we setup the FaceEnum type!
          return std::to_string(face);
      }
    }

    std::string Card::GetSuitName() {
      switch (suit) {
        case (SPADE):
          return "Spade";
        case (DIAMOND):
          return "Diamond";
        case (HEART):
          return "Heart";
        case (CLUB):
          return "Club";
        case (INVALID_SUIT):
          return "INVALID";
      }
    }

    std::string Card::GetSuitSymbol() {
      switch (suit) {
        case (SPADE):
          return SPADE_SYMBOL;
        case (DIAMOND):
          return DIAMOND_SYMBOL;
        case (HEART):
          return HEART_SYMBOL;
        case (CLUB):
          return CLUB_SYMBOL;
        case (INVALID_SUIT):
          return "I";
      }
    }

    Card::Card(const Card &aCard) {
      // This shallow copy is the default behavior, even if we didn't specify this
      // copy constructor. Note that the copy constructor can access private members
      // of another object!
      face = aCard.face;
      suit = aCard.suit;
      debug_print = aCard.debug_print;

      if (debug_print) {
        std::cout << "COPY CONSTRUCTION: " << CardString() << std::endl;
      }
    }

    Card &Card::operator=(const Card &aCard) {
      // This shallow copy is the default behavior, even if we didn't specify this
      // copy constructor. Note that the copy constructor can access private members
      // of another object!

      // We explicitly handle the case of self-assignment, we don't need to do anything
      // Note, we didn't need to handle this case, as the shallow copy would have done the "right"
      // thing for this class. We show this case here to point out this condition!
      if (&aCard == this) {
        if (debug_print) {
          std::cout << "SELF-ASSIGNMENT: " << CardString() << std::endl;
        }
        return *this;
      }

      // Shallow copy!
      face = aCard.face;
      suit = aCard.suit;
      debug_print = aCard.debug_print;

      if (debug_print) {
        std::cout << "ASSIGNED: " << CardString() << std::endl;
      }

      return *this;
    }

    std::string Card::CardString() {
      return GetSuitSymbol() + GetFaceName();
    }

    std::string Card::PrettyCardString() {
      // The C++ has a constructor that will repeat a character for us
      // use this feature to create the right padding for the lines with the face
      // to handle the special case of the 10 card
      // The cards are 7 characters wide (the definition PRETTY_CARD_WIDTH) and 2 characters
      // are used for the left and right vertical lines. We substract off the length of the face Name
      std::string face_padding(PRETTY_CARD_WIDTH - GetFaceName().length() - 2, ' ');

      // Build the pretty print line by line
      std::string pretty_card = std::string(TOP_LINE) + "\n";
      pretty_card += VERTICAL_BAR + GetFaceName() + face_padding + VERTICAL_BAR + "\n";
      pretty_card += std::string(VERTICAL_BAR) + "  " + GetSuitSymbol() + "  " + VERTICAL_BAR + "\n";
      pretty_card += std::string(VERTICAL_BAR) + face_padding + GetFaceName() + VERTICAL_BAR + "\n";
      pretty_card += std::string(BOTTOM_LINE) + "\n";

      return pretty_card;

    }

    // Checks for matching card, note that we can access the "private" members of another instance
    // this is because they are of the same class!
    bool Card::DoesCardMatch(const Card &a_card) {
      return a_card.face == face && a_card.suit == suit;
    }

    bool Card::IsCardEqual(const Card &a_card) {
      return a_card.face == face;
    }

    bool Card::IsCardGreaterThan(const Card &a_card) {
      if (face > a_card.face) return true;
      return false;
    }

}