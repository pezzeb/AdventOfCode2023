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


class Mapper
{
	long long destinationStart;
	long long sourceStart;
	long long length;

public:
	Mapper(long long _destinationStart, long long _sourceStart, long long _length) : destinationStart(_destinationStart), sourceStart(_sourceStart), length(_length) {};
	Mapper(std::string line) 
	{
		std::string value;
		std::istringstream iss(line);
		std::string st1, st2, st3;
		iss >> st1 >> st2 >> st3;
		destinationStart = std::stoll(st1);
		sourceStart = std::stoll(st2);
		length = std::stoll(st3);
	}

	bool isSourceInMapper(long long testSource)
	{
		return (sourceStart <= testSource and testSource <= (sourceStart + length));
	}

	long long computeDestinationFromSource(long long testSource)
	{
		auto tmp = destinationStart - sourceStart + testSource;
		return tmp;
	}
};

class MapSourceToDestion
{
	std::vector<Mapper> mappers;

public:
	MapSourceToDestion(std::vector<std::string> input)
	{
		for (auto line : input)
		{
			mappers.push_back(Mapper(line));
		}
	}

	long long computeDestinationFromSource(long long source)
	{
		for (auto mapper : mappers)
		{
			if (mapper.isSourceInMapper(source))
			{
				auto tmp = mapper.computeDestinationFromSource(source);
				return tmp;
			}
				
		}
		return source;
	}
};


struct parsedInput {
	// Member variables
	std::string seeds;
	std::vector<std::vector<std::string>> mapperParsed;
};

class Almanac
{
	std::vector<MapSourceToDestion> phases;
	std::vector<long long> seeds;

	long long findLocationOfSeed(long long seed)
	{
		for (auto phase : phases)
		{
			seed = phase.computeDestinationFromSource(seed);
		}
		return seed;
	}

	std::vector<std::string>
		splitStringIntoVector(const std::string inputString, const char delimiter = ';')
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

	parsedInput
		readData(std::string dataFile)
	{
		std::ifstream inputFile(dataFile);
		std::vector<std::string> data; 
		std::string line;

		std::vector<std::vector<std::string>> output;
		parsedInput parserStruct;

		std::vector<std::string> tmpVec;
		while (std::getline(inputFile, line)) {
			std::istringstream iss(line);
			std::vector<long long> row;
			
			if (line != "")
			{
				
				std::string searchFor = "seeds:";
				auto it = std::search(line.begin(), line.end(), searchFor.begin(), searchFor.end());

				if (it != line.end())
				{
					parserStruct.seeds = line.substr(it - line.begin()+7);
				}
				else
				{
					searchFor = ":";
					it = std::search(line.begin(), line.end(), searchFor.begin(), searchFor.end());
					if (it== line.end())
						tmpVec.push_back(line);
				}
			}
			else
			{
				if (!tmpVec.empty())
				{
					parserStruct.mapperParsed.push_back(tmpVec);
					tmpVec.clear();
				}
					
			}
		}
		if (!tmpVec.empty())
		{
			parserStruct.mapperParsed.push_back(tmpVec);
		}
		return parserStruct;
	}


public:
	Almanac(std::string input)
	{
		parsedInput parsedStruct = readData(input);
		auto seedasfd = splitStringIntoVector(parsedStruct.seeds, ' ');
		std::transform(seedasfd.begin(), seedasfd.end(), std::back_inserter(seeds), [](std::string str) { return std::stoll(str); });

		for (auto mP : parsedStruct.mapperParsed)
		{
			phases.push_back(MapSourceToDestion(mP));
		}
	}

	long long computePart1()
	{
		std::vector<long long> location;
		for (auto seed : seeds)
		{
			location.push_back(findLocationOfSeed(seed));
		}
		auto minElement = std::min_element(location.begin(), location.end());
		return *minElement;
	}
	long long computePart2()
	{
		//std::vector<long long> seedsToTest;
		
		std::vector<long long> results;
		for (size_t i = 0; i < seeds.size(); i=i+2)
		{
			std::cout << i << std::endl;
			std::vector<long long> location;
			auto seedStart = seeds[i];
			auto nSeed = seeds[i+1];
			for (size_t j = 0; j < nSeed; j++)
			{
				auto tmpSeed = seedStart + j;
				//seedsToTest.push_back(tmpSeed);
				location.push_back(findLocationOfSeed(tmpSeed));
			}
			std::cout << "after for loop" << std::endl;
			auto minElement = std::min_element(location.begin(), location.end());
			std::cout << "after for MIN" << std::endl;
			results.push_back(*minElement);
			std::cout << "after push" << std::endl;
		}
		std::cout << "here" << std::endl;
		auto minElement = std::min_element(results.begin(), results.end());
		std::cout << "after min igen" << std::endl;
		return *minElement;
	}

};

int main()
{
	
	auto almTestPart = Almanac("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day6test.txt");
	auto almRealPart = Almanac("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day6real.txt");

	auto part1test = almTestPart.computePart1();
	auto part1real = almRealPart.computePart1();

	auto part2test = almTestPart.computePart2();
	auto part2real = almRealPart.computePart2();
	std::cout << part2real;
	//auto part2real = 90L;
	std::cout << "Part 1, test: " << part1test << " real: " << part1real << std::endl;
	std::cout << "Part 2, test: " << part2test << " real: " << part2real << std::endl;

}