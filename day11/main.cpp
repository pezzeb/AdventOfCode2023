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



std::vector<std::string> readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);
	std::string line;
	std::vector<std::string> data;

	while (std::getline(inputFile, line))
	{
		//std::istringstream iss(line);
		data.push_back(line);
	}
	return data;
}

std::vector<std::pair<long long, long long>> findPairs(std::string dataFile)
{
	std::vector<std::pair<long long, long long>> output;
	auto rawLines = readData(dataFile);
	for (size_t i = 0; i < rawLines.size(); i++)
	{
		auto line = rawLines[i];
		for (size_t j = 0; j < line.size(); j++)
		{
			if (line[j]=='#')
			{
				output.push_back(std::pair<long long, long long>(i, j));
			}
		}
	}
	return output;
}

std::vector<long long> findMissingColon(long long minElement, long long maxElement, std::vector<long long> coordinates)
{
	std::vector<long long> missingRows;
	for (long long x = minElement; x <= maxElement; x++)
	{
		if (std::find(coordinates.begin(), coordinates.end(), x) == coordinates.end())
			missingRows.push_back(x);
	}
	return missingRows;
}

class pairOfCoordinates
{
	std::pair<long long, long long> par1;
	std::pair<long long, long long> par2;
public:
	pairOfCoordinates(std::pair<long long, long long> par1in, std::pair<long long, long long> par2in)
	{
		par1 = par1in;
		par2 = par2in;
	}

	long long 
	computeLengthDifference(std::vector<long long> missingRows, std::vector<long long> missingCols, long long multiplier)
	{
		auto maxX = par1.first > par2.first ? par1.first : par2.first;
		auto maxY = par1.second > par2.second ? par1.second : par2.second;
		auto minX = par1.first > par2.first ? par2.first : par1.first;
		auto minY = par1.second > par2.second ? par2.second : par1.second;


		auto orgX = std::abs(par1.first - par2.first);
		auto orgY = std::abs(par1.second - par2.second);
		
		auto xAdd = std::reduce(missingRows.begin(), missingRows.end(), 0, [&minX, &maxX](long long acc, long long a) { return (minX < a) and (a < maxX) ? acc + 1 : acc; });
		auto yAdd = std::reduce(missingCols.begin(), missingCols.end(), 0, [&minY, &maxY](long long acc, long long a) { return (minY < a) and (a < maxY) ? acc + 1: acc; });

		return orgX + orgY + (xAdd + yAdd)* multiplier;
		//return 0;
	}
};

std::vector< pairOfCoordinates>
createALlPair(std::vector<std::pair<long long, long long>> input)
{
	std::vector< pairOfCoordinates> outputVec;
	for (size_t i = 0; i < input.size(); ++i) {
		for (size_t j = i + 1; j < input.size(); ++j) {
			auto tmpCoordpair = pairOfCoordinates(input[i], input[j]);
			outputVec.push_back(tmpCoordpair);
		}
	}
	return outputVec;
}

long long computePar1(std::vector<std::pair<long long, long long>> input, long long multiplier=1)
{
	std::vector<long long> xCoord(input.size());
	std::vector<long long> yCoord(input.size());
	std::transform(input.begin(), input.end(), xCoord.begin(), [](std::pair<long long, long long> ab) { return ab.first; });
	std::transform(input.begin(), input.end(), yCoord.begin(), [](std::pair<long long, long long> ab) { return ab.second; });

	const auto minXElement = std::min_element(xCoord.begin(), xCoord.end());
	const auto maxXElement = std::max_element(xCoord.begin(), xCoord.end());
	const auto minYElement = std::min_element(yCoord.begin(), yCoord.end());
	const auto maxYElement = std::max_element(yCoord.begin(), yCoord.end());
	
	std::vector<long long> missingRows = findMissingColon(*minXElement, *maxXElement, xCoord);
	std::sort(missingRows.begin(), missingRows.end());
	std::vector<long long> missingCols = findMissingColon(*minYElement, *maxYElement, yCoord);
	std::sort(missingCols.begin(), missingCols.end());

	auto allPars = createALlPair(input);
	long long cumsum = 0;
	for (auto pp : allPars)
	{
		auto koasdf = pp.computeLengthDifference(missingRows, missingCols, multiplier);
		cumsum += koasdf;
	}


	return cumsum;
}


int main()
{

	auto part1test = findPairs("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day11test.txt");
	auto part1testResult = computePar1(part1test);

	auto part1real = findPairs("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day11real.txt");
	auto part1realResult = computePar1(part1real);

	std::cout << "Part 1 test: " << part1testResult << ", real: " << part1realResult << std::endl;

	auto kola = 90;

}