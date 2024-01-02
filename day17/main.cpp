#include <iostream>
#include <vector>
#include <map>
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


#include <iostream>

#include "aStar.h"

#include "MathLibrary.h"

std::vector<std::vector<int>> readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);
	std::string line;

	std::vector<std::string> tmp;

	std::vector<std::vector<int>> output;

	while (std::getline(inputFile, line))
	{
		std::vector<int> tmpLine;
		for (auto ch : line)
		{
			tmpLine.push_back(int(ch) - int('0'));
		}

		output.push_back(tmpLine);
	}

	return output;
}

enum Direction { Left, Right, Up, Down, None};


class part2Class
{
public:
	int x, y;
	int nRows;
	int nCols;
	std::vector<std::vector<int>>* heatMap;
	std::vector<Direction> previousDirection;
	double fScore;
	int minimumOfSteps;// = 4; //these are steps and not block
	int maximumOfSteps;// = 10;

	part2Class()
	{
		heatMap = nullptr; // std::vector<std::vector<int>>();
		x = 0;
		y = 0;
		nRows = -1;
		nCols = -1;
		{
			previousDirection = std::vector<Direction>{ None };
		}
		fScore = std::numeric_limits<double>::infinity();
		minimumOfSteps = 4;//these are steps and not block
		maximumOfSteps = 10; 

	}

	part2Class(int xin, int yin, std::vector<std::vector<int>>* heatMapIn, int minIn=4, int maxIn=10) //these are steps and not block
	{
		heatMap = heatMapIn;
		x = xin;
		y = yin;
		nRows = heatMap->size() - 1; //This is a bad name for the highest idnex
		//auto kola = heatMap->front().size() - 1;
		nCols = heatMap->front().size() - 1;

		previousDirection = std::vector<Direction>{ None };
		fScore = std::numeric_limits<double>::infinity();

		minimumOfSteps = minIn;
		maximumOfSteps = maxIn;

	}

	Direction const getNoGoDirection() const
	{
		if (previousDirection.size() < maximumOfSteps)
			return None;
		else
		{
			std::vector<Direction> tmpVec(previousDirection.end() - maximumOfSteps, previousDirection.end());
			sort(tmpVec.begin(), tmpVec.end());
			std::vector<Direction>::iterator it;
			it = std::unique(tmpVec.begin(), tmpVec.end());
			tmpVec.resize(std::distance(tmpVec.begin(), it));
			if (tmpVec.size() == 1)
				return tmpVec[0];
			else
				return None;
		}
	}

	std::vector<Direction> const streakOfDirection() const
	{
		std::vector<Direction> kola;
		kola.push_back(*previousDirection.rbegin());
		for (auto it = previousDirection.rbegin()+1; it != previousDirection.rend(); it++)
		{
			if (kola.back() == *it)
				kola.push_back(*it);
			else
				return kola;
		}
		return kola;
	}

	const bool isLockedState()
	{
		auto currentStreak = streakOfDirection();

		int nCurrentStreak = currentStreak.size();
		auto lastDirect = previousDirection.back();


		if (lastDirect == None)
			nCurrentStreak = 0;
		bool isLockedDirection = nCurrentStreak < minimumOfSteps and lastDirect != None;
		return isLockedDirection;
	}

	const std::vector<part2Class> getNeighbor()
	{
		std::vector<part2Class> tmp;

		auto noGoDirection = getNoGoDirection();

		auto lastDirect = previousDirection.back();

		auto currentStreak = streakOfDirection();

		int nCurrentStreak = currentStreak.size();

		if (lastDirect == None)
			nCurrentStreak = 0;

		//no point in starting in a Direction unless four is possible - CHECK

		//DO NOT GENERATE IF nCurrentSTREAK IS LESS THEN four - 

		bool isLockedDirection = nCurrentStreak < minimumOfSteps and lastDirect!=None;

		auto minstepDifferential = (minimumOfSteps - nCurrentStreak);
		minstepDifferential = std::max<int>(minstepDifferential, 0);

		auto asdfasdf = x - minstepDifferential;
		auto asdfasdfasdf = asdfasdf > 0;

		   // Have to be able to take all steps, has to be able to 
		if (x - minstepDifferential >= 0 and x > 0 and noGoDirection != Up and lastDirect != Down and (!isLockedDirection or  lastDirect == Up)) //Go up
		{
			auto cc = part2Class(x - 1, y, heatMap);
			cc.previousDirection = previousDirection;
			cc.previousDirection.push_back(Up);
			tmp.push_back(cc);
		}
		if (x + minstepDifferential <= nRows and x < nRows and noGoDirection != Down and lastDirect != Up and (!isLockedDirection or lastDirect == Down)) //Go down
		{
			auto cc = part2Class(x + 1, y, heatMap);
			cc.previousDirection = previousDirection;
			cc.previousDirection.push_back(Down);
			tmp.push_back(cc);
		}
		if (y - minstepDifferential >= 0 and y > 0 and noGoDirection != Left and lastDirect != Right and (!isLockedDirection or lastDirect == Left))
		{
			auto cc = part2Class(x, y - 1, heatMap);
			cc.previousDirection = previousDirection;
			cc.previousDirection.push_back(Left);
			tmp.push_back(cc);
		}
		if (y + minstepDifferential  <= nCols and y < nCols and noGoDirection != Right and lastDirect != Left and (!isLockedDirection or lastDirect == Right))
		{
			auto cc = part2Class(x, y + 1, heatMap);
			cc.previousDirection = previousDirection;
			cc.previousDirection.push_back(Right);
			tmp.push_back(cc);
		}
		return tmp;
	}


	const bool operator<(const part2Class& rhs) const
	{
		if (x < rhs.x)
			return true;
		else  if (x > rhs.x)
			return false;
		else if (y < rhs.y)
			return true;
		else if (y > rhs.y)
			return false;

		std::vector<Direction> lhsPD;
		std::vector<Direction> rhsPD;

		lhsPD = streakOfDirection();
		rhsPD = rhs.streakOfDirection();

		if (lhsPD.size() < rhsPD.size())
			return true;
		else if (lhsPD.size() > rhsPD.size())
			return false;
		else
			return lhsPD.front() < rhsPD.front();


	}
	const bool operator==(const part2Class& rhs) const
	{
		if (*this < rhs)
			return false;
		else if (rhs < *this)
			return false;
		else
			return true;
	}

	

};

class part1Class
{
public:

	int x, y;
	int nRows;
	std::vector<std::vector<int>>* heatMap;
	std::vector<Direction> previousDirection;
	double fScore;

	part1Class()
	{
		heatMap = nullptr; // std::vector<std::vector<int>>();
		x = 0;
		y = 0;
		nRows = - 1;
		{
			previousDirection = std::vector<Direction>{None};
		}
		fScore = std::numeric_limits<double>::infinity();
	}

	part1Class(int xin, int yin, std::vector<std::vector<int>>* heatMapIn)
	{
		heatMap = heatMapIn;
		x = xin;
		y = yin;
		nRows = heatMap->size() - 1;

		previousDirection = std::vector<Direction>{None};
		fScore = std::numeric_limits<double>::infinity();

	}

	Direction const getNoGoDirection() const
	{

		if (previousDirection.size() < 3)
			return None;
		else
		{
			std::vector<Direction> tmpVec(previousDirection.end() - 3, previousDirection.end());
			sort(tmpVec.begin(), tmpVec.end());
			std::vector<Direction>::iterator it;
			it = std::unique(tmpVec.begin(), tmpVec.end());
			tmpVec.resize(std::distance(tmpVec.begin(), it));
			if (tmpVec.size() == 1)
				return tmpVec[0];
			else
				return None;
		}
	}

	std::vector<Direction> const streakOfDirection() const
	{
		std::vector<Direction> kola;
		kola.push_back(*previousDirection.rbegin());
		for (auto it = previousDirection.rbegin()+1; it != previousDirection.rend(); it++)
		{
			if (kola.back() == *it)
				kola.push_back(*it);
			else
				return kola;
		}
		return kola;
	}


	const bool isLockedState()
	{
		return false;
	}

	const std::vector<part1Class> getNeighbor()
	{
		std::vector<part1Class> tmp;

		auto noGoDirection = getNoGoDirection();
		auto lastDirect = previousDirection.back();

		if (x > 0 and noGoDirection!=Up and lastDirect!=Down)
		{
			auto cc = part1Class(x - 1, y, heatMap);
			cc.previousDirection = previousDirection;
			cc.previousDirection.push_back(Up);
			tmp.push_back(cc);
		}
		if (x < nRows and noGoDirection != Down and lastDirect != Up)
		{
			auto cc = part1Class(x + 1, y, heatMap);
			cc.previousDirection = previousDirection;
			cc.previousDirection.push_back(Down);
			tmp.push_back(cc);
		}
		if (y > 0 and noGoDirection != Left and lastDirect != Right)
		{
			auto cc = part1Class(x, y - 1, heatMap);
			cc.previousDirection = previousDirection;
			cc.previousDirection.push_back(Left);
			tmp.push_back(cc);
		}
		if (y < nRows and noGoDirection != Right and lastDirect != Left)
		{
			auto cc = part1Class(x, y + 1, heatMap);
			cc.previousDirection = previousDirection;
			cc.previousDirection.push_back(Right);
			tmp.push_back(cc);
		}
		return tmp;
	}
	
	const bool operator<(const part1Class& rhs) const
	{
		if (x < rhs.x)
			return true;
		else  if (x > rhs.x)
			return false;
		else if (y < rhs.y)
			return true;
		else if (y > rhs.y)
			return false;

		std::vector<Direction> lhsPD;
		std::vector<Direction> rhsPD;

		lhsPD = streakOfDirection();
		rhsPD = rhs.streakOfDirection();

		if (lhsPD.size() < rhsPD.size())
			return true;
		else if (lhsPD.size() > rhsPD.size())
			return false;
		else
			return lhsPD.front() < rhsPD.front();


	}
	const bool operator==(const part1Class& rhs) const
	{
		if (*this < rhs)
			return false;
		else if (rhs < *this)
			return false;
		else
			return true;
	}
};

double hfunc(const part1Class cc)
{
	return cc.nRows * 2 - cc.y - cc.x; //no heat loss when you are in the point
}

double dfunc(const part1Class cc, const part1Class dd)
{
	return (*dd.heatMap)[dd.x][dd.y];
}

double hfunc2(const part2Class cc)
{
	return cc.nRows + cc.nCols - cc.y - cc.x; //no heat loss when you are in the point
}

double dfunc2(const part2Class cc, const part2Class dd)
{
	return (*dd.heatMap)[dd.x][dd.y];
}

double computeHeatLoss(const std::vector<part1Class> vec)
{
	int cumsum = 0;
	for (auto vv : vec)
	{
		auto delta = (*vv.heatMap)[vv.x][vv.y];
		cumsum += delta;
	}
	return cumsum;
}
double computeHeatLoss(const std::vector<part2Class> vec)
{
	int cumsum = 0;
	for (auto vv : vec)
	{
		auto delta = (*vv.heatMap)[vv.x][vv.y];
		cumsum += delta;
	}
	return cumsum;
}

int main()
{
	auto aStarAlgorithComputer = AstarAlgorithm<part1Class>();

	//https://www.reddit.com/r/adventofcode/comments/18llbkq/day_17_part_2_need_some_tips/

	//auto heatMapTest = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day17testPart2.txt"); //Works in part 2
	//auto heatMapTest = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day17test.txt");  //Works in part 2
	//auto startTest = part1Class(0, 0, &heatMapTest);
	//auto endingTest = part1Class(heatMapTest.size() - 1, heatMapTest.size() - 1, &heatMapTest);
	//auto pathNTest = aStarAlgorithComputer.A_Star(startTest, endingTest, &hfunc, &dfunc);
	//std::reverse(pathNTest.begin(), pathNTest.end());
	//auto part1test = computeHeatLoss(pathNTest);

	auto heatMapTest = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day17real.txt");
	/*auto startReal = part1Class(0, 0, &heatMapReal);
	auto endingReal = part1Class(heatMapReal.size() - 1, heatMapReal.size() - 1, &heatMapReal);
	auto pathNReal = aStarAlgorithComputer.A_Star(startReal, endingReal, &hfunc, &dfunc);
	std::reverse(pathNReal.begin(), pathNReal.end());
	auto part1real = computeHeatLoss(pathNReal);*/
	
	//std::cout << "Part 1 test:" << part1test - heatMapTest[0][0] << std::endl;
	//std::cout << "Part 1 real:" << part1real - heatMapReal[0][0] << std::endl;

	auto aStarAlgorithComputer2 = AstarAlgorithm<part2Class>();

	auto startTest2 = part2Class(0, 0, &heatMapTest);
	auto endingTest2 = part2Class(heatMapTest.size()-1, heatMapTest[0].size()-1, &heatMapTest); 
	auto pathNTest2 = aStarAlgorithComputer2.A_Star(startTest2, endingTest2, &hfunc2, &dfunc2);
	std::reverse(pathNTest2.begin(), pathNTest2.end());
	auto part2test = computeHeatLoss(pathNTest2);

	std::cout << "Part 2 test:" << part2test - heatMapTest[0][0] << std::endl; //1398 is too high

    return 0;


}

