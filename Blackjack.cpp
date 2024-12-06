#include <iostream>
#include <vector>
#include <ctime>
#include <limits>
#include <random>

namespace CardGame {
    constexpr int ACE_VALUE = 11;
    constexpr int FACE_CARD_VALUE = 10;
    constexpr int DEALER_THRESHOLD = 17;
    constexpr int CARD_MIN_VALUE = 2;
    constexpr int CARD_MAX_VALUE = 14;
}

void hit(std::vector<int> &cards);
void startGame(std::vector<int> &pCards, std::vector<int> &cCards, bool &blackjack);
void showNumbers(const std::vector<int> &pCards, const std::vector<int> &cCards, bool stand);
char playerMove(std::vector<int> &pCards, std::vector<int> &cCards);
void computerMove(std::vector<int> &cards);
bool checkWinner(int &pTotal, int &cTotal, bool stand);
int getTotal(const std::vector<int> &cards);
bool isBusted(int total);
bool isBlackjack(int total);
void softAce(std::vector<int> &cards);
int adjustCardValue(int card);
bool isAce(int card);

int main()
{

    std::vector<int> pCards;
    std::vector<int> cCards;

    bool running = true;
    char pMove;
    char playAgain;
    int pTotal = 0;
    int cTotal = 0;
    bool stand = false;
    bool blackjack = false;


    startGame(pCards, cCards, blackjack);
    showNumbers(pCards, cCards, stand);

    while (running)
    {
        if (!blackjack){
            if (!stand)
            {
                pMove = playerMove(pCards, cCards);
                if (pMove == 'Q')
                {
                    running = false;
                    break;
                }
                else if (pMove == 'H')
                {
                    hit(pCards);
                }
                else if (pMove == 'S')
                {
                    stand = true;
                    computerMove(cCards);
                }
            }

        pTotal = getTotal(pCards);
        cTotal = getTotal(cCards);

        showNumbers(pCards, cCards, stand);
    }

        if (checkWinner(pTotal, cTotal, stand) || blackjack == true)
        {
            std::cout << "Thanks For Playing!\n";

            std::cout << "Would You Like To Play Again? (Y/N): ";
            std::cin >> playAgain;
            playAgain = toupper(playAgain);

            while (playAgain != 'Y' && playAgain != 'N')
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid Input. Please Enter Y (Yes) or N (No): ";
                std::cin >> playAgain;
                playAgain = toupper(playAgain);
            }

            if (playAgain == 'N')
            {
                running = false;
            }
            else if (playAgain == 'Y')
            {
                pCards.clear();
                cCards.clear();
                stand = false;
                blackjack = false;
                startGame(pCards, cCards, running);
                showNumbers(pCards, cCards, stand);
            }
        }
    }


    return 0;
}

void hit(std::vector<int> &cards)
{

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(CardGame::CARD_MIN_VALUE, CardGame::CARD_MAX_VALUE);


    int card = adjustCardValue(distrib(gen));

    cards.push_back(card);
    softAce(cards);
}

void startGame(std::vector<int> &pCards, std::vector<int> &cCards, bool &blackjack)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(CardGame::CARD_MIN_VALUE, CardGame::CARD_MAX_VALUE);
    static std::uniform_int_distribution<> aceDistrib(CardGame::CARD_MIN_VALUE, 10);

    int playerFirstCard = adjustCardValue(distrib(gen));
    int playerSecondCard = adjustCardValue(distrib(gen));
    int computerFirstCard = adjustCardValue(distrib(gen));
    int computerSecondCard = adjustCardValue(distrib(gen));

    if (playerFirstCard == CardGame::ACE_VALUE && playerSecondCard == CardGame::ACE_VALUE) {  
        playerSecondCard = aceDistrib(gen);
    }  

    if (computerFirstCard == CardGame::ACE_VALUE && computerSecondCard == CardGame::ACE_VALUE) {  
        computerSecondCard = aceDistrib(gen);  
    }

    pCards.push_back(playerFirstCard);
    pCards.push_back(playerSecondCard);
    cCards.push_back(computerFirstCard);
    cCards.push_back(computerSecondCard);

    if (isBlackjack(getTotal(pCards)))
    {
        if (isBlackjack(getTotal(cCards)))
        {
            std::cout << "It's The Ultimate Tie!\n";
            blackjack = true;
        }
        else
        {
            std::cout << "You Got A Blackjack!\n";
            blackjack = true;
        }
    }

}

void showNumbers(const std::vector<int> &pCards, const std::vector<int> &cCards, bool stand)
{
    int total = 0;

    std::cout << "Player's Cards: ";
    for (int card : pCards)
    {
        if (isAce(card)){
            std::cout << 'A' << ' ';
        }
        else{
            std::cout << card << ' ';
        }
        total += card;
    }

    std::cout << "\nPlayer's Total: " << total << '\n';

    std::cout << "\n******************************************\n\n";

    if (stand)
    {
        int ctotal = 0;

        std::cout << "Dealer's Cards: ";
        for (int card : cCards)
        {
            if (isAce(card)){
                std::cout << 'A' << ' ';
            }
            else{
                std::cout << card << ' ';
            }
            ctotal += card;
        }
        std::cout << "\nDealer's Total: " << ctotal << '\n';
        std::cout << "******************************************\n";
    }
    else
    {
        if (isAce(cCards[0])){
            std::cout << "Dealer's Cards: A X\n";
        }
        else{
            std::cout << "Dealer's Cards: " << cCards[0] << " X\n";
        }
    }
}


char playerMove(std::vector<int> &pCards, std::vector<int> &cCards)
{
    char move;

    while (true)
    {
        std::cout << "Enter H (Hit), S (Stand), Q (Quit): ";
        std::cin >> move;

        if (std::cin.fail() || std::cin.peek() != '\n')
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter H, S, or Q.\n";
            continue;
        }

        move = toupper(move);

        if (move == 'H' || move == 'S' || move == 'Q')
        {
            return move;
        }
    }
}

void computerMove(std::vector<int> &cards)
{
    while (getTotal(cards) < CardGame::DEALER_THRESHOLD)
    {
        hit(cards);
    }
}

bool checkWinner(int &pTotal, int &cTotal, bool stand)
{
    if (isBlackjack(pTotal))
    {
        std::cout << "You Won!\n";
        return true;
    }
    else if (isBusted(pTotal))
    {
        std::cout << "You Busted!\n";
        return true;
    }

    if (isBlackjack(cTotal))
    {
        std::cout << "You Lost!\n";
        return true;
    }
    else if (isBusted(cTotal))
    {
        std::cout << "You Won!\n";
        return true;
    }

    if (stand)
    {
        if (pTotal == cTotal)
        {
            std::cout << "It's A Tie!\n";
            return true;
        }
        else if (pTotal > cTotal)
        {
            std::cout << "You Won!\n";
            return true;
        }
        else
        {
            std::cout << "You Lost!\n";
            return true;
        }
    }

    return false;
}

int getTotal(const std::vector<int> &cards)
{
    int total = 0;
    for (int card : cards)
    {
        total += card;
    }
    return total;
}

bool isBusted(int total)
{
    return total > 21;
}

bool isBlackjack(int total)
{
    return total == 21;
}

void softAce(std::vector<int> &cards)
{
    int total = getTotal(cards);

    for (int i = 0; i < cards.size(); i++) {
        if (cards[i] == CardGame::ACE_VALUE && isBusted(total)) {
            cards[i] = 1;
            total = getTotal(cards);
        }
    }
}



int adjustCardValue(int card)
{
    return (card > CardGame::ACE_VALUE) ? CardGame::FACE_CARD_VALUE : card;
}

bool isAce(int card)
{
    return card == CardGame::ACE_VALUE || card == 1;
}
