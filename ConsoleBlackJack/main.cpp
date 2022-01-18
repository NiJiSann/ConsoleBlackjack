#include <algorithm> // std::shuffle
#include <array>
#include <ctime>
#include <iostream>
#include <numeric> // std::reduce
#include <random>

enum PlayerState
{
    start, lose, win, stand
};
enum class CardSuit
{
    club, diamond, heart, spade,
    max_suits
};

enum class CardRank
{
    rank_2, rank_3, rank_4,
    rank_5, rank_6, rank_7,
    rank_8, rank_9, rank_10,
    rank_jack, rank_queen, rank_king, rank_ace,
    max_ranks
};
struct Card
{
    CardRank rank{};
    CardSuit suit{};
};
std::string printCard(const Card& card)
{
    std::string res{};
    switch (card.rank)
    {
    case CardRank::rank_2:      res+="2";   break;
    case CardRank::rank_3:      res+="3";   break;
    case CardRank::rank_4:      res+="4";   break;
    case CardRank::rank_5:      res+="5";   break;
    case CardRank::rank_6:      res+="6";   break;
    case CardRank::rank_7:      res+="7";   break;
    case CardRank::rank_8:      res+="8";   break;
    case CardRank::rank_9:      res+="9";   break;
    case CardRank::rank_10:     res+="T";   break;
    case CardRank::rank_jack:   res+="J";   break;
    case CardRank::rank_queen:  res+="Q";   break;
    case CardRank::rank_king:   res+="K";   break;
    case CardRank::rank_ace:    res+="A";   break;
    default:
            res+= "?";
        break;
    }

    switch (card.suit)
    {
    case CardSuit::club:        res+="C";   break;
    case CardSuit::diamond:     res+="D";   break;
    case CardSuit::heart:       res+="H";   break;
    case CardSuit::spade:       res+="S";   break;
    default:
            res+= "?";
        break;
    }
    return res;
}

std::array<Card, 312> createDeck()
{
    std::array<Card, 312> deck{};
    int deckIterator{0};
    for ( int _mul{0} ;_mul<6 ; _mul++) {
        for (int _suit{0}; _suit<4; _suit++) {
            for (int _rank{0}; _rank<13; _rank++ ) {
                deck[deckIterator].suit = static_cast<CardSuit>(_suit);
                deck[deckIterator].rank = static_cast<CardRank>(_rank);
                deckIterator++;
            }
        }
    }
    return deck;
}

void printDeck(std::array<Card, 312>& deck)
{
    for (auto& item : deck) {
        printCard(item);
        std::cout<<' ';
    }
    std::cout<<"\n";
}

void shuffleDeck(std::array<Card, 312>& deck)
{
    static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
    std::shuffle(deck.begin(), deck.end(), mt);
}


int getCardValue(Card card)
{
    switch (card.rank) {
        case CardRank::rank_jack:
        case CardRank::rank_queen:
        case CardRank::rank_king:
            return 10;
        case CardRank::rank_ace:
            return 11;
        default:
            return static_cast<int>(card.rank)+2;
    }
}


void getCards(std::array<Card, 312>& deck, std::string& playerHand, int& playerScore,PlayerState& playerState , bool isDealer)
{
    std::string dealerFirstCards{};
    static int dealerIterator{1};
    static int playerIterator{0};
    bool playerHasAce{};
    bool dealerHasAce{};
    int* iteratorPtr{nullptr};
    bool* acePtr{nullptr};
    
    if (isDealer){
        iteratorPtr = &dealerIterator;
        acePtr = &dealerHasAce;
        dealerFirstCards =printCard(deck[1]);
    }
    else{
        acePtr = &playerHasAce;
        iteratorPtr = &playerIterator;
    }

    
    if (playerState!= PlayerState::stand) {
        playerHand += printCard(deck[*iteratorPtr]);
        if (getCardValue(deck[*iteratorPtr])==11) {
            *acePtr = true;
        }
        playerScore+= getCardValue(deck[*iteratorPtr]);
        *iteratorPtr+=2;
        playerHand += " | ";
    }
    
    if (playerScore>21) {
        if (*acePtr==true)
            playerScore-=10;
        else
            playerState = PlayerState::lose;
    } else if (playerScore==21)
    {
        playerState = PlayerState::win;
    }
    
    
    if (isDealer){
        if (playerScore>=17 && playerScore<21) {
            playerState = PlayerState::stand;
        }
        std::cout << dealerFirstCards << "\t\tDealers hand." <<"\n";
        
    }
    else{
        std::cout << playerHand;
        std::cout<<" Current player score: "<< playerScore<<"\n";
       
    }
    
}

bool win_lose_check(PlayerState& currState, bool isDealer)
{

    std::string playerType{"Player"};
    bool endGame{false};
    if (isDealer) {
        playerType = "Dealer ";
    }
    
    switch (currState) {
        case PlayerState::win:
            std::cout<<playerType<<" Won the game!!!\n";
            endGame=true;
            break;
        case PlayerState::lose:
            std::cout<<playerType<<" Lose the game!!!\n";
            endGame=true;
            break;
    }
    return  endGame;
}

void getPlayerInput(PlayerState& player)
{
    static char playerinput{'h'};
    if (playerinput == 'h') {
        std::cout<< "Stand or Hit?(input 'h' - to hit, 's' - to stand): ";
        std::cin>>playerinput;
        if (playerinput=='s') {
            player = PlayerState::stand;
        }
    }
}

int main()
{
    
    PlayerState player{};
    PlayerState dealer{};
    
    int playerScore{0};
    int dealerScore{0};
    
    std::string playerCards{};
    std::string dealerFirstCards{};
    std::string dealerCards{};
 
    
    // creating deck of 312 cards.
    auto deck{createDeck()};
    shuffleDeck(deck);
    
    //giving first cards.
    getCards(deck, playerCards, playerScore, player, false);
    getCards(deck, dealerCards, dealerScore, dealer, true);
    while (true) {
        getCards(deck, playerCards, playerScore, player, false);
        getCards(deck, dealerCards, dealerScore, dealer, true);

        if (win_lose_check(player, false)||win_lose_check(dealer, true)) {
            std::cout<<"Dealer Cads "<<dealerCards;
            std::cout<<"\t Dealer Score: "<<dealerScore<<'\n';
            return 0;
        }
        if (player == PlayerState::stand && dealer == PlayerState::stand) {
            std::cout<<"Dealer Cads "<<dealerCards;
            std::cout<<"\t Dealer Score: "<<dealerScore<<'\n';
            if (playerScore>dealerScore)
                std::cout<<"Player won the game!!!\n";
            else if (playerScore==dealerScore)
                std::cout<<"Tie!!!\n";
            else
                std::cout<<"Dealer won the game!!!\n";
            return 0;
        }

        if (player != PlayerState::stand)
            getPlayerInput(player);
        
    }
  return 0;
}
