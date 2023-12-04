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
#include <functional>


#include <locale>


#include <vector>
#include <ranges>
#include <cstddef>

std::vector<std::string>
splitStringIntoVector(const std::string inputString, const char delimiter = ';')
{
	// Create a stringstream from the input string
	std::istringstream iss(inputString);

	// Vector to store the split substrings
	std::vector<std::string> substrings;

	// Split the string using std::getline and semicolon as the delimiter
	std::string substring;
	while (std::getline(iss, substring, delimiter)) {
		substrings.push_back(substring);
	}
	return substrings;
}

std::string
removeFirstPartOfInput(const std::string str)
{
	std::string str2(": ");
	std::size_t found = str.find(str2);
	std::string substring = str.substr(found + 2);
	return substring;
}

class singleCard {
public:
	std::vector<std::string> ticketNumbers;
	std::vector<std::string> winningNumber;

	singleCard(const std::string input)
	{
		auto vectorOfInputs = splitStringIntoVector(input, '|');

		std::vector<std::string> list = splitStringIntoVector(vectorOfInputs[0], ' ');
		std::copy_if(list.begin(), list.end(), std::back_inserter(ticketNumbers), [](std::string s) {return true; });
		std::sort(ticketNumbers.begin(), ticketNumbers.end());
		
		list = splitStringIntoVector(vectorOfInputs[1], ' ');
		std::copy_if(list.begin(), list.end(), std::back_inserter(winningNumber), [](std::string s) {return s != ""; });
		std::sort(winningNumber.begin(), winningNumber.end());
	}

	long resultOfCard()
	{
		auto matchingNumbers = numberOfWinningNumbers();
		return matchingNumbers==0 ? 0 : std::pow(2, matchingNumbers - 1);
	}

	long numberOfWinningNumbers() { return findNumberOfMatchingNumbers().size(); }

private:
	std::vector<std::string>
	findNumberOfMatchingNumbers()
	{
		// Use set_intersection to find common elements
		std::vector<std::string> commonElements;
		std::set_intersection(ticketNumbers.begin(), ticketNumbers.end(),
			winningNumber.begin(), winningNumber.end(),
			std::back_inserter(commonElements));
		return commonElements;
	}
};

class DeckOfCards
{
	std::vector<singleCard> cards;
	std::vector<long> NumberOfCards;
public:
	DeckOfCards(const std::string dataFile)
	{
		cards = readData(dataFile);
		std::vector<long> onesVector(cards.size(), 1L);
		NumberOfCards = onesVector;
	}

	DeckOfCards() = default;

	long 
	computeDeckPart1()
	{
		return std::accumulate(cards.cbegin(), cards.cend(), 0L, [](auto acc, auto card) { return acc + card.resultOfCard(); });
	}

	long
	computeDeckPart2()
	{
		for (size_t i = 0; i < cards.size(); i++)
		{
			long currentMultiplier = NumberOfCards[i];
			auto newTickets = cards[i].numberOfWinningNumbers();
			for (size_t j = 1; j < newTickets+1; j++)
				NumberOfCards[i + j] += currentMultiplier;
		}
		return std::accumulate(NumberOfCards.begin(), NumberOfCards.end(), 0L);
	}


private:
	std::vector<singleCard>
		readData(std::string dataFile)
	{
		std::vector<singleCard> data;
		std::ifstream inputFile(dataFile);

		std::string line;
		while (std::getline(inputFile, line)) {
			data.push_back(singleCard(removeFirstPartOfInput(line)));
		}
		return data;
	}
};



int main()
{
	auto testCards = DeckOfCards("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day4test.txt");
	auto realCards = DeckOfCards("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day4real.txt");
	
	auto testPart1 = testCards.computeDeckPart1();
	auto realPart1 = realCards.computeDeckPart1();

	auto testPart2 = testCards.computeDeckPart2();
	auto realPart2 = realCards.computeDeckPart2();

	std::cout << "Part 1, test: " << testPart1 << "; and real: " << realPart1;
	std::cout << std::endl;
	std::cout << "Part 2, test: " << testPart2 << "; and real: " << realPart2;
	std::cout << std::endl;

}