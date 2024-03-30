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

enum Direction { UP, LEFT, RIGHT, DOWN };
using position = std::pair<int, int>;
using edge = std::pair<position, position>;



class SinglePath
{
	position currentFrontiers;
	Direction lastStepCameFrom = UP;
	bool isStuck = false;
	bool isEnded = false;

public:

	std::vector<position> actualPath;

	SinglePath(position pos)
	{
		currentFrontiers = pos;
		actualPath.push_back(pos);
	}
	SinglePath(position pos, Direction lastStepCameFrom_)
	{
		currentFrontiers = pos;
		actualPath.push_back(pos);
		lastStepCameFrom = lastStepCameFrom_;
	}

	edge getStartEdge()
	{
		return edge(actualPath[0], actualPath[1]);
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

	position getStartPoint()
	{
		return actualPath[0];
	}


	void declareStuck()
	{
		//std::cout << std::endl << "the stuck: " << actualPath.size() - 1;

		isStuck = true;
	}
	void declareEnded()
	{
		//std::cout << std::endl << "the end: " << actualPath.size() - 1;
		isEnded = true;
	}

	void declareStuckOrEnded(position endingPosition)
	{
		if (currentFrontiers == endingPosition)
			declareEnded();
		else
			declareStuck();
	}

	long long getCurrentNumberofSteps()
	{
		return actualPath.size();
	}
	bool checkIfDone()
	{
		return isStuck || isEnded;
	}
	bool checkIfEnded()
	{
		return isEnded;
	}
	bool checkIfStuck()
	{
		return isStuck;
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
	
	//std::map<position, position> networkMapper;
	std::vector<edge> visitedPoints;
	std::map<edge, long long> edgeLength;

	std::queue<SinglePath> pointsToStartFrom;

	void updateEdgeLength(position pos1, position pos2, long long lengthOfEdge)
	{
		auto oldLength = edgeLength[edge(pos1, pos2)];
		if(lengthOfEdge>oldLength)
			edgeLength[edge(pos1, pos2)] = lengthOfEdge;
	}

	bool generateNewPath(SinglePath& path)
	{
		std::vector<position> newDirections = this->getPossibleDirection(path.getCurrentFrontier(), path.getlastStepCameFrom());
		
		if (newDirections.empty())
		{
			updateEdgeLength(path.getStartPoint(), endingPosition, path.actualPath.size() - 1);
			//edgeLength[edge(path.getStartPoint(), endingPosition)] = path.actualPath.size()-1;
			//auto currentJunction = path.getStartEdge();
			visitedPoints.push_back(path.getStartEdge());

			//networkMapper.insert(std::make_pair(path.getStartPoint(), endingPosition));
		}
		else if (newDirections.size() > 1 || newDirections.empty())
		{
			auto currentJunction = path.getCurrentFrontier();

			//networkMapper.insert(std::make_pair(path.getStartPoint(), currentJunction));
			visitedPoints.push_back(path.getStartEdge());
			updateEdgeLength(path.getStartPoint(), currentJunction, path.actualPath.size() - 1);
			//edgeLength[edge(path.getStartPoint(), currentJunction)] = path.actualPath.size() - 1;

			for (auto nd : newDirections)
			{
				auto tmpSinglePath = SinglePath(currentJunction);//nd, path.getlastStepCameFrom()
				tmpSinglePath.updatePath(nd);
				pointsToStartFrom.push(tmpSinglePath);
			}
			return true;
		}
		else
		{
			auto nd = newDirections[0];
			auto it = std::find(path.actualPath.begin(), path.actualPath.end(), nd);
			if (it == path.actualPath.end())
			{
				path.updatePath(nd);
			}
			else
			{
				path.declareStuckOrEnded(endingPosition);
			}
			return false;
		}
	}

	void findEndOrNextJunction(SinglePath junctionStartPoint)
	{
		auto it = std::find(visitedPoints.begin(), visitedPoints.end(), junctionStartPoint.getStartEdge());
		
		if (it == visitedPoints.end())//point has bit been visited
		{
			while (!generateNewPath(junctionStartPoint))
			{
				auto asdf = 91; // This is a bit stupid perhaps
			}
		}
	}

	void buildEdgeMap()
	{
		while (!pointsToStartFrom.empty()) //This might is prettier to fix with a for loop.
		{
			findEndOrNextJunction(pointsToStartFrom.front());
			pointsToStartFrom.pop();
		}
	}

	void initiliser()
	{
		auto tmpSinglePath = SinglePath(position(0, 1), UP);
		tmpSinglePath.updatePath(position(1, 1));
		pointsToStartFrom.push(tmpSinglePath); 
		buildEdgeMap();
	}

public:

	Field() = default;

	Field(std::string path)
	{
		blocks = readData(path);
		firstPosition = position(0, 1);
		endingPosition = position(blocks.size() - 1, blocks.size() - 2);
		initiliser();
	}

	position getFirstPosition()
	{
		return firstPosition;
	}
	
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
		if (curPos.second - 1 >= 0 and cameFrom != LEFT)
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

	std::vector<position> getPossiblejunction(position pos)
	{
		std::vector<position> tmp;
		for (auto edge : edgeLength)
		{
			if (edge.first.first == pos)
				tmp.push_back(edge.first.second); //Might need to add mor e
			/*else if (edge.first.second == pos)
				tmp.push_back(edge.first.first);*/
		}
		return tmp; 
	}

	position getEnding()
	{
		return endingPosition;
	}

	std::map<edge, long long> getEdgeLength()
	{
		return edgeLength;
	}


};

class PathIndexer
{
private:
	Field field;

	std::queue<SinglePath> paths;

public:

	PathIndexer(std::string path)
	{
		field = Field(path);
		paths.push(SinglePath(field.getFirstPosition()));
	}

	long long computeLengthofPath(SinglePath& pathN)
	{
		long long cumsum = 0;
		for (size_t i = 0; i < pathN.actualPath.size()-1; i++)
		{
			edge tmpEdge = edge(pathN.actualPath[i], pathN.actualPath[i + 1]);
			cumsum += field.getEdgeLength()[tmpEdge];
		}
		return cumsum;
	}

	long long findLongestpath()
	{
		long long ijk = 0;
		long long res = 0;
		while (!paths.empty())
		{
			ijk++;
			auto pp = paths.front();
			paths.pop();
		
			if (pp.getCurrentFrontier() == field.getEnding())
			{
				//compute length
				auto pathLength = computeLengthofPath(pp);
				//update res if it is bigger than the current one
				if (pathLength > res)
				{
					res = pathLength;
					std::cout << res << std::endl;
				}
			}
			else
			{
				auto possibleJunction = field.getPossiblejunction(pp.getCurrentFrontier());
				//auto possibleJunction2 = field.getPossiblejunction(position(14,5));
				for (auto junc : possibleJunction)
				{
					//check if junction has already been visited
					if (std::find(pp.actualPath.begin(), pp.actualPath.end(), junc)!=pp.actualPath.end())
					{
						//item has already been visit, so that path we give up on
						auto debuggingBreker = 91;
					}
					else
					{					
						//otherwise add junc to the path and then added to the queue
						auto dummyCopy = pp;
						dummyCopy.updatePath(junc);
						paths.push(dummyCopy);
					}
				}
			}
		}

		return res;
	}

};

int main()
{// A first observation i can perhaps use is that it is only right (>) and downward (v)

	auto indexerTest2 = PathIndexer("C:/Users/soder/source/repos/pezzeb/AdventOfCode2023/data/day23test2.txt");
	auto asdf3 = indexerTest2.findLongestpath();


	auto indexerTest1 = PathIndexer("C:/Users/soder/source/repos/pezzeb/AdventOfCode2023/data/day23test.txt");
	auto asdf1 = indexerTest1.findLongestpath();

	auto indexerReal1 = PathIndexer("C:/Users/soder/source/repos/pezzeb/AdventOfCode2023/data/day23real.txt");
	auto asdf2 = indexerReal1.findLongestpath();

	
	
	auto indexerReal2 = PathIndexer("C:/Users/soder/source/repos/pezzeb/AdventOfCode2023/data/day23real2.txt");
	auto asdf4 = indexerReal2.findLongestpath();

	/*
	indexerReal1.untilEnd();

	std::cout << "part 1 test:" << indexerTest1.longestStep() << ";; and we have real: " << indexerReal1.longestStep();

	indexerTest2.untilEnd();
	auto asdf = indexerTest2.longestStep();
	indexerReal2.untilEnd();
	std::cout << "part 2 test:" << indexerTest2.longestStep() << ";; and we have real: " << indexerReal2.longestStep();
	*/
	std::cout << "nu er slut!";
}

// 
// 
// 
// 
// 
//
// 
