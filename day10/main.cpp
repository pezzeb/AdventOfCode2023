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


enum Direction { north, west, east, south };


class Pipe
{
private:
	std::string typeOfPipe;

public:

	virtual std::string getType() = 0;
	virtual bool getNewDirection(Direction& entryDirection) = 0;

};

class NorthAndSouth : public Pipe
{
private:
	std::string typeOfPipe = "|";
public:
	std::string getType()
	{
		return typeOfPipe;
	}
	bool getNewDirection(Direction& entryDirection)
	{
		if (entryDirection == north)
		{
			entryDirection = north;
			return true;
		}
		else if (entryDirection == south)
		{
			entryDirection = south;
			return true;
		}
		else
		{
			return false;
		}
	}
};
class WestAndEast : public Pipe
{
private:
	std::string typeOfPipe = "-";
public:
	std::string getType()
	{
		return typeOfPipe;
	}
	bool getNewDirection(Direction& entryDirection)
	{
		if (entryDirection == west)
		{
			entryDirection = west;
			return true;
		}
		else if (entryDirection == east)
		{
			entryDirection = east;
			return true;
		}
		else
		{
			return false;
		}
	}
};
class NorthAndEast : public Pipe
{
private:
	std::string typeOfPipe = "L";
public:
	std::string getType()
	{
		return typeOfPipe;
	}
	bool getNewDirection(Direction& entryDirection)
	{
		if (entryDirection == south)
		{
			entryDirection = east;
			return true;
		}
		else if (entryDirection == west)
		{
			entryDirection = north;
			return true;
		}
		else
		{
			return false;
		}
	}
};
class NorthAndWest : public Pipe
{
private:
	std::string typeOfPipe = "J";
public:
	std::string getType()
	{
		return typeOfPipe;
	}
	bool getNewDirection(Direction& entryDirection)
	{
		if (entryDirection == south)
		{
			entryDirection = west;
			return true;
		}
		else if (entryDirection == east)
		{
			entryDirection = north;
			return true;
		}
		else
		{
			return false;
		}
	}
};
class SouthAndWest : public Pipe
{
private:
	std::string typeOfPipe = "7";
public:
	std::string getType()
	{
		return typeOfPipe;
	}
	bool getNewDirection(Direction& entryDirection)
	{
		if (entryDirection == east)
		{
			entryDirection = south;
			return true;
		}
		else if (entryDirection == north)
		{
			entryDirection = west;
			return true;
		}
		else
		{
			return false;
		}
	}
};
class SouthAndEast : public Pipe
{
private:
	std::string typeOfPipe = "F";
public:
	std::string getType()
	{
		return typeOfPipe;
	}
	bool getNewDirection(Direction& entryDirection)
	{
		if (entryDirection == north)
		{
			entryDirection = east;
			return true;
		}
		else if (entryDirection == west)
		{
			entryDirection = south;
			return true;
		}
		else
		{
			return false;
		}
	}
};
class Start : public Pipe
{
private:
	std::string typeOfPipe = "S";
public:
	std::string getType()
	{
		return typeOfPipe;
	}
	bool getNewDirection(Direction& entryDirection)
	{
		return true;
	}
};
class NoPipe : public Pipe
{
private:
	std::string typeOfPipe = ".";
public:
	std::string getType()
	{
		return typeOfPipe;
	}
	bool getNewDirection(Direction& entryDirection)
	{
		return false;
	}
};



class Ground
{
	std::vector<std::vector<Pipe*>> pipes;
	int nRows, nCols;
	//Direction currentDirection;
	std::tuple<int, int> startCoordinateOfTheGround;

	void readData(std::string dataFile)
	{
		std::ifstream inputFile(dataFile);
		std::string line;
		std::vector<std::string> data;
		//std::vector<std::vector<Pipe>> output;

		while (std::getline(inputFile, line))
		{
			std::vector<Pipe*> tmpOutput;
			for (auto bb : line)
			{
				if (bb == '|')
				{
					tmpOutput.emplace_back(new NorthAndSouth());
				}
				else if (bb == '-')
				{
					tmpOutput.emplace_back(new WestAndEast());
				}
				else if (bb == 'L')
				{
					tmpOutput.emplace_back(new NorthAndEast());
				}
				else if (bb == 'J')
				{
					tmpOutput.emplace_back(new NorthAndWest());
				}
				else if (bb == '7')
				{
					tmpOutput.emplace_back(new SouthAndWest());
				}
				else if (bb == 'F')
				{
					tmpOutput.emplace_back(new SouthAndEast());
				}
				else if (bb == 'S')
				{
					tmpOutput.emplace_back(new Start());
				}
				else
				{
					tmpOutput.emplace_back(new NoPipe());
				}
			}
			pipes.emplace_back(tmpOutput);
		}
		//return output;
	}

	
	std::tuple<int, int> findStart()
	{
		int x = 0;
		int y = 0;

		for (auto itVec = pipes.begin(); itVec != pipes.end(); ++itVec)
		{
			y = 0;
			
			for (auto it= (*itVec).begin(); it!=(*itVec).end(); ++it)
			{
				if ("S" == (*it)->getType())
				{
					return std::tuple<int, int>(x, y);
				}
				y++;
			}
			x++;
		}
	}

public:

	Ground(std::string intputPath)
	{
		readData(intputPath);
		nRows = pipes.size();
		nCols = pipes[0].size();
		startCoordinateOfTheGround = findStart();
	}

	bool newCoordinate(std::tuple<int, int>& startCoordinate, Direction direction)
	{
		std::tuple<int, int> tmp;
		if (direction == north || direction==south)
		{
			auto x = std::get<0>(startCoordinate) + (direction == north ? -1 : 1);
			auto y = std::get<1>(startCoordinate);
			if (x < 0 || x>nRows)
				return false;
			else
				tmp = std::tuple<int, int>(x, y);
		}
		else 
		{
			auto x = std::get<0>(startCoordinate);
			auto y = std::get<1>(startCoordinate) + (direction == west ? -1 : 1);
			if (y < 0 || y>nCols)
				return false;
			else
				tmp = std::tuple<int, int>(x, y);
		}

		std::swap(startCoordinate, tmp);
		return true;
	}

	Pipe* findPipeOfCoordinate(std::tuple<int, int>coordinate)
	{
		auto x = std::get<0>(coordinate);
		auto y = std::get<1>(coordinate);
		return pipes[x][y];
	}

	
	bool validDirection(std::tuple<int,int> currentCoordinate, Direction StartDirection)
	{
		bool nextCoordinate = newCoordinate(currentCoordinate, StartDirection);
		if (!nextCoordinate)
			return false;
		Pipe* nextPipe = findPipeOfCoordinate(currentCoordinate);
		return nextPipe->getNewDirection(StartDirection);
	}

	long long computePart1(std::tuple<int, int> currentCoordinate, Direction currentDirection)
	{
		std::vector<std::tuple<int, int>> loop;
		do 
		{
			loop.push_back(currentCoordinate);
			bool nextCoordinate = newCoordinate(currentCoordinate, currentDirection); //dummy bool
			Pipe* nextPipe = findPipeOfCoordinate(currentCoordinate);
			nextPipe->getNewDirection(currentDirection);
		} while (startCoordinateOfTheGround != currentCoordinate);
		return loop.size() / 2;
	}

	long long computePart1Main()
	{
		

		auto isNorthValid = validDirection(startCoordinateOfTheGround, north);
		auto isWestValid = validDirection(startCoordinateOfTheGround, west);
		auto isEastValid = validDirection(startCoordinateOfTheGround, east);
		auto isSouthValid = validDirection(startCoordinateOfTheGround, south);

		//startNorth
		if (isNorthValid)
		{
			return computePart1(startCoordinateOfTheGround, north);
		}
		else if (isWestValid)
		{
			return computePart1(startCoordinateOfTheGround, west);
		}
		else if (isEastValid)
		{
			return computePart1(startCoordinateOfTheGround, east);
		}
		else
		{
			///SHOULD NOT BE ABLE TO HIT
		}
	}

};


int main()
{
	
	auto groundTest1 = Ground("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day10test1.txt");
	auto groundTest2 = Ground("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day10test2.txt");
	auto groundReal = Ground("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day10real.txt");
	auto test1 = groundTest1.computePart1Main();
	auto test2 = groundTest2.computePart1Main();
	auto real = groundReal.computePart1Main();

	std::cout << "Part 1, test 1: " << test1 << "; test2 :" << test2 << "; and real: " << real << std::endl;
	
}

