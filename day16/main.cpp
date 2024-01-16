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
	virtual void unEnergize() = 0;
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
	void unEnergize() override { hasBeenEnergized = false; }
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
	void unEnergize() override { hasBeenEnergized = false; }
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
	void unEnergize() override { hasBeenEnergized = false; }
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
	std::vector<std::vector<SquareGrid*>>* squares;
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

	playingField(std::vector<std::vector<SquareGrid*>>* squaresIn)
	{
		squares = squaresIn;
		laserBeamsVec.push_back(LaserBeamPoint(0, -1, 0, 1));
		(*squares)[0][0]->energize();
	}
	playingField(std::vector<std::vector<SquareGrid*>>* squaresIn, long long xin, long long yin, long long vxin, long long vyin)
	{
		squares = squaresIn;
		laserBeamsVec.push_back(LaserBeamPoint(xin, yin, vxin, vyin));
		(*squares)[xin+vxin][yin+vyin]->energize();
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
				if (las.isValidBeam(squares->size()-1, squares[0].size()-1))
				{
					auto tmp = (*squares);
					auto squ = tmp[las.getX()][las.getY()];
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
		for (size_t r = 0; r < squares->size(); r++)
		{
			for (size_t c = 0; c < (*squares)[0].size(); c++)
			{
				counter += (*squares)[r][c]->isEnergized() ? 1 : 0;
			}
		}
		return counter;
	}

	void unEnegerizeGrid()
	{
		for (size_t r = 0; r < squares->size(); r++)
		{
			for (size_t c = 0; c < (*squares)[0].size(); c++)
			{
				(*squares)[r][c]->unEnergize();
			}
		}
	}

	void printEnerigation()
	{
		for (size_t r = 0; r < squares->size(); r++)
		{
			for (size_t c = 0; c < (*squares)[0].size(); c++)
			{
				if ((*squares)[r][c]->isEnergized())
					std::cout << "#";
				else
					std::cout << ".";
				
			}
			std::cout << std::endl;
		}
	}

};



int main()
{
	auto squaresWithMirrorsTest = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day16test.txt");
	auto playFieldPart1test = playingField(&squaresWithMirrorsTest);
	playFieldPart1test.part1runner();
	auto numberOfEnergizeTestPart1 = playFieldPart1test.countEnergized();

	auto squaresWithMirrorsReal = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day16real.txt");
	auto playFieldPart1real = playingField(&squaresWithMirrorsReal);
	playFieldPart1real.part1runner();
	auto numberOfEnergizeRealPart1 = playFieldPart1real.countEnergized();

	std::cout << "Part 1, test: " << numberOfEnergizeTestPart1 << "; and real: " << numberOfEnergizeRealPart1 << std::endl;

	// Part 2 
	auto part2field = squaresWithMirrorsReal;
	auto nRow = part2field.size();
	auto nCol = part2field[0].size();

	//Left Edge
	
	std::vector<long long> energizedCells;

	for (size_t i = 0; i < nRow; i++)
	{
		auto playFieldPart1realtmp = playingField(&part2field, i, -1, 0, 1);
		playFieldPart1realtmp.part1runner();
		auto numberOfEnergizeRealPart2tmp = playFieldPart1realtmp.countEnergized();
		//std::cout << numberOfEnergizeRealPart2tmp << std::endl;
		std::cout << i << ", ";
		energizedCells.push_back(numberOfEnergizeRealPart2tmp);
		playFieldPart1realtmp.unEnegerizeGrid();
	}
	std::cout << std::endl;
	//Right Edge
	for (size_t i = 0; i < nRow; i++)
	{
		auto playFieldPart1realtmp = playingField(&part2field, i, nCol, 0, -1);
		playFieldPart1realtmp.part1runner();
		auto numberOfEnergizeRealPart2tmp = playFieldPart1realtmp.countEnergized();
		//std::cout << numberOfEnergizeRealPart2tmp << std::endl;
		std::cout << i << ", ";
		energizedCells.push_back(numberOfEnergizeRealPart2tmp);
		playFieldPart1realtmp.unEnegerizeGrid();
	}
	std::cout << std::endl;
	//Upper edge 
	for (size_t i = 0; i < nRow; i++)
	{
		auto playFieldPart1realtmp = playingField(&part2field, -1, i, 1, 0);
		playFieldPart1realtmp.part1runner();
		auto numberOfEnergizeRealPart2tmp = playFieldPart1realtmp.countEnergized();
		//std::cout << numberOfEnergizeRealPart2tmp << std::endl;
		energizedCells.push_back(numberOfEnergizeRealPart2tmp);
		std::cout << i << ", ";
		//playFieldPart1realtmp.printEnerigation();
		playFieldPart1realtmp.unEnegerizeGrid();
	}
	std::cout << std::endl;
	//Lower edge 
	for (size_t i = 0; i < nRow; i++)
	{
		auto playFieldPart1realtmp = playingField(&part2field, nCol, i, -1, 0);
		playFieldPart1realtmp.part1runner();
		auto numberOfEnergizeRealPart2tmp = playFieldPart1realtmp.countEnergized();
		//std::cout << numberOfEnergizeRealPart2tmp << std::endl;
		std::cout << i << ", ";
		energizedCells.push_back(numberOfEnergizeRealPart2tmp);
		playFieldPart1realtmp.unEnegerizeGrid();
	}
	std::cout << std::endl;


	auto part2test = *max_element(std::begin(energizedCells), std::end(energizedCells));
	std::cout << "part 2 real: " << part2test;

	auto ending = 90;
}