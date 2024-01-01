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

class dummyClass
{
public:

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
		for (auto it = previousDirection.rbegin(); it != previousDirection.rend(); it++)
		{
			if (kola.back() == *it)
				kola.push_back(*it);
			else
				return kola;
		}
		return kola;
	}


	const Direction tmpeFunc() const
	{
		return None;
	}

	int x, y;

	int nRows;

	std::vector<std::vector<int>>* heatMap;

	std::vector<Direction> previousDirection;

	double fScore;

	dummyClass()
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

	dummyClass(int xin, int yin, std::vector<std::vector<int>>* heatMapIn)
	{
		heatMap = heatMapIn;
		x = xin;
		y = yin;
		nRows = heatMap->size() - 1;

		previousDirection = std::vector<Direction>{None};
		fScore = std::numeric_limits<double>::infinity();

	}

	const std::vector<dummyClass> getNeighbor()
	{
		std::vector<dummyClass> tmp;

		auto noGoDirection = getNoGoDirection();
		auto lastDirect = previousDirection.back();

		if (x > 0 and noGoDirection!=Up and lastDirect!=Down)
		{
			auto cc = dummyClass(x - 1, y, heatMap);
			cc.previousDirection = previousDirection;
			cc.previousDirection.push_back(Up);
			tmp.push_back(cc);
		}
		if (x < nRows and noGoDirection != Down and lastDirect != Up)
		{
			auto cc = dummyClass(x + 1, y, heatMap);
			cc.previousDirection = previousDirection;
			cc.previousDirection.push_back(Down);
			tmp.push_back(cc);
		}
		if (y > 0 and noGoDirection != Left and lastDirect != Right)
		{
			auto cc = dummyClass(x, y - 1, heatMap);
			cc.previousDirection = previousDirection;
			cc.previousDirection.push_back(Left);
			tmp.push_back(cc);
		}
		if (y < nRows and noGoDirection != Right and lastDirect != Left)
		{
			auto cc = dummyClass(x, y + 1, heatMap);
			cc.previousDirection = previousDirection;
			cc.previousDirection.push_back(Right);
			tmp.push_back(cc);
		}
		return tmp;
	}
	
	const bool operator<(const dummyClass& rhs) const
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
	const bool operator==(const dummyClass& rhs) const
	{
		if (*this < rhs)
			return false;
		else if (rhs < *this)
			return false;
		else
			return true;
	}


};

double hfunc(const dummyClass cc)
{
	return cc.nRows * 2 - cc.y - cc.x; //no heat loss when you are in the point
}

double dfunc(const dummyClass cc, const dummyClass dd)
{
	return (*dd.heatMap)[dd.x][dd.y];
}

double computeHeatLoss(const std::vector<dummyClass> vec)
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

	//auto heatMapN = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day17test.txt");
	auto heatMapN = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day17real.txt");

	//fScore is 0 and NOT inf?

    auto asdf = AstarAlgorithm<dummyClass>();
    auto start = dummyClass(0, 0, &heatMapN);
    auto ending = dummyClass(heatMapN.size()-1, heatMapN.size()-1, &heatMapN);

	std::cout << start.fScore << std::endl;

    auto pathN = asdf.A_Star(start, ending, &hfunc, &dfunc);
	std::reverse(pathN.begin(), pathN.end());


	auto part1test = computeHeatLoss(pathN);

	std::cout << "Part 1 test:" << part1test - heatMapN[0][0] << std::endl;

    return 0;


}

