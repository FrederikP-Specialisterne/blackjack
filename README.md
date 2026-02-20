#Blondie's game of CLI-Blackjack
##A blackjack game that is played in the command line interface.
###

Blackjack is a casino cardgame where you play against an NPC dealer alongside other players.
Everyone starts with a hand of two cards.
Everyone then takes turns to draw additional cards to increase the value of their hands.
Players win by ending their turn with a greater value in their hand than the dealer.
But if the value of a hand exceeds 21, the hand automatically looses.

Blackjack is a very widespread game, and more comprehensive decriptions of the rules can easily be found by googling. 

##FEATURES
- Up to 4 players playing against the same dealer
- All players start out with $100 in their bank accounts to make bets from
- A realistic standard deck of 52 cards that gets reshuffled between each round
- The option to split a hand, double down on a hand, or to surrender a hand when applicable

##HOW TO PLAY
The game bids prompts you to enter the number of players, an integer between 1 and 4.
Each player is then in turn prompted to enter a bet, a positive integer.
Everyone is then dealt two cards at random.
Each player is in turn told their cards and the first of the dealer's two cards.
The player is then prompted to enter which move they want to make
- "Hit"		to draw another card
- "Stand"	to draw no more cards
- "Split"	if the two cards are of the same type, an additional bet can be placed to split the hand into two hands which play out simultaneously.
- "Double down"	place an additional bet at the start to draw one and only one additional card
- "Surrender"	forfeit your hand and recover half of the bet placed

Once all players have entered their actions the round resolves, i.e, hands get compared and bets get paid out.
Then the game starts over again, and players now place bets from their updated bank accounts.

##NOT YET IMPLEMENTED
- Insurance bets
- The split move should be selected in the same way that other moves are selected, instead of a (y/n) prompt 
- ...

##KNOWN ISSUES
- The input validation may read multiple entries from the I/O stream despite 'enter' only being pressed once
- Currently no logic for handling when players go bankrupt
- Currently no logic for handling bank balance overflows
- Currently no linespacing between players or rounds, which makes it difficult to keep track of where in the game you are

Written in C++
Compiled with g++ in WSL
Containerized with Docker
