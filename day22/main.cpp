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
	std::vector<std::string> output;
	for (auto ss : substrings)
	{
		output.push_back(ss);
	}
	return output;
}

enum brickOrientation { XBAR, YBAR, ZBAR, CUBE };

class Position
{
public:
	long long x = 0, y = 0, z = 0;

	Position() = default;

	Position(std::string str)
	{
		auto pars = splitStringIntoVector(str);
		x = std::stoll(pars[0]);
		y = std::stoll(pars[1]);
		z = std::stoll(pars[2]);
	}
	long long getX()
	{
		return x;
	}
	long long getY()
	{
		return y;
	}
	long long getZ()
	{
		return z;
	}
	void reduceZ(long long dz)
	{
		z -= dz;
	}
	void reduceZ()
	{
		reduceZ(1);
	}
};

class Brick // This is a abstract class
{
	Position pos1;
	Position pos2;

	bool isStationary = false;

public:

	bool isExisting = true;
	brickOrientation orientationOfBrick;

	Brick(std::string str)
	{
		auto stringPositions = splitStringIntoVector(str, '~');
		pos1 = Position(stringPositions[0]);
		pos2 = Position(stringPositions[1]);

		if (pos1.getX() == pos2.getX() and pos1.getY() == pos2.getY() and pos1.getZ() == pos2.getZ())
			orientationOfBrick = CUBE;
		else if (pos1.getX() == pos2.getX() and pos1.getY() == pos2.getY())
			orientationOfBrick = ZBAR;
		else if (pos1.getX() == pos2.getX() and pos1.getZ() == pos2.getZ())
			orientationOfBrick = YBAR;
		else
			orientationOfBrick = XBAR;
	}

	Position getPos1()
	{
		return pos1;
	}

	Position getPos2()
	{
		return pos2;
	}

	void applyGravity()
	{
		pos1.reduceZ(1);
		pos2.reduceZ(1);
	}
	void reverseGravity()
	{
		pos1.reduceZ(-1);
		pos2.reduceZ(-1);
	}

	long long findMinimumZ()
	{
		return std::min(pos1.getZ(), pos2.getZ());
	}

};

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



class collectionOfBricks
{
	std::vector<Brick> bricks;

	bool isRangeLeftInsideTheRight(double left1, double left2, double right1, double right2)
	{
		// we assume the left1 <= left2 ; and right1 <= right2
		if (right1 <= left1 and left1 <= right2)
			return true;
		if (right1 <= left2 and left2 <= right2)
			return true;
		if (left1 <= right1 and left2 >= right2)
			return true;
		return false;
	}
	
	bool isRangeLeftInsideTheRight(Brick& br1, Brick& br2)
	{
		return
			isRangeLeftInsideTheRight(br1.getPos1().x, br1.getPos2().x, br2.getPos1().x, br2.getPos2().x) and
			isRangeLeftInsideTheRight(br1.getPos1().y, br1.getPos2().y, br2.getPos1().y, br2.getPos2().y) and
			isRangeLeftInsideTheRight(br1.getPos1().z, br1.getPos2().z, br2.getPos1().z, br2.getPos2().z);
	}

	bool overLappingbricks(Brick& br1, Brick& br2)
	{ //https://stackoverflow.com/questions/55220355/how-to-detect-whether-two-segments-in-3d-space-intersect
		
		auto br1IsInside2 = isRangeLeftInsideTheRight(br1, br2);
		auto br2IsInside1 = isRangeLeftInsideTheRight(br1, br2); //Not sure that I need both...

		if (!br1IsInside2 and !br2IsInside1) 
			return false;
		else
			return true;
	}

	void sortBricks()
	{
		std::sort(bricks.begin(), bricks.end(), [](Brick& lhs, Brick& rhs)
			{
				return lhs.findMinimumZ() < rhs.findMinimumZ();
			});
	}

public:

	collectionOfBricks(std::vector<std::string> bricksInStringFormat)
	{
		for (auto vv : bricksInStringFormat)
		{
			bricks.push_back(Brick(vv));
		}
		sortBricks();

	}
	
	bool checkIfOverlapping(long long i)
	{
		//for (size_t i = 0; i < bricks.size(); i++)
		//{
			Brick& brickN = bricks[i];
			if (brickN.isExisting)
			{
				if (brickN.findMinimumZ() == 0)//have passed through the floor, so it is overlapping
				{
					return true;
				}
				else
				{
					for (size_t j = 0; j <= i; j++)
					{
						if (i != j)
						{
							if (bricks[j].isExisting and overLappingbricks(brickN, bricks[j])) //overlapping
							{
								return true;
							}
						}
					}
				}
			}
		//}
		return false;
	}

	bool checkIfOverlapping()
	{
		for (size_t i = 0; i < bricks.size(); i++)
		{
			Brick& brickN = bricks[i];
			if (brickN.isExisting)
			{
				if (brickN.findMinimumZ() == 0)//have passed through the floor, so it is overlapping
				{
					return true;
				}
				else
				{
					for (size_t j = 0; j <= i; j++)
					{
						if (i != j)
						{
							if (bricks[j].isExisting and overLappingbricks(brickN, bricks[j])) //overlapping
							{
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	}

	void stablizePile()
	{
		for (size_t i = 0; i < bricks.size(); i++)
		{
			sortBricks();
			Brick& brickN = bricks[i];
			bool overlappingBrick = false;
			while (not overlappingBrick)
			{
				brickN.applyGravity();
				overlappingBrick = checkIfOverlapping(i);

				if (overlappingBrick)
				{
					brickN.reverseGravity();
				}
			}
		}
		sortBricks();
	}

	long long countNonRedunant()
	{
		long long cumsum = 0;
		for (size_t i = 0; i < bricks.size(); i++)
		{
			std::cout << i << ";";
			Brick& brickN = bricks[i];
			brickN.isExisting = false;
			bool overlappingBrick = false;
			bool somethingHasFallen = false;

			for (long long j = i + 1; j < bricks.size(); j++)
			{
				Brick& brickLoop = bricks[j];
				brickLoop.applyGravity();
				bool LooseBrick = checkIfOverlapping(j); //if NO overLapping it could move freely

				if (!LooseBrick) //if not overlapping it could move freely -> nonRedundant was removed
				{
					if (!somethingHasFallen) //Debugging but not to double count
						cumsum += 1;
					somethingHasFallen = true;
				}
				brickLoop.reverseGravity();
			}
			brickN.isExisting = true;
		}
		std::cout << std::endl;
		return bricks.size() - cumsum;
	}

	void makeAllExistAgain()
	{
		for (long long i = bricks.size() - 1; i >= 0; i--) // Compute from the top
			bricks[i].isExisting = true;
	}

	long long countNonExisting()
	{
		long long cumsum = 0;
		for (long long i = bricks.size() - 1; i >= 0; i--) // Compute from the top
		{
			if (!bricks[i].isExisting)
				cumsum++;
		}
		return cumsum;
	}

	//0 - A; 1 - B; 2 - C; 3 - D; 4 - E; 5 - F; 6 - G
	long long countDisintegrating()
	{
		long long cumsum = 0;
		for (long long i = bricks.size()-1; i >= 0; i--) // Compute from the top
		{
			std::cout << i << ";";
			Brick& brickN = bricks[i];
			brickN.isExisting = false;
			bool overlappingBrick = false;
			bool somethingHasFallen = false;

			for (long long j = i + 1; j < bricks.size(); j++) //check the ones that are above
			{
				Brick& brickLoop = bricks[j];
				brickLoop.applyGravity();
				bool LooseBrick = checkIfOverlapping(j); //if NO overLapping it could move freely

				if (!LooseBrick) //if not overlapping it could move freely -> nonRedundant was removed
				{
					brickLoop.isExisting = false;
					//somethingHasFallen = true;
				}
				brickLoop.reverseGravity();
			}
			long long tmpsum = countNonExisting() - 1; // one is non existing per default
			cumsum += tmpsum;
			makeAllExistAgain();
		}
		std::cout << std::endl;
		return cumsum;
	}

	

	void applyGravity() 
	{
		//sort bricks in order of z-component, and only necessary to check previously stable bricks

		for (auto br : bricks)
		{
			br.applyGravity();
		}
	}

	long long computePar1()
	{
		stablizePile();
		return countNonRedunant();
	}
	long long computePar2()
	{
		stablizePile(); //This is perhaps redundant
		return countDisintegrating();
	}
};



int main()
{
	auto testData = readData("C:/Users/soder/source/repos/pezzeb/AdventOfCode2023/data/day22.txt");
	auto testCollection = collectionOfBricks(testData);
	long long testPart1RedundantBricks = testCollection.computePar1();
	long long testPart2RedundantBricks = testCollection.computePar2();
	std::cout << "Test part 1: " << testPart1RedundantBricks << std::endl;
	std::cout << "Test part 2: " << testPart2RedundantBricks << std::endl;

	auto realData = readData("C:/Users/soder/source/repos/pezzeb/AdventOfCode2023/data/day22real.txt");
	auto realCollection = collectionOfBricks(realData);
	long long realPart1RedundantBricks = realCollection.computePar1();
	long long realPart2RedundantBricks = realCollection.computePar2();
	std::cout << "Test part 1: " << realPart1RedundantBricks << std::endl;
	std::cout << "Test part 2: " << realPart2RedundantBricks << std::endl;



	//1499 is too high!

	int asdf = 90;
}