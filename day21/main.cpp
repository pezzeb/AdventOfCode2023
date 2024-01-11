#include <iostream>
#include <vector>
#include <map>
#include <set>
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
#include <functional>


std::vector<std::string>
splitStringIntoVector(const std::string inputString, const char delimiter = ',')
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

enum typeOfState {START, GARDEN, ROCK };

enum direction {UP, DOWN, RIGHT, LEFT, NONE };

class State
{
private:
	typeOfState typeState;
	int xpos, ypos;


public:

	State() = default;

	State(char ch, int xposin, int yposin) 
	{
		xpos = xposin;
		ypos = yposin;

		if (ch == 'S')
		{ 
			typeState = START;
		}
		else if (ch == '#')
		{ 
			typeState = ROCK;
		}
		else
		{ 
			typeState = GARDEN;
		}
	}

	typeOfState getType()
	{
		return typeState;
	}
	std::pair<int, int> getPairOfPos()
	{
		return std::pair<int, int>{xpos, ypos};
	}

};

std::vector<State>
readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);
	std::vector<State> data;
	std::string line;

	int i = 0;
	while (std::getline(inputFile, line))
	{
		int j = 0;
		for (auto ch : line)
		{
			data.push_back(State(ch,i,j));
			j++;
		}
		i++;
	}
	return data;
}

std::vector<std::pair<int, int>> stepper(std::vector<std::pair<int, int>> currentOuterPoints)
{
	std::vector<std::pair<int, int>> pointsToTestAsOuter;

	for (auto cc : currentOuterPoints)
	{
		//auto ijjk = cc.first;
		pointsToTestAsOuter.push_back(std::pair<int, int>{cc.first - 1, cc.second});
		pointsToTestAsOuter.push_back(std::pair<int, int>{cc.first + 1, cc.second});
		pointsToTestAsOuter.push_back(std::pair<int, int>{cc.first, cc.second - 1});
		pointsToTestAsOuter.push_back(std::pair<int, int>{cc.first, cc.second + 1});
	}
	return pointsToTestAsOuter;
}

void removeElements(std::vector<std::pair<int, int>>& a, std::vector<std::pair<int, int>>& b)
{
	a.erase(remove_if(begin(a), end(a),
		[&](auto x) {return find(begin(b), end(b), x) != end(b); }), end(a));
}

long long computePart1(std::vector<State> vecOfState, int stepsToTake)
{
	
	auto itStart = std::find_if(vecOfState.begin(), vecOfState.end(), [](State st) {return st.getType() == START; });
	std::pair<int, int> startPoint = itStart->getPairOfPos();
	
	std::vector<std::pair<int, int>> visitedPoints;
	visitedPoints.push_back(startPoint);


	std::vector<std::pair<int, int>> gardenPoints;
	for (auto ss : vecOfState)
	{
		if (ss.getType() == GARDEN or ss.getType()==START)
			gardenPoints.push_back(ss.getPairOfPos());
	}
	std::sort(gardenPoints.begin(), gardenPoints.end());

	std::vector<std::pair<int, int>> tmpSet;
	tmpSet.push_back(startPoint);
	for (size_t i = 0; i < stepsToTake; i++)
	{
		
		std::vector<std::pair<int, int>> pointsThatArePOssibleOutsidePoints, pointsThatArePOssibleOut;
		tmpSet = stepper(tmpSet);
	
		std::sort(tmpSet.begin(), tmpSet.end());

		// Using default function
		auto it1 = std::set_intersection(gardenPoints.begin(), gardenPoints.end(), tmpSet.begin(), tmpSet.end(), std::back_inserter(pointsThatArePOssibleOutsidePoints));
		
		//removeElements(pointsThatArePOssibleOutsidePoints, visitedPoints);

		pointsThatArePOssibleOut = pointsThatArePOssibleOutsidePoints;

		//visitedPoints.insert(visitedPoints.end(), pointsThatArePOssibleOut.begin(), pointsThatArePOssibleOut.end());
		tmpSet = pointsThatArePOssibleOut;

	}

	std::sort(visitedPoints.begin(), visitedPoints.end());

	auto last = std::unique(visitedPoints.begin(), visitedPoints.end());
	visitedPoints.erase(last, visitedPoints.end());

	return tmpSet.size();
}


int main()
{
	auto testData = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day21test.txt");
	auto part1Test = computePart1(testData, 6);

	auto realData = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day21real.txt");
	auto part1Real = computePart1(realData, 64);

	std::cout << "Part 1 test: " << part1Test << "; real: " << part1Real << std::endl;

	auto kola = 90;
}