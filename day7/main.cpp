#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <fstream>
#include <sstream>
#include <tuple>
#include <unordered_map>

#include <vector>
#include <ranges>
#include <cstddef>



class pokerCard
{
private:
    

public:
    int val;

    pokerCard(char c)
    {
        if (c == 'A')
            val = 14;
        else if (c == 'K')
            val = 13;
        else if (c == 'Q')
            val = 12;
        else if (c == 'J')
            val = 0;
        else if (c == 'T')
            val = 10;
        else
            val = c - '0';
    }

    bool operator==(const pokerCard& other) const {
        return val == other.val;
    }

    bool operator<(const pokerCard& other) const {
        return val < other.val;
    }

};

class pokerHand
{
private:
    enum typeOfPokerHand {
        HIGHCARD = 0,
        ONEPAIR = 1,
        TWOPAIR = 2,
        THREEKIND = 3,
        FULLHOUSE = 4,
        FOURKIND = 5,
        FIVEKIND = 6
    };

    std::vector<pokerCard> cardsInHand {};
    long bet;
    typeOfPokerHand typeOfHand;

    std::vector<int> possibleReplacments{2,3,4,5,6,7,8,9,10,12,13,14 };


    typeOfPokerHand classifyHandPart1(std::vector<pokerCard>cards) //Part 1
    {
        return classifyHandInner(cards);
    }

    typeOfPokerHand classifyHand(std::vector<pokerCard>cards) //Part 2
    {
        std::vector< typeOfPokerHand> hands;
        for (auto rr : possibleReplacments)
        {
            auto tmpCards = cards;
            for (auto &cc : tmpCards)
            {
                if (cc.val == 0)
                    cc.val = rr;
            }
            hands.push_back(classifyHandInner(tmpCards));
        }
        
        std::sort(hands.begin(), hands.end());
        return hands.back();
    }

    typeOfPokerHand classifyHandInner(std::vector<pokerCard>cards)
    {
        std::sort(cards.begin(), cards.end());
        if (cards[0] == cards[4])
            return FIVEKIND;
        else if (cards[0] == cards[3] or cards[1] == cards[4])
            return FOURKIND;
        else if (cards[0] == cards[2])
        {
            auto tmp = (cards[3] == cards[4]) ? FULLHOUSE : THREEKIND;;
            return tmp;
        }
        else if (cards[1] == cards[3])
            return THREEKIND;
        else if (cards[2] == cards[4])
        {
            auto tmp = (cards[0] == cards[1]) ? FULLHOUSE : THREEKIND;
            return tmp;
        }
        else if (cards[0] == cards[1])
        {
            auto tmp = (cards[2] == cards[3] or cards[3] == cards[4]) ? TWOPAIR : ONEPAIR;
            return tmp;
        }
        else if (cards[1] == cards[2])
        {
            auto tmp = (cards[3] == cards[4]) ? TWOPAIR : ONEPAIR;
            return tmp;
        }
        else if (cards[2] == cards[3] or cards[3] == cards[4])
            return ONEPAIR;
        else
            return HIGHCARD;

    }


public:

    pokerHand(std::string line)
    {
        std::istringstream iss(line);
        std::string st1, st2;
        iss >> st1 >> st2;
        bet = std::stol(st2);
        std::transform(st1.begin(), st1.end(), std::back_inserter(cardsInHand), [](char c) { return pokerCard(c); });
        typeOfHand = classifyHand(cardsInHand);
        //std::reverse(cardsInHand.begin(), cardsInHand.end());
        
    }

    const typeOfPokerHand getTypeOfHand()
    {
        return typeOfHand;
    }
    const std::vector<pokerCard> getCardsInHand()
    {
        return cardsInHand;
    }
    const long getBet()
    {
        return bet;
    }

    bool operator<(pokerHand& rhs) const {

        auto RHSotherType = rhs.getTypeOfHand();
        if (typeOfHand == RHSotherType)
            return cardsInHand < rhs.getCardsInHand();
        else
            return typeOfHand < RHSotherType;

    }
};

std::vector<pokerHand> readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);
    std::vector<pokerHand> data;
	std::string line;

	while (std::getline(inputFile, line)) 
    {
        std::istringstream iss(line);

        data.push_back(pokerHand(line));
    }

	return data;
}


long part1computer(std::vector<pokerHand> hands)
{
    std::sort(hands.begin(), hands.end());
    long long cumsum = 0L;
    for (size_t i = 0; i < hands.size(); i++)
    {
        cumsum += (i + 1) * hands[i].getBet();
    }
    return cumsum;
}

void main()
{
    //auto ownHand = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day7ownTesting.txt");
    //auto ownPart1 = part1computer(ownHand);

    auto testHand = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day7test.txt");
    auto testPart1 = part1computer(testHand);
    auto realHand = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day7real.txt");
    auto realPart1 = part1computer(realHand);

    std::cout << "Part 1, test: " << testPart1 << " and real: " << realPart1 << std::endl; // 249944807 is too low
}
