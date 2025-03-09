#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

class Card {
public:
    enum Suit { HEARTS, DIAMONDS, CLUBS, SPADES };
    enum Rank { ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };

private:
    Suit suit;
    Rank rank;

public:
    Card(Suit suit, Rank rank) : suit(suit), rank(rank) {}

    int getValue() const {
        if (rank == ACE) return 11;
        if (rank == JACK || rank == QUEEN || rank == KING) return 10;
        return rank + 1;
    }

    std::string getRankStr() const {
        std::string rankNames[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
        return rankNames[rank];
    }

    std::string getSuitStr() const {
        std::string suitNames[] = { "Hearts", "Diamonds", "Clubs", "Spades" };
        return suitNames[suit];
    }

    std::string toString() const {
        return getRankStr() + " of " + getSuitStr();
    }
};

class Deck {
private:
    std::vector<Card> cards;

public:
    Deck() { Reset(); }

    void Shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cards.begin(), cards.end(), g);
    }

    Card DrawCard() {
        Card card = cards.back();
        cards.pop_back();
        return card;
    }

    void Reset() {
        cards.clear();
        for (int suit = 0; suit < 4; ++suit) {
            for (int rank = 0; rank < 13; ++rank) {
                cards.push_back(Card(static_cast<Card::Suit>(suit), static_cast<Card::Rank>(rank)));
            }
        }
    }
};

class Player {
protected:
    std::vector<Card> hand;
    int totalScore;
    std::string name;

public:
    Player(const std::string& playerName = "") : totalScore(0), name(playerName) {}

    void AddCard(const Card& card) {
        hand.push_back(card);
        totalScore += card.getValue();
    }

    int GetTotalScore() const {
        return totalScore;
    }

    void ClearHand() {
        hand.clear();
        totalScore = 0;
    }

    void DisplayHand(bool hideFirstCard = false) const {
        std::cout << "Hand: ";
        for (size_t i = 0; i < hand.size(); ++i) {
            if (i == 0 && hideFirstCard) {
                std::cout << "[Hidden] ";
            } else {
                std::cout << hand[i].toString() << " ";
            }
        }
        std::cout << "\n";
    }

    std::string GetName() const {
        return name;
    }
};

class Dealer : public Player {
public:
    void DisplayFirstCard() const {
        if (!hand.empty()) {
            std::cout << "Dealer's first card: " << hand[0].toString() << std::endl;
        }
    }
};

class Game {
private:
    Deck deck;
    Player player;
    Dealer dealer;

public:
    void Play() {
        std::string playerName;
        std::cout << "Enter your name: ";
        std::getline(std::cin, playerName);
        
        player = Player(playerName);
        deck.Shuffle();
        player.ClearHand();
        dealer.ClearHand();

        // Initial two cards for player and dealer
        player.AddCard(deck.DrawCard());
        dealer.AddCard(deck.DrawCard());
        player.AddCard(deck.DrawCard());
        dealer.AddCard(deck.DrawCard());

        // Display initial hands
        std::cout << "\n--- Blackjack Game ---\n";
        std::cout << player.GetName() << "'s Hand: ";
        player.DisplayHand();
        std::cout << "Dealer's Hand: ";
        dealer.DisplayHand(true);

        // Player's turn
        while (player.GetTotalScore() < 21) {
            char choice;
            std::cout << player.GetName() << ", do you want to hit or stand? (h/s): ";
            std::cin >> choice;

            if (choice == 'h') {
                player.AddCard(deck.DrawCard());
                std::cout << player.GetName() << "'s Hand: ";
                player.DisplayHand();
            } else if (choice == 's') {
                break;
            }
        }

        // If player busts
        if (player.GetTotalScore() > 21) {
            std::cout << player.GetName() << " busts! Dealer wins.\n";
            return;
        }

        // Dealer's turn
        std::cout << "Dealer reveals hidden card: ";
        dealer.DisplayHand(false);

        while (dealer.GetTotalScore() < 17) {
            dealer.AddCard(deck.DrawCard());
            std::cout << "Dealer hits.\n";
            dealer.DisplayHand(false);
        }

        // Determine winner
        DisplayResult();
    }

    void DisplayResult() const {
        std::cout << "\nFinal Scores:\n";
        std::cout << player.GetName() << "'s Total: " << player.GetTotalScore() << "\n";
        std::cout << "Dealer's Total: " << dealer.GetTotalScore() << "\n";

        if (player.GetTotalScore() > 21) {
            std::cout << player.GetName() << " busts! Dealer wins.\n";
        } else if (dealer.GetTotalScore() > 21) {
            std::cout << "Dealer busts! " << player.GetName() << " wins!\n";
        } else if (player.GetTotalScore() > dealer.GetTotalScore()) {
            std::cout << player.GetName() << " wins!\n";
        } else if (player.GetTotalScore() < dealer.GetTotalScore()) {
            std::cout << "Dealer wins.\n";
        } else {
            std::cout << "It's a tie!\n";
        }
    }
};

int main() {
    Game game;
    game.Play();
}
