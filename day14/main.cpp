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


class Stone
{
	bool roolable;
	int xPosition;
	int yPosition;
public:
	Stone(bool _roolable, int x, int y)
	{
		roolable = _roolable;
		xPosition = x;
		yPosition = y;
	}

	void roolNorth(int distance)
	{
		xPosition -= distance;
	}
	void roolNorthTo(int newPos)
	{
		xPosition = newPos;
	}
	void roolSouth(int distance)
	{
		xPosition += distance;
	}
	void roolWest(int distance)
	{
		xPosition -= distance;
	}
	void roolEast(int distance)
	{
		yPosition += distance;
	}

	const int getX()
	{
		return xPosition;
	}
	const int getY()
	{
		return yPosition;
	}
	const bool isRoolable()
	{
		return roolable;
	}
		

};


class platform
{
	std::vector<Stone> stones;
	int lowestX = 0;
	int lowestY = 0;
	int hightestX;
	int hightestY;


	std::vector<Stone> readData(std::string dataFile)
	{
		std::ifstream inputFile(dataFile);
		std::string line;
		std::vector<Stone> data;

		int x = 0;
		hightestX = 0;
		while (std::getline(inputFile, line))
		{
			hightestX++;
			hightestY = line.size();
			std::istringstream iss(line);
			for (size_t y = 0; y < line.size(); y++)
			{
				if(line[y]=='O')
					data.push_back(Stone(true,x,y));
				else if (line[y] == '#')
					data.push_back(Stone(false, x, y));
			}
			x++;
		}
		return data;
	}

	//std::vector<std::vector<Stone>> 
	//groupByVertically(std::vector<Stone> input)
	//{
	//	for (size_t i = 0; i < hightestY; i++)
	//	{
	//		for (size_t j = 0; j < hightestX; j++) {

	//		}
	//	};
	//}


public:

	bool sortStonesVertically(Stone& st1, Stone& st2)
	{
		if (st1.getY() < st2.getY())
			return true;
		else if (st2.getY() < st1.getY())
			return false;
		else
			return st1.getX() < st2.getX();
	}

	platform(std::string inputPath)
	{
		stones = readData(inputPath);
		std::sort(stones.begin(), stones.end(), [this](Stone a, Stone b) {return this->sortStonesVertically(a,b); });
	}

	void roolPlatformNorth()
	{
		roolPlatformGeneric();
	}


	void roolPlatformGeneric()
	{
		auto currentStone = stones.begin();
		auto itNextColon = stones.begin();
		int currentColon = 0;
		int lowestPossible = lowestX;
		std::vector<Stone>::iterator itCurrentColon;
		while (currentStone!=stones.end())
		{
			if (currentStone == itNextColon)
			{
				lowestPossible = lowestX;
				itCurrentColon = std::find_if(currentStone, stones.end(), [currentColon](Stone s) {return s.getY() == currentColon; });
				itNextColon = std::find_if(currentStone, stones.end(), [currentColon](Stone s) {return s.getY() == currentColon+1; });
				currentColon++;
			}
			if (!currentStone->isRoolable())
			{
				lowestPossible = currentStone->getX() + 1;
			}
			else
			{
				currentStone->roolNorthTo(lowestPossible);
				lowestPossible++;
			}
			currentStone++;
		}
	}

	int computePart1()
	{
		this->roolPlatformNorth();
		int cumsum = 0;
		for (auto stone : stones)
		{
			if (stone.isRoolable())
			{
				auto tmp = (hightestY - stone.getX());
				cumsum += tmp;
			}
				
		}
		return cumsum;
	}
};


int main()
{

	auto platformTest1 = platform("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day14test.txt");
	auto platformReal1 = platform("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day14real.txt");

	auto testPart1 = platformTest1.computePart1();
	auto realPart1 = platformReal1.computePart1();

	//for (size_t i = 0; i < 1000; i++)
	//{
	//	std::cout << i << std::endl;
	//	platformReal1.roolPlatformNorth();
	//}


	std::cout << "Part1 test: " << testPart1 << " real: " << realPart1 << std::endl;

	auto kola = 90;
}