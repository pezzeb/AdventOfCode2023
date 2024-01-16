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
	return substrings;
}

enum splitterOrientation { VERTICAL, HORIZONTAL };
enum mirrorOrientation { FORWARDSLASH, BACKWARDSLASH };

class LaserBeamPoint
{
	long long x;
	long long y;
	long long vx;
	long long vy;
public:
	LaserBeamPoint(long long xin, long long yin, long long vxin, long long vyin)
	{
		//x is rows, and y is cols
		x = xin;
		y = yin;
		vx = vxin;
		vy = vyin;
	}

	long long getX() { return x; }
	long long getY() { return y; }
	long long getVelX() { return vx; }
	long long getVelY() { return vy; }
	void setVelX(long long vel) { vx = vel; }
	void setVelY(long long vel) { vy = vel; }

	void printBeam()
	{
		std::cout << x << ";" << y << std::endl;
	}

	bool isValidBeam(long long maxX, long long maxY)
	{
		return x >= 0 and y >= 0 and x <= maxX and y <= maxY;
	}

	void takeStep()
	{
		x += vx;
		y += vy;
	}

};

class SquareGrid
{
	bool hasBeenEnergized;
public:
	// Pure Virtual Function
	virtual bool isEnergized() = 0;
	virtual void energize() = 0;
	virtual std::vector<LaserBeamPoint> processLaser(LaserBeamPoint& las) = 0;
};

class EmptySpace : public SquareGrid
{
	bool hasBeenEnergized;
public:
	EmptySpace()
	{
		hasBeenEnergized = false;
	}
	bool isEnergized() override {return hasBeenEnergized;}
	void energize() override { hasBeenEnergized = true; }
	std::vector<LaserBeamPoint> processLaser(LaserBeamPoint& las) override
	{
		return std::vector<LaserBeamPoint>();
	}
};

class Mirror : public SquareGrid
{
	bool hasBeenEnergized;
	mirrorOrientation orientation;
public:
	Mirror(mirrorOrientation orientationIn)
	{
		hasBeenEnergized = false;
		orientation = orientationIn;
	}
	bool isEnergized() override { return hasBeenEnergized; }
	void energize() override { hasBeenEnergized = true; }
	std::vector<LaserBeamPoint> processLaser(LaserBeamPoint& las) override
	{
		auto tmpBeamer = std::vector<LaserBeamPoint>();
		if ((las.getVelX() > 0 and orientation == FORWARDSLASH) or (las.getVelX() < 0 and orientation == BACKWARDSLASH))
		{
			las.setVelY(-1);
			las.setVelX(0) ;
		}
		else if ((las.getVelX() < 0 and orientation == FORWARDSLASH) or (las.getVelX() > 0 and orientation == BACKWARDSLASH))
		{
			las.setVelY(1);
			las.setVelX(0);
		}
		else if ((las.getVelY() > 0 and orientation == FORWARDSLASH) or (las.getVelY() < 0 and orientation == BACKWARDSLASH))
		{
			las.setVelY(0);
			las.setVelX(-1);
		}
		else if ((las.getVelY() < 0 and orientation == FORWARDSLASH) or (las.getVelY() > 0 and orientation == BACKWARDSLASH))
		{
			las.setVelY(0);
			las.setVelX(1);
		}

		return tmpBeamer;
	}
};

class Splitter : public SquareGrid
{
	bool hasBeenEnergized;
	splitterOrientation orientation;
public:
	Splitter(splitterOrientation orientationIn)
	{
		hasBeenEnergized = false;
		orientation = orientationIn;
	}
	bool isEnergized() override { return hasBeenEnergized; }
	void energize() override { hasBeenEnergized = true; }
	std::vector<LaserBeamPoint> processLaser(LaserBeamPoint& las) override
	{
		auto tmpBeamer = std::vector<LaserBeamPoint>();
		if (las.getVelX()!=0 and orientation== HORIZONTAL)
		{
			tmpBeamer.push_back(LaserBeamPoint(las.getX(), las.getY(), 0, 1));
			tmpBeamer.push_back(LaserBeamPoint(las.getX(), las.getY(), 0, -1));
		}
		else if (las.getVelY() != 0 and orientation == VERTICAL)
		{
			tmpBeamer.push_back(LaserBeamPoint(las.getX(), las.getY(), 1, 0  ));
			tmpBeamer.push_back(LaserBeamPoint(las.getX(), las.getY(), -1, 0 ));
		}

		return tmpBeamer;
	}
};


std::vector<std::vector<SquareGrid*>>
readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);
	std::vector < std::vector<SquareGrid*>> data;
	std::string line;

	int i = 0;
	while (std::getline(inputFile, line))
	{
		int j = 0;
		std::vector<SquareGrid*> tmpData;
		for (auto ch : line)
		{
			if (ch == '.')
			{
				tmpData.emplace_back(new EmptySpace());
			}
			else if (ch == '|')
			{
				tmpData.emplace_back(new Splitter(VERTICAL));
			}
			else if (ch == '-')
			{
				tmpData.emplace_back(new Splitter(HORIZONTAL));
			}
			else if (ch == '/')
			{
				tmpData.emplace_back(new Mirror(FORWARDSLASH));
			}
			else 
			{
				tmpData.emplace_back(new Mirror(BACKWARDSLASH));
			}
			j++;
		}
		data.emplace_back(tmpData);
		i++;
	}
	return data;
}


class playingField
{
	std::vector<std::vector<SquareGrid*>> squares;
	std::vector<LaserBeamPoint> historyOfLaserBeams;

	bool checkIfBeamsHasBeenThere(LaserBeamPoint &las)
	{
		auto it = std::find_if(historyOfLaserBeams.begin(), historyOfLaserBeams.end(), [&las](LaserBeamPoint lasin) {
			return las.getVelX() == lasin.getVelX() and las.getVelY() == lasin.getVelY() and las.getX() == lasin.getX() and las.getY() == lasin.getY();
			});

		return it != historyOfLaserBeams.end();
	}
	bool addIfNecessary(LaserBeamPoint las)
	{
		if (checkIfBeamsHasBeenThere(las))
		{
			//found and hence it is not necessary to add
			return true;
		}
		else
		{
			historyOfLaserBeams.push_back(las);
			return false;
		}
	}

public:
	std::vector<LaserBeamPoint> laserBeamsVec;

	playingField(std::string pathToInput)
	{
		squares = readData(pathToInput);
		laserBeamsVec.push_back(LaserBeamPoint(0, -1, 0, 1));
		squares[0][0]->energize();
		//historyOfLaserBeams.push_back(laserBeamsVec.back());
	}
	void runLaserBeam()
	{
		LaserBeamPoint las = laserBeamsVec.back();
		laserBeamsVec.pop_back();

		while (true)
		{
			las.takeStep();
			//las.printBeam();

			if(addIfNecessary(las))
			{
				//the laser beam has already been there in that state so we can break here 
				break;
			}
			else
			{
				if (las.isValidBeam(squares.size()-1, squares[0].size()-1))
				{
					auto squ = squares[las.getX()][las.getY()];
					squ->energize();
					auto newBeams = squ->processLaser(las);
					if (newBeams.size() > 0)
					{
						laserBeamsVec.insert(laserBeamsVec.end(), newBeams.begin(), newBeams.end());
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	void part1runner()
	{

		while (laserBeamsVec.size()>0)
		{
			runLaserBeam();
		}
	}

	long long countEnergized()
	{
		long long counter = 0;
		for (size_t r = 0; r < squares.size(); r++)
		{
			for (size_t c = 0; c < squares[0].size(); c++)
			{
				counter += squares[r][c]->isEnergized() ? 1 : 0;
			}
		}
		return counter;
	}

};

int main()
{
	auto playFieldPart1test = playingField("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day16test.txt");
	playFieldPart1test.part1runner();
	auto numberOfEnergizeTestPart1 = playFieldPart1test.countEnergized();

	auto playFieldPart1real = playingField("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day16real.txt");
	playFieldPart1real.part1runner();
	auto numberOfEnergizeRealPart1 = playFieldPart1real.countEnergized(); //120 is too low

	std::cout << "Part 1, test: " << numberOfEnergizeTestPart1 << "; and real: " << numberOfEnergizeRealPart1 << std::endl;

	auto ending = 90;
}