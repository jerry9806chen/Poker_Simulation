//
// Created by Vikash Gilja on 5/5/20.
//

#ifndef Card_hpp
#define Card_hpp

#define SPADE_SYMBOL	"S"
#define CLUB_SYMBOL    "C"
#define HEART_SYMBOL   "H"
#define DIAMOND_SYMBOL "D"

#define TOP_LINE  "┌─────┐"
#define BOTTOM_LINE "└─────┘"
#define VERTICAL_BAR "|"
#define PRETTY_CARD_WIDTH 7

#include <iostream>
#include <string>

namespace ECE17 {

// We define enumerated types for the face and suit
// Note, we've set the ACE=1 so that the numeric values of the face match typical
// card game values
    enum FaceEnum {
        ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, INVALID_FACE
    };
    enum SuitEnum {
        SPADE = 0, DIAMOND, HEART, CLUB, INVALID_SUIT
    };

    class Card {
    public:
        // Default Constructor
        // Since we are setting default values, this method is the default constructor,
        // even though it can take parameters (Here we show that you can define methods in the header file)
        // Note: that we use the keyword "explicit." This tells the compiler to NOT use type conversion
        // "implicitly" when calling this function -- it means that the function can only be called with
        // parameters matching the types specified in the function declaration.
        explicit Card(FaceEnum face = ACE, SuitEnum suit = SPADE, bool debug_print = false) :
                face(face), suit(suit), debug_print(debug_print) {
          if (debug_print) {
            std::cout << "DEFAULT CONSTRUCTION: " << CardString() << std::endl;
          }
        };

        /* Note: you'll find commented out versions of a much simpler Default Constructor below
        Card() {
            face = ACE;
            suit = SPADE;
            debug_print = false;
         };
         Card() : face(ACE), suit(SPADE), debug_print(false) {};
         These two versions of constructors are equivalent to each other, but are not as powerful as the version uncommented
         above. The version above acts as a default constructor as well as a constructor that takes 1, 2, or 3 parameters.
         */

        // Copy Constructor
        // We didn't need to specify this, since the compiler would have created this
        // constructor for us with a shallow copy of the members. We make the definition
        // here as an example for you to draw upon
        Card(const Card &aCard);

        // Copy Constructor
        // We didn't need to specify this, since the compiler would have created this
        // constructor for us with a shallow copy of the members. We make the definition
        // here as an example for you to draw upon
        Card &operator=(const Card &aCard);

        // Destructor
        // We didn't need to specify this, since there isn't anything special for the
        // destructor to do (e.g. we didn't allocate dynamic memory that needs to be
        // released). Again we provide it here as an example for you
        ~Card();

        // These helper methods can be called to enable/disable the debug_print
        // Note: this feature should not be enabled in your final version of the code
        void EnableDebugPrint() { debug_print = true; }

        void DisableDebugPrint() { debug_print = false; }

        // Returns Face and Suit Names
        // Face names are designed for printing (so are abbreviated)
        // Suit names are complete
        std::string GetFaceName();

        std::string GetSuitName();

        // Returns unicode Suit symbol for printing
        std::string GetSuitSymbol();

        // Returns Face and Suit enum values
        FaceEnum GetFace() { return face; }

        SuitEnum GetSuit() { return suit; }

        // The CardString() method is nice for abbreviated printing
        // PrettyCardString() isn't need for this assignment, included here to call back to assignment 0!
        std::string CardString();

        std::string PrettyCardString();

        // Checks if the card matched a card provided as a parameter
        bool DoesCardMatch(const Card &a_card);

        bool IsCardEqual(const Card &a_card);

        bool IsCardGreaterThan(const Card &a_card);

        bool operator==(const Card &a_card) { return IsCardEqual(a_card); };

        bool operator>(const Card &a_card) { return IsCardGreaterThan(a_card); };

        bool operator>=(const Card &a_card) { return IsCardGreaterThan(a_card) || IsCardEqual(a_card); };

        bool operator<(const Card &a_card) { return !(IsCardGreaterThan(a_card) || IsCardEqual(a_card)); };

        bool operator<=(const Card &a_card) { return !IsCardGreaterThan(a_card); };

        bool operator!=(const Card &a_card) { return !IsCardEqual(a_card); };

    private:
        FaceEnum face;
        SuitEnum suit;
        bool debug_print;
    };
}
#endif //Card_hpp
