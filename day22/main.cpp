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
	bool isExisting = true;
	bool isStationary = false;

public:

	Brick(std::string str)
	{
		auto stringPositions = splitStringIntoVector(str, '~');
		pos1 = Position(stringPositions[0]);
		pos2 = Position(stringPositions[1]);
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

	bool overLappingbricks(Brick& br1, Brick& br2)
	{
		long long dp_x = br1.getPos1().x - br2.getPos1().x;
		long long dp_y = br1.getPos1().y - br2.getPos1().y;
		long long dp_z = br1.getPos1().z - br2.getPos1().z;
		
		long long v1_x = br1.getPos1().x - br1.getPos2().x;
		long long v1_y = br1.getPos1().y - br1.getPos2().y;
		long long v1_z = br1.getPos1().z - br1.getPos2().z;

		long long v2_x = br2.getPos1().x - br2.getPos2().x;
		long long v2_y = br2.getPos1().y - br2.getPos2().y;
		long long v2_z = br2.getPos1().z - br2.getPos2().z;

		long long w_x = v1_y * v2_z - v1_z * v2_y;
		long long w_y = v1_z * v2_x - v1_x * v2_z;
		long long w_z = v1_x * v2_y - v1_y * v2_x;

		auto res = w_x * dp_x + w_y * dp_y + w_z * dp_z;

		return res == 0;
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
	
	bool stablizePile()
	{
		bool stable = true;
		sortBricks();
		for (size_t i = 0; i < bricks.size(); i++)
		{
			Brick& brickN = bricks[i];
			bool overlappingBrick = false;
			while (not overlappingBrick)
			{
				brickN.applyGravity();
				if (brickN.findMinimumZ() == 0)
				{
					brickN.reverseGravity();
					overlappingBrick = true;
				}
				else
				{
					for (long long j = i - 1; j >= 0; j--)
					{
						overlappingBrick = overlappingBrick or overLappingbricks(brickN, bricks[j]);
					}
					if (overlappingBrick) //overlapping
					{
						brickN.reverseGravity();
						overlappingBrick = true;
					}
					else
						stable = false;
				}
			}
		}
		return stable;
	}
	long long countRedunant()
	{
		
	}

	void applyGravity() 
	{
		//sort bricks in order of z-component, and only necessary to check previously stable bricks

		for (auto br : bricks)
		{
			br.applyGravity();
		}
	}

};




int main()
{
	auto testData = readData("C:/Users/soder/source/repos/pezzeb/AdventOfCode2023/data/day22.txt");
	auto testCollection = collectionOfBricks(testData);

	testCollection.stablizePile();

	//hanterar inte kuber korrekt i matchning...


	int asdf = 90;
}