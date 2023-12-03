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
	// different member versions of find in the same order as above:
	std::size_t found = str.find(str2);
	std::string substring = str.substr(found + 2);
	return substring;
}

class singleDraw {
public:
	std::unordered_map<std::string, int> myDictionary;

	singleDraw(std::string input)
	{
		int i;
		std::string s;
		auto vectorOfInputs = splitStringIntoVector(input, ',');
		
		for (auto ll : vectorOfInputs)
		{
			std::istringstream iss(ll);
			iss >> i >> s;
			myDictionary[s] = i;
		}
	}

	long
	getValueOrDefault(const std::string& key, long defaultValue) {
		auto it = myDictionary.find(key);
		return (it != myDictionary.end()) ? it->second : defaultValue;
	};
};

class singleGame {
public:
	std::vector<singleDraw> singleDraws;

	
	// Constructor to initialize members
	singleGame(std::string& input)
	{
		auto vectorOfSingleDraws = splitStringIntoVector(input, ';');
		for (auto draw : vectorOfSingleDraws)
		{
			auto tmp = singleDraw(draw);
			singleDraws.push_back(tmp);
		}
	}
	
	bool 
	part1computer()
	{
		auto dictN = findMaxDict();
		auto it = dictN.find("red");
		if (it == dictN.end() or ((it->second) > 12L))
			return false;
		it = dictN.find("green");
		if (it == dictN.end() or ((it->second) > 13L))
			return false;
		it = dictN.find("blue");
		if (it == dictN.end() or ((it->second) > 14L))
			return false;

		return true;
	}

	long
	part2computer()
	{
		auto dictN = findMaxDict();
		auto itRed = dictN.find("red");
		auto itGreen = dictN.find("green");
		auto itBlue = dictN.find("blue");
		
		return (itRed->second)* (itGreen->second)* (itBlue->second);
	}

private:
	std::unordered_map<std::string, long>
	findMaxDict()
	{
		std::unordered_map<std::string, long> maxDictionary;
		std::vector<std::string> colors {"red","green","blue"};

		for (auto const color : colors)
		{
			maxDictionary[color] = 0;
		}

		for (auto draw : singleDraws)
		{
			for (auto const color : colors)
			{
				auto tmp = draw.getValueOrDefault(color, 0L);
				maxDictionary[color] = (maxDictionary[color] > tmp ? maxDictionary[color] : tmp);
			}
		}
		return maxDictionary;
	}
};



std::vector<singleGame>
readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);

	std::vector<singleGame> data; 

	std::string line;
	while (std::getline(inputFile, line)) {
		auto lineRhss = removeFirstPartOfInput(line);
		auto vectorOfDraws = splitStringIntoVector(lineRhss);
		auto game = singleGame(lineRhss);

		data.push_back(game);

		//data.push_back(row); // Add the row to the vector of rows
	}
	return data;
}

long 
firstPart(std::vector<singleGame>games)
{
	std::vector<bool> okeyGame;
	long cumsum = 0;
	for (size_t ijk = 0; ijk < games.size(); ijk++)
	{
		auto game = games[ijk];
		if (game.part1computer())
			cumsum += (ijk + 1);
	}
	return cumsum;
}
long
secondPart(std::vector<singleGame>games)
{
	std::vector<long> prodGame;
	
	for (auto game : games)
	{
		prodGame.push_back(game.part2computer());
	}
	
	long summan = std::accumulate(prodGame.begin(), prodGame.end(), 0, [](int acc, int element) { return acc + element; });

	return summan;
}

int main()
{
	auto testGames = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day2test.txt");
	auto part1Test = firstPart(testGames);
	auto part2Test = secondPart(testGames);

	auto realGames = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day2real.txt");
	auto part1Real = firstPart(realGames);
	auto part2Real = secondPart(realGames);

	std::cout << "Part 1, test: " << part1Test << "; and real: " << part1Real;
	std::cout << std::endl;
	std::cout << "Part 2, test: " << part2Test << "; and real: " << part2Real;

	// Part 1 
	/*std::vector<std::string> stringVector = { "1abc2", "pqr3stu8vwx", "a1b2c3d4e5f", "treb7uchet" };
	auto part1test = firstPart("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/AoC2023solution/data/day1test.txt");
	auto part1real = firstPart("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/AoC2023solution/data/day1real.txt");*/
	//std::cout << "Part 1, test: " << part1test << "; and real: " << part1real;

	std::cout << std::endl;

	//Part 2 
	/*auto part2test = secondPart("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/AoC2023solution/data/day1test2.txt");
	auto part2real = secondPart("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/AoC2023solution/data/day1real.txt");*/
	//std::cout << "Part 2, test: " << part2test << "; and real: " << part2real;

}