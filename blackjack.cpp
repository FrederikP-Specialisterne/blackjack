#include <iostream> // For basic input/output
#include <cstdlib>  // For random numbers
#include <string>   // Because strings

#define BLACKJACK 22 // Greater than a 21
#define BUSTED -1
#define SURRENDER -2

struct card {
    int suit;
    /*
    0 = DIAMONDS, 
    1 = HEARTS,
    2 = SPADES
    3 = CLUBS
    */

    int val;
    /*
    00 = TWO
    01 = THREE
    ...
    08 = TEN,
    09 = JACK
    10 = QUEEN
    11 = KING
    12 = ACE
    */
};

struct card_deck {
    card cards[52];     // List of cards in the deck
    int top_card = 0;   // Tracks index of top card (i.e, how many cards are left in the deck)
};

struct hand {
    card aces[4];
    int n_aces = 0;
    card non_aces[7];
    int n_non_aces = 0;
};

void add_to_hand(hand &h, card &c) {
    if (c.val == 12) {
        h.aces[h.n_aces] = c;
        h.n_aces++;
    } else {
        h.non_aces[h.n_non_aces] = c;
        h.n_non_aces++;
    }
}

card draw_card(card_deck &deck) {
    /*
    I do not check whether there are any cards left to draw,
    because the deck will is reset between games, so it will never run out.
    Might be a relevant check to implement in the future
    */
    card drawn_card = deck.cards[deck.top_card];
    deck.top_card++;
    return drawn_card;

}

struct player {
    bool isDealer = false;  // Players are not the dealer by default
    int id;
    int bank_account = 100;
    int hand_values[16];
    int base_bet;
    int bet_list[16];
    card starting_hand[2];
};

void player_init(player players[5]) {
    // Just sets the dealer and the player IDs
    players[0].isDealer = true;
    for (int i = 0; i < 5; i++) {
        players[i].id = i;
    }
}



void get_base_bet(player &p) {
    std::string raw_input;
    int bet;
    float float_check;

    std::cout << "Player "
              << p.id 
              << ": You have $" 
              << p.bank_account 
              << " to bet from."
              << std::endl;
    
    // Keep trying until success
    while (true) {
        std::cin >> raw_input;  // Read input as string, and then try to parse it as an int
        try {
            bet = std::stoi(raw_input);         // Cast string to integer
            float_check = std::stof(raw_input); // Cast same string to float (to check for decimal values)
            if (bet != float_check) {
                throw (float_check);
            }

        // Error handling    
        } catch (float) {
            std::cout << "That is not a whole number. Please try again" << std::endl;
            continue;
        } catch (std::invalid_argument) {
            std::cout << "That is not a number. Please try again" << std::endl;
            continue;
        }

        // Bet is valid. Update numbers
        if (0 < bet && bet <= p.bank_account) {
            p.base_bet = bet;
            return;


        // More error handling
        } else if (bet <= 0) {
            std::cout << "Bet must be nonnegative. Please try again" << std::endl;
        } else if (bet > p.bank_account) {
            std::cout << "Insufficient funds. Please try again" << std::endl;
        } else {
            std::cout << "Invalid input. Please try again" << std::endl;
        }
    }
}

void increase_bet(player &p, int hand_id) {
    p.bet_list[hand_id] += p.base_bet;
    p.bank_account      -= p.base_bet;
}

int get_card_value(card c, int score) {
    if (c.val < 9) {            // The numbered cards:
        return c.val + 2;       // Simply the value of the card

    } else if (c.val < 12) {    // Picture cards:
        return 10;              // Are worth 10
    
    } else {                   // Aces:
        if (score < 11) {      // 11 or 1, depending on the score
            return 11;
        } else {
            return 1;
        }
    }
}

int get_hand_value(hand h) {
    int hand_val = 0;
    for (int i = 0; i < h.n_non_aces; i++) {
        hand_val += get_card_value(h.non_aces[i], hand_val);
    }
    for (int i = 0; i < h.n_aces; i++) {
        hand_val += get_card_value(h.aces[i], hand_val);
    }
    return hand_val;
}

void print_card_suit(card c) {
    switch (c.suit) {
        case 0:
            std::cout << "diamonds";
            break;
        case 1:
            std::cout << "hearts";
            break;
        case 2:
            std::cout << "spades";
            break;
        case 3:
            std::cout << "clubs";
            break;
    }
}

void print_card_value(card c, bool capitalized) {
    if (c.val < 9) {
        std::cout << c.val + 2;
    } else {
        if (capitalized) {
            switch (c.val) {
                case 9:
                std::cout << "Jack";
                break;
                case 10:
                std::cout << "Queen";
                break;
                case 11:
                std::cout << "King";
                break;
                case 12:
                std::cout << "Ace";
                break;
            }
        } else {
            switch (c.val) {
                case 9:
                std::cout << "jack";
                break;
                case 10:
                std::cout << "queen";
                break;
                case 11:
                std::cout << "king";
                break;
                case 12:
                std::cout << "ace";
                break;
            }
        }
    }
}

void print_card(card c, bool capitalized) {
    print_card_value(c, capitalized);
    std::cout << " of ";
    print_card_suit(c);
}

void print_deck(card_deck deck){
    for (int i = 0; i < 52; i++) {
        print_card(deck.cards[i], true);
        std::cout << std::endl;
    }
}


void deck_init(card_deck &deck) {
    // Initializes the suits and values of the card deck

    for (int s=0; s < 4; s++) {         // Range over each card suit
        for (int v=0; v < 13; v++) {    // Range voer each card value
            // There are 13 cards in each suit, 13 x suit + value
            deck.cards[13 * s + v].suit = s;
            deck.cards[13 * s + v].val  = v;
        }
    }
}

void reset_scores(player players[5]) {
    for (int i = 0; i < 5; i++) {
        players[i].base_bet = 0;
        for (int j = 0; j < 16; j++) {
            players[i].hand_values[j] = 0;
            players[i].bet_list[j] = 0;
        }
    }
}

void deck_shuffle(card_deck &deck) {
    // Fisher-Yates shuffling algorithm
    for(int i = 51; i >= 0; i--) {  // Start from the end and move to the start
        int j = rand() % (i+1);     // A random integer between 0 and the first index
        // Swap them

        card temp = deck.cards[i];
        deck.cards[i] = deck.cards[j];
        deck.cards[j] = temp;
    }
    deck.top_card = 0;
}

int get_n_players() {
    char inp;
    std::cout << "Please enter the number of players (1-4) or enter \'q\' to quit." << std::endl;
    while (true) { // Keep trying until valid input is entered
        std::cin >> inp; // Read input from user
        switch (inp) {
            case '1':
                return 1;
            case '2':
                return 2;
            case '3':
                return 3;
            case '4':
                return 4;

            case 'q':
                return 0; // 0 quits the program
            default:
                std::cout << "Invalid input. Please try again." << std::endl;
                std::cout << "Enter a number (1-4) or enter \'q\' to quit." << std::endl;
        }
    }
}


int get_next_move(player &p, int cards_in_turn, int current_score, bool ace_split) {
    if (!p.isDealer) {
        bool is_move_legal[4] = {false, true, false, false};
        int num_legal_moves = 1;
        int moves_counted = 0;
        std::string move_names[4] = {"\"Stand\"",
                                     "\"Fold\"",
                                     "\"Double down\"",
                                     "\"Surrender\""};
        int num_moves = sizeof(move_names) / sizeof(move_names[0]);

        if (!ace_split) { // You cannot draw additional cards after an ace split
            is_move_legal[0] = true;
            num_legal_moves++;
        }
        if (cards_in_turn == 2) {
            if (p.bank_account > p.base_bet) { // You have to be able to afford doubling down
                is_move_legal[2] = true;
                num_legal_moves++;
            }
            is_move_legal[3] = true; // You can surrender a hand that has only two cards in it
            num_legal_moves++;
        }

        
        std::string legal_moves_string = "You can ";
        for (int i = 0; i < num_moves; i++) {
            if (moves_counted == num_legal_moves - 1) {
                legal_moves_string += "or ";
            }
            if (is_move_legal[i]) {
                legal_moves_string += move_names[i];
                moves_counted++;
                if (moves_counted < num_legal_moves) {
                    legal_moves_string += ", ";
                }
            }

        }

        std::cout << "You are currently at " << current_score << std::endl;
        std::cout << "What would you like to do?" << std::endl;

        std::string inp;
        bool error_given;
        while (true) {
            error_given = false;
            std::cout << legal_moves_string << std::endl;
            std::getline(std::cin >> std::ws, inp);
            
            // Make the strings lowercase for easier comparison
            std::string s1 = inp;
            for (int i = 0; i < s1.length(); i++) {
                s1[i] = tolower(s1[i]);
            }
            
            for (int move_id = 0; move_id < num_moves; move_id++) {
                std::string s2 = move_names[move_id];
                s2 = s2.substr(1,s2.length() - 2); // Removes the quotation marks
                for (int i = 0; i < s2.length(); i++) {
                    s2[i] = tolower(s2[i]);
                }
                if (s1 == s2) {
                    if (is_move_legal[move_id]) {
                        return move_id;
                    } else {
                        std::cout << "That move is not legal at the moment. Try again" << std::endl;
                        error_given = true;
                    }
                }
            }
            if (!error_given) {
                std::cout << "Invalid input. Try again" << std::endl;
                error_given = true;
            }
        }
    } else { // Logic for the dealer
        if (current_score < 17) {
            return 0; // Stand
        } else {
            return 1; // Fold
        }
    }
}

bool top_draw_blackjack(card c1, card c2) {
    // A blackjack requires an ace and a {10, jack, queen, king}
    if (c1.val == 12) {
        for (int i = 8; i < 12; i++) {
            if (c2.val == i) {
                return true;    // We have a blackjack!
            }
        }
    }

    if (c2.val == 12) {
        for (int i = 8; i < 12; i++) {
            if (c1.val == i) {
                return true;    // We have a blackjack!
            }
        }
    }

    return false;               // We have no blackjack...
}

bool yes_or_no() {
    char inp;
    while (true) { // Keep trying until valid input is entered
        std::cin >> inp; // Read input from user
        switch (inp) {
            case 'y':
                return true;
            case 'n':
                return false;
            default:
                std::cout << "Invalid input. Please try again." << std::endl;
                std::cout << "\'y\' for yes, \'n\' for no." << std::endl;
        }
    }
}

bool is_face_card(card c) {
    if (8 <= c.val && c.val <= 11) {
        return true;
    } else {
        return false;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////



/*
Handles the logic for a single round of blackjack (player or dealer)

Note how this is essentially just a wrapper for itself (it is overloaded right below)
This id done in order to handle split-rounds recursively
*/
void play_round(player &dealer, player &p, card_deck &deck, card card1, card card2, bool ace_split, int hand_id) {
    if (p.bet_list[hand_id] == 0) {
        increase_bet(p,hand_id);
    }
    int current_score = 0;

    // Print out what cards the player has
    if (!p.isDealer) {
        std::cout << "You got "
        << ((card1.val == 12) ? "an " : "a ");
        print_card(card1, false);
        std::cout << " and "
        << ((card2.val == 12) ? "an " : "a ");
        print_card(card2, false);
        std::cout << "."
        << std::endl;
    }
    
    // First check if there is blackjack, and proceed if so
    if (top_draw_blackjack(card1,card2)) {
        std::cout << "That's a blackjack! "
                  << ((p.isDealer) ? "The house wins." : "Lucky you.")
                  << std::endl;
        current_score = BLACKJACK;
        p.hand_values[hand_id] = current_score;
        return;
    } else {
        // We don't have blackjack. Proceed as normally
        // Print out what the dealer has
        std::cout << "The dealer has "
                  << ((dealer.starting_hand[0].val == 12) ? "an " : "a ");
                     print_card(dealer.starting_hand[0], false);
        std::cout << " face up."
                  << std::endl;

        if (!p.isDealer) {
            // If the first two cards are the same, splitting is an option (Provided there is enough bank balance)
            if (card1.val == card2.val || (is_face_card(card1) && is_face_card(card2))) {
                if (p.bank_account > p.base_bet) {

                    std::cout << "Would you like to split your two cards (y/n)"
                              << std::endl;
                    std::cout << "It will cost another bet of $"
                              << p.base_bet
                              << ". ";
                    std::cout << "You have $"
                              << p.bank_account
                              << " in your account."
                              << std::endl;

                    if (yes_or_no()) {
                        bool is_ace_split = false;
                        if (card1.val == 12) {
                            is_ace_split = true;
                        }
                        // BUG NOTICE!!
                        // This is not how I should be handling the split ID.
                        // I need a way of assigning each split a unique ID
                        card card3 = draw_card(deck);
                        play_round(dealer, p, deck, card1, card3, is_ace_split, hand_id);

                        card card4 = draw_card(deck);
                        play_round(dealer, p, deck, card2, card4, is_ace_split, hand_id + 1);

                        return;
                    }
                } else { // There is a split, but funds are insufficient
                    std::cout << "You do not have enough money to place another bet for a split." << std::endl;
                }
            }
        }

        
        // BUG NOTICE!!
        // This is not how I should be counting scores.
        // I need to compute the value of all cards simultaneously because of aces
        // Or rather, I could do it like this.. But then I needa way of ensuring that I always count all aces last
        hand round_hand; // The greatest number of cards one can have without busting is 11

        
        current_score += get_card_value(card1, current_score);
        current_score += get_card_value(card2, current_score);

        int cards_in_turn = 1; // To ensure doubling down and surrendering only is possible on 2 cards
        bool is_done = false;
        while (true) {
            if (current_score == 21) {
                
                std::cout << ((p.isDealer) ? "They" : "You")
                          << " hit 21. "
                          << ((p.isDealer) ? "Oof!" : "Nice!")
                          << std::endl;
                p.hand_values[hand_id] = current_score;
                is_done = true;
            } else if (current_score > 21) {
                std::cout << ((p.isDealer) ? "They" : "You")
                          << " have busted."
                          << std::endl;
                current_score = BUSTED;
                p.hand_values[hand_id] = current_score;             
                is_done = true;
            }
            if (is_done) {
                return;
            }
            cards_in_turn++;
            
            int next_move = get_next_move(p, cards_in_turn, current_score, ace_split);
            
            switch (next_move) {
                card new_card;
                case 0: // Stand
                    new_card = draw_card(deck);
                    current_score += get_card_value(new_card, current_score);
                    std::cout << ((p.isDealer) ? "They" : "You")
                              << " drew "
                              << ((new_card.val == 12) ? "an " : "a ");
                                 print_card(new_card,false);
                    std::cout << "."
                              << std::endl;
                    
                    break;

                case 1: // Fold
                    p.hand_values[hand_id] = current_score;
                    is_done = true;
                    break;

                case 2: // Double down
                    increase_bet(p,hand_id);

                    new_card = draw_card(deck);
                    current_score += get_card_value(new_card, current_score);
                    p.hand_values[hand_id] = current_score;
                    std::cout << "You drew "
                              << ((new_card.val == 12) ? "an " : "a ");
                                 print_card(new_card,false);
                    std::cout << "."
                              << std::endl;

                    is_done = true;
                    break;

                case 3: // Surrender
                    current_score = SURRENDER;
                    p.hand_values[hand_id] = current_score;
                    p.bank_account += p.bet_list[hand_id] / 2;

                    is_done = true;
                    break;
            }   
        }
    }       // At the end of the round, gotta add a check for whether a player still has any money left to play with
}
// Now we overload it
void play_round(player &dealer, player &p, card_deck &deck) {
    std::cout << ((p.isDealer) ? "Dealer\'s" : ("Player " + std::to_string(p.id) + "\'s")) << " turn." << std::endl;
    play_round(dealer, p, deck, p.starting_hand[0], p.starting_hand[1], false, 0);
}

void resolve_round(player players[5]) {
    for (int p_id = 4 ; p_id >= 0; p_id--) {
        std::cout << "Player " << p_id << ": ";
        for (int hand = 0; hand < 16; hand++) {
            std::cout << players[p_id].hand_values[hand] << ",\t";
        }
        std::cout << std::endl;
    }

    std::cout << "== Comparing hands== " << std::endl;
    if (players[0].hand_values[0] == BLACKJACK) {
        std::cout << "The dealer has a blackjack" << std::endl;
    }
    for (int p_id = 4 ; p_id > 0; p_id--) {
        for (int hand = 0; hand < 16; hand++) {
            if (players[p_id].hand_values[hand] != 0) {
                std::cout << "Player "
                          << p_id 
                          << ": You have a hand ";
                if (players[p_id].hand_values[hand] == BUSTED) {
                    std::cout << "that busted!" << std::endl;
                } else {
                    std::cout << "of " 
                              << players[p_id].hand_values[hand]
                              << std::endl;
                }
            }
        }
    }
}


int main() {

    // Game setup
    srand(time(0)); // Set the seed for the random number generator

    // Create the deck of cards
    card_deck deck;
    deck_init(deck);
    
    // Create list of game participants
    player players[5];
    player_init(players);

    std::cout << "Welcome to Blackjack!" << std::endl;


    while (true) { // Main loop
        
        // Get number of players or exit game
        int n_players = get_n_players();
        //int n_players = 1;
        if (n_players == 0) { // The player wants to quit the game
            break;
        }

        // Get the game state ready
        deck_shuffle(deck);
        reset_scores(players);
        
        // First, everyone makes their bets
        for (int turn=n_players; turn >= 1; turn--) {
            get_base_bet(players[turn]);
        }
        
        // Second, everyone gets dealt two cards
        for (int turn=n_players; turn >= 0; turn--) {
            players[turn].starting_hand[0] = draw_card(deck);
            players[turn].starting_hand[1] = draw_card(deck);
        }

        // Third everyone takes their turn. Dealer goes last
        for (int turn=n_players; turn >= 0; turn--) {
            play_round(players[0], players[turn], deck);
        }

        // Finally, showdown.
        resolve_round(players);


    }

    return 0; // end of program
}