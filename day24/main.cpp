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


struct Point
{
	long long x, y, z;


public:
	Point(long long xin, long long yin, long long zin)
	{
		x = xin;
		y = yin;
		z = zin;
	}

	Point() = default;

	auto operator<=>(const Point&) const = default;

	bool isParallel2D(Point h2)
	{
		if ((this->x == 0 and h2.x == 0) or (this->y == 0 and h2.y == 0))
			return true;
		else
		{
			//double xSlope = this->x / h2.x;
			//double ySlope = this->y / h2.y;
			return this->x * h2.y == this->y * h2.x;
		}
	}

};

class HailStone
{
	Point startingPoint;
	Point velocity;

public:
	HailStone(std::string strIn)
	{
		//strIn.replace(",", "");
		std::replace(strIn.begin(), strIn.end(), ',', ' ');
		std::string xstr, ystr, zstr, dummy, velx, vely, velz;
		std::istringstream iss(strIn);

		iss >> xstr >> ystr >> zstr >> dummy >>velx >>vely >>velz;
		
		startingPoint = Point(stoll(xstr), stoll(ystr), stoll(zstr));
		velocity = Point(stoll(velx), stoll(vely), stoll(velz));
	}

	Point getStartPoint()
	{
		return startingPoint;
	}

	Point getVelocity()
	{
		return velocity;
	}
};

class Storm
{
	std::vector<HailStone> hailstones;

	Point lowerLimits;
	Point upperLimits;

	std::vector<HailStone>
		readData(std::string dataFile)
	{
		std::ifstream inputFile(dataFile);
		std::vector<HailStone> data;
		std::string line;

		while (std::getline(inputFile, line)) {
			//std::istringstream iss(line);
			data.push_back(HailStone(line));
		}
		return data;
	}



	bool passingHailStones2D(HailStone& h1, HailStone& h2)
	{
		if (h1.getVelocity().isParallel2D(h2.getVelocity()))
		{
			return false;
		}
		else
		{
			
			auto xDiff = h2.getStartPoint().x - h1.getStartPoint().x;
			auto yDiff = h2.getStartPoint().y - h1.getStartPoint().y;
			double lhs = xDiff * h1.getVelocity().y - yDiff * h1.getVelocity().x;
			double rhsf = h2.getVelocity().y * h1.getVelocity().x - h2.getVelocity().x * h1.getVelocity().y;
			double lam2 = lhs / rhsf;
			double lam1 = (xDiff + lam2 * h2.getVelocity().x) / h1.getVelocity().x;
			
			if (lam1 < 0 or lam2 < 0) //Have passed in the path
				return false;

			auto intersectingX = h2.getStartPoint().x + lam2 * h2.getVelocity().x;
			auto intersectingY = h2.getStartPoint().y + lam2 * h2.getVelocity().y;

			if ((lowerLimits.x < intersectingX and intersectingX < upperLimits.x) and (lowerLimits.y < intersectingY and intersectingY < upperLimits.y))
			{
				return true;
			}
			else
			{
				return false;
			}

			//x1 + lamb1*vx1 = x2 + lamb2*vx2 \iff 
			//y1 + lamb1*vy1 = y2 + lamb2*vy2
			// lamb1 = (x2 - x1 + lamb2*vx2)/vx1 = (y2 - y1 + lamb2*vy2)/vy1 \iff 
			// (x2 - x1 + lamb2*vx2)vy1 = (y2 - y1 + lamb2*vy2)vx1 \iff 
			// (x2 - x1)*vy1 + lamb2*vx2*vy1 = (y2 - y1)vx1 + lamb2*vy2*vx1 \iff 
			// (x2 - x1)*vy1 - (y2 - y1)vx1=  + lamb2(vy2*vx1 - vx2*vy1)\iff 
			// (x2 - x1)*vy1 - (y2 - y1)vx1 =  + lamb2(vy2*vx1 - vx2*vy1)\iff 
		}
	}

public:
	Storm(std::string pathToStones, long long lower, long long upper)
	{
		hailstones = readData(pathToStones);
		lowerLimits = Point(lower, lower, lower);
		upperLimits = Point(upper, upper, upper);
	}

	long long computePart1()
	{
		long long cumsum = 0;
		for (size_t i = 0; i < hailstones.size(); i++)
		{
			for (size_t j = i+1; j < hailstones.size(); j++)
			{
				auto h1 = hailstones[i];
				auto h2 = hailstones[j];
				if(passingHailStones2D(h1,h2))
					cumsum += 1;
			}
		}
		return cumsum;
	}

};

int main()
{

	//auto h1 = HailStone("19, 13, 30 @ -2,  1, -2");
	auto stormNTest = Storm("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day24test.txt", 7, 27);
	auto part1Test = stormNTest.computePart1();

	auto stormNReal = Storm("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day24real.txt", 200000000000000, 400000000000000);
	auto part1Real = stormNReal.computePart1();

	std::cout << "Part 1 test: " << part1Test << "; real " << part1Real << std::endl; // 15921 is too low

	auto ending = 90;
}