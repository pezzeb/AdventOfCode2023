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

	void roolNorthTo(int newPos)
	{
		xPosition = newPos;
	}
	void roolWestTo(int newPos)
	{
		yPosition = newPos;
	}
	void roolSouthTo(int newPos)
	{
		xPosition = newPos;
	}
	void roolEastTo(int newPos)
	{
		yPosition = newPos;
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


	bool sortStonesForNorthRoll(Stone& st1, Stone& st2)
	{
		if (st1.getY() < st2.getY())
			return true;
		else if (st2.getY() < st1.getY())
			return false;
		else
			return st1.getX() < st2.getX();
	}
	void sortNorth()
	{
		std::sort(stones.begin(), stones.end(), [this](Stone a, Stone b) {return this->sortStonesForNorthRoll(a, b); });
	}
	bool sortStonesForWestRoll(Stone& st1, Stone& st2)
	{
		if (st1.getX() < st2.getX())
			return true;
		else if (st2.getX() < st1.getX())
			return false;
		else
			return st1.getY() < st2.getY();
	}
	void sortWest()
	{
		std::sort(stones.begin(), stones.end(), [this](Stone a, Stone b) {return this->sortStonesForWestRoll(a, b); });
	}
	bool sortStonesForSouthRoll(Stone& st1, Stone& st2)
	{
		if (st1.getY() < st2.getY())
			return true;
		else if (st2.getY() < st1.getY())
			return false;
		else
			return st1.getX() > st2.getX();
	}
	void sortSouth()
	{
		std::sort(stones.begin(), stones.end(), [this](Stone a, Stone b) {return this->sortStonesForSouthRoll(a, b); });
	}
	bool sortStonesForEastRoll(Stone& st1, Stone& st2)
	{
		if (st1.getX() < st2.getX())
			return true;
		else if (st2.getX() < st1.getX())
			return false;
		else
			return st1.getY() > st2.getY();
	}
	void sortEast()
	{
		std::sort(stones.begin(), stones.end(), [this](Stone a, Stone b) {return this->sortStonesForEastRoll(a, b); });
	}

public:
	platform(std::string inputPath)
	{
		stones = readData(inputPath);
	}

	void roolPlatformNorth()
	{
		sortNorth();
		roolPlatformGeneric(
			lowestX,
			0,
			[](int a, Stone s) {return s.getY() == a; },
			[](int a, Stone s) {return s.getY() == a + 1; },
			[](int a) { return a + 1; },
			[](int a) { return a + 1; },
			[](std::vector<Stone>::iterator s) {return s->getX() + 1; },
			[](std::vector<Stone>::iterator s, int newPos) {s->roolNorthTo(newPos);}
		);
	}
	void roolPlatformWest()
	{
		sortWest();
		roolPlatformGeneric(
			lowestY,
			0,
			[](int a, Stone s) {return s.getX() == a; },
			[](int a, Stone s) {return s.getX() == a + 1; },
			[](int a) { return a + 1; },
			[](int a) { return a + 1; },
			[](std::vector<Stone>::iterator s) {return s->getY() + 1; },
			[](std::vector<Stone>::iterator s, int newPos) {s->roolWestTo(newPos); }
		);
	}
	void roolPlatformSouth()
	{
		sortSouth();
		roolPlatformGeneric(
			hightestX - 1,
			0,
			[](int a, Stone s) {return s.getY() == a; },
			[](int a, Stone s) {return s.getY() == a + 1; },
			[](int a) { return a + 1; },
			[](int a) { return a - 1; },
			[](std::vector<Stone>::iterator s) {return s->getX() - 1; },
			[](std::vector<Stone>::iterator s, int newPos) {s->roolSouthTo(newPos); }
		);
	}
	void roolPlatformEast()
	{
		sortEast();
		roolPlatformGeneric(
			hightestY - 1,
			0,
			[](int a, Stone s) {return s.getX() == a; },
			[](int a, Stone s) {return s.getX() == a + 1; },
			[](int a) { return a + 1; },
			[](int a) { return a - 1; },
			[](std::vector<Stone>::iterator s) {return s->getY() - 1; },
			[](std::vector<Stone>::iterator s, int newPos) {s->roolEastTo(newPos); }
		);
	}


	void roolPlatformGeneric(
		int resetPos, 
		int startColon, 
		const std::function<bool(int currentColonOrRow, Stone s)>& findCurrentColonOrRow,
		const std::function<bool(int currentColonOrRow, Stone s)>& findNextColonOrRow,
		const std::function<int(int currentColonOrRow)>& updateCurrentColonOrRow,
		const std::function<int(int currentColonOrRow)>& updateResetPositionInLoop,
		const std::function<int(std::vector<Stone>::iterator s)>& updateResetPositionifNotRoolable,
		const std::function<void(std::vector<Stone>::iterator s, int newPos)>& rollStone)
	{
		auto currentStone = stones.begin();
		auto itNextColon = stones.begin();
		int currentColonOrRow = startColon;
		int resetPosition = resetPos;
		std::vector<Stone>::iterator itCurrentColon;

		while (currentStone!=stones.end())
		{
			if (currentStone == itNextColon)
			{
				resetPosition = resetPos;
				itCurrentColon = std::find_if(currentStone, stones.end(), [findCurrentColonOrRow, currentColonOrRow](Stone s) {return findCurrentColonOrRow(currentColonOrRow, s); });
				itNextColon = std::find_if(currentStone, stones.end(), [findNextColonOrRow, currentColonOrRow](Stone s) {return findNextColonOrRow(currentColonOrRow, s); });
				currentColonOrRow = updateCurrentColonOrRow(currentColonOrRow);
			}
			if (!currentStone->isRoolable())
			{
				resetPosition = updateResetPositionifNotRoolable(currentStone);
			}
			else
			{
				rollStone(currentStone, resetPosition);
				resetPosition = updateResetPositionInLoop(resetPosition);
			}
			currentStone++;
		}
	}

	void cycle()
	{
		this->roolPlatformNorth();
		this->roolPlatformWest();
		this->roolPlatformSouth();
		this->roolPlatformEast();
	}

	std::vector<std::tuple<int, int>> encodePlatform()
	{
		std::vector<std::tuple<int, int>> tmp;
		for (auto stone : stones)
		{
			if (stone.isRoolable())
			{
				tmp.push_back(std::tuple<int, int>(stone.getX(),stone.getY()));
			}
		}
		return tmp;
	}

	int computeValue()
	{
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

	int computePart1()
	{
		this->roolPlatformNorth();
		return computeValue();
	}
	int computePart2()
	{
		long long restCycles;
		std::vector<std::vector<std::tuple<int, int>>> previousEncodings;
		while (true)
		{
			this->cycle();
			auto tmpCoding = this->encodePlatform();
			auto it = std::find(previousEncodings.begin(), previousEncodings.end(), tmpCoding);
			if (it == previousEncodings.end())
			{
				previousEncodings.push_back(tmpCoding);
			}
			else
			{
				auto diffN = it- previousEncodings.begin();
				auto numberOfCyclesInLoop = previousEncodings.size() - diffN;
				auto kola = (1000000000 - diffN)/ numberOfCyclesInLoop;
				auto asdf = kola * numberOfCyclesInLoop;
				restCycles = 1000000000 - diffN - asdf;
				break;
			}
		}
		for (size_t i = 0; i < restCycles-1; i++)
		{
			this->cycle();
		}
		return computeValue();
	}
};


int main()
{

	auto platformTest1 = platform("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day14test.txt");
	auto platformReal1 = platform("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day14real.txt");

	auto testPart1 = platformTest1.computePart1();
	auto realPart1 = platformReal1.computePart1();
	std::cout << "Part1 test: " << testPart1 << " real: " << realPart1 << std::endl;

	auto testPart2 = platformTest1.computePart2();
	auto realPart2 = platformReal1.computePart2();
	std::cout << "Part 2 test: " << testPart2 << " real: " << realPart2 << std::endl;

	auto kola = 90;
}