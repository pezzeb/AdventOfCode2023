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
#include <queue>
#include <regex>


std::vector<std::string> readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);
	std::string line;
	std::vector<std::string> data;

	while (std::getline(inputFile, line))
	{
		data.push_back(line);
	}
	return data;
}

enum Direction { UP, LEFT, RIGHT, DOWN};
using position = std::pair<int, int>;



class SinglePath
{
	position currentFrontiers;
	Direction lastStepCameFrom = UP;
	bool isEndedOrStuck = false;

	//long long stepsTaken /// this can be derived from the length of the vector...

	//std::map<position, long long> longestToComeToAspecificPoint; // I think is overkill...and regardless it should be in the SinglePath

public:

	std::vector<position> actualPath;



	SinglePath(position pos)
	{
		currentFrontiers = pos;
		actualPath.push_back(pos);
	}

	void updatePath(position newPos)
	{
	
		if (newPos.first < currentFrontiers.first)
			lastStepCameFrom = DOWN;
		else if (newPos.first > currentFrontiers.first)
			lastStepCameFrom = UP;
		else if (newPos.second < currentFrontiers.second)
			lastStepCameFrom = RIGHT;
		else if (newPos.second > currentFrontiers.second)
			lastStepCameFrom = LEFT;
	
		currentFrontiers = newPos;
		actualPath.push_back(newPos);
	
	}

	position getCurrentFrontier()
	{
		return currentFrontiers;
	}
	Direction getlastStepCameFrom()
	{
		return lastStepCameFrom;
	}
	void declareStuckOrEnded()
	{
		isEndedOrStuck = true;
	}

	long long getCurrentNumberofSteps()
	{
		return actualPath.size();
	}
	bool checkIfDone()
	{
		return isEndedOrStuck;
	}
};

struct MyStruct
{
public:
	bool possible;
	position pos;

	MyStruct(bool p, position posN)
	{
		possible = p;
		pos = posN;
	}
	
};

class Field
{
	std::vector<std::string> blocks;
	position firstPosition;
	position endingPosition;

public:
	//if it would be possible to go on a slope, then return THE SLOPE. Next time it is enough just to give the next one. 
	//I think that this is the easiest way of not get lose of steps... 
	
	Field() = default;

	Field(std::string path)
	{
		blocks = readData(path);
		firstPosition = position(0, 1);
	}
	
	position getFirstPosition()
	{
		return firstPosition;
	}

	/*std::vector<position> getPossibleDirection(position curPos)
	{

	}*/
	std::vector<position> getPossibleDirection(position curPos, Direction cameFrom)
	{
		std::vector<position> listOfNewPositions;
		//enum Direction { UP, DOWN, LEFT, RIGHT };

		// Go UP   on the next step
		if (curPos.first - 1 >= 0 and cameFrom != UP)
		{
			auto newPositionBlock = blocks[curPos.first - 1][curPos.second];
			if (newPositionBlock != '#' and newPositionBlock != 'v')
			{
				//This is valid
				listOfNewPositions.push_back(position(curPos.first - 1, curPos.second));
			}
		}
		// Go DOWN on the next step
		if (curPos.first + 1 < blocks.size() and cameFrom != DOWN)
		{
			auto newPositionBlock = blocks[curPos.first + 1][curPos.second];
			if (newPositionBlock != '#' and newPositionBlock != '^')
			{
				//This is valid
				listOfNewPositions.push_back(position(curPos.first + 1, curPos.second));
			}
		}
		//Go LEFT on the next step
		if (curPos.second - 1>= 0 and cameFrom != LEFT)
		{
			auto newPositionBlock = blocks[curPos.first][curPos.second - 1];
			if (newPositionBlock != '#' and newPositionBlock != '>')
			{
				//This is valid
				listOfNewPositions.push_back(position(curPos.first, curPos.second - 1));
			}
		}
		//Go RIGHT on the next step
		if (curPos.second + 1 < blocks[0].size() and cameFrom != RIGHT)
		{
			auto newPositionBlock = blocks[curPos.first][curPos.second + 1];
			if (newPositionBlock != '#' and newPositionBlock != '<')
			{
				//This is valid
				listOfNewPositions.push_back(position(curPos.first, curPos.second + 1));
			}
		}
		return listOfNewPositions;

	}

	std::vector<SinglePath> generateNewPath(SinglePath path)
	{
		std::vector<SinglePath> returnPaths;
		auto newDirections = this->getPossibleDirection(path.getCurrentFrontier(), path.getlastStepCameFrom());
		if (newDirections.empty())
		{
			path.declareStuckOrEnded();
			returnPaths.push_back(path);
		}
		else
		{
			for (auto nd : newDirections) //This is not optimal in neither speed nor memory...
			{
				auto copyOfPath = path;
				copyOfPath.updatePath(nd);
				returnPaths.push_back(copyOfPath);
			}
		}
		return returnPaths;
	}
};

class PathIndexer
{
private:
	Field field;
	std::vector<SinglePath> paths;

	bool checkIfDone()
	{
		for (auto pp : paths)
		{
			if (!pp.checkIfDone())
				return false;
		}
		return true;
	}

	void initilizePath()
	{
		paths.push_back(SinglePath(field.getFirstPosition()));
	}

public:

	PathIndexer(std::string path)
	{
		field = Field(path);
		initilizePath();
	}

	void advanceAstep()
	{
		std::vector<SinglePath> newPaths;
		for (auto pp : paths)
		{
			if (!pp.checkIfDone())//if not done then we continue
			{
				auto kola = field.generateNewPath(pp);
				newPaths.insert(newPaths.end(), kola.begin(), kola.end());
			}
			else
			{
				newPaths.push_back(pp);
			}
		}
		paths = newPaths;
	}

	void untilEnd()
	{
		while (!checkIfDone()) 
		{
			advanceAstep();
		}
	}

	long long longestStep()
	{
		auto tmp = 
			std::max_element(paths.begin(), paths.end(), [](SinglePath a, SinglePath b)
			{
				return a.actualPath.size() < b.actualPath.size();
			});
		return tmp->actualPath.size() - 1;
	}

};

int main()
{// A first observation i can perhaps use is that it is only right (>) and downward (v)

	auto indexerTest1 = PathIndexer("C:/Users/soder/source/repos/pezzeb/AdventOfCode2023/data/day23test.txt");
	auto indexerReal1 = PathIndexer("C:/Users/soder/source/repos/pezzeb/AdventOfCode2023/data/day23real.txt");
	auto indexerTest2 = PathIndexer("C:/Users/soder/source/repos/pezzeb/AdventOfCode2023/data/day23test2.txt");
	auto indexerReal2 = PathIndexer("C:/Users/soder/source/repos/pezzeb/AdventOfCode2023/data/day23real2.txt");

	indexerTest1.untilEnd();
	indexerReal1.untilEnd();

	std::cout << "part 1 test:" << indexerTest1.longestStep() << ";; and we have real: " << indexerReal1.longestStep();
	
	indexerTest1.untilEnd();
	indexerReal1.untilEnd();
	std::cout << "part 2 test:" << indexerTest2.longestStep() << ";; and we have real: " << indexerReal2.longestStep();

	std::cout << "nu er slut!";
}