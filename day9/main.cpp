#include <iostream>
#include <vector>
#include <string>
#include <set>
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

class lineClass
{
	std::vector<long long> sequence;

	std::vector<std::vector<long long>> findAllvectors(std::vector<long long> sequence)
	{
		std::vector<std::vector<long long>> output;
		std::set<long long> setOfDifference(sequence.begin(), sequence.end());

		output.push_back(sequence);
		while (setOfDifference.size()>1 or *setOfDifference.begin() != 0)
		{
			std::adjacent_difference(sequence.begin(), sequence.end(), sequence.begin());
			sequence.erase(sequence.begin()); //adjcent keeps first element untouched...
			output.push_back(sequence);
			setOfDifference = std::set<long long>(sequence.begin(), sequence.end());
		}
		return output;
	};

public:
	lineClass(std::string line)
	{
		auto tmp = splitStringIntoVector(line, ' ');
		std::transform(tmp.begin(), tmp.end(), std::back_inserter(sequence), [](std::string str) { return std::stoll(str); });
	}

	long long extrapolate()
	{
		auto vectors = findAllvectors(sequence);
		long long prevDiff = 0;
		for (auto it = vectors.rbegin(); it != vectors.rend(); ++it)
		{
			
			if (it == vectors.rbegin())
				prevDiff = 0;
			else
			{
				prevDiff = (it - 1)->back();
			}
			it->push_back(it->back() + prevDiff);
		}
		
		return vectors[0].back();
	}

};

std::vector<lineClass> readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);
	std::string line;
	std::vector<lineClass> data;

	while (std::getline(inputFile, line))
	{
		std::istringstream iss(line);
		data.push_back(lineClass(line));
	}

	return data;
}

class part1Computer
{
	std::vector<lineClass> lines;
public:
	part1Computer(std::string path)
	{
		lines = readData(path);
	}

	long long computePart1()
	{
		long long cumsum = 0;
		for (auto lineN : lines)
		{
			auto tmp = lineN.extrapolate();
			cumsum += tmp;
		}
		return cumsum;
	}
};

int main()
{
	auto ownTest = lineClass("0 3 6 9 12 15");
	auto ownTestReult = ownTest.extrapolate();

	auto part1test = part1Computer("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day9test.txt");
	auto part1testResult = part1test.computePart1();

	auto part1real = part1Computer("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day9real.txt");
	auto part1realResult = part1real.computePart1();

	std::cout << "Part1: test: " << part1testResult << "; real: " << part1realResult << std::endl;

}