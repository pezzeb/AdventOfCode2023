#include <iostream>
#include <vector>
#include <map>
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

std::string
readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);
	std::vector<std::string> data;
	std::string line;

	std::vector<std::vector<std::string>> output;

	std::vector<std::string> tmpVec;
	while (std::getline(inputFile, line)) {

		return line;
	}
}



static class Hasher
{
public:
	long long hashingAlgorithm(long long currentValue, long long increase)
	{
		currentValue += increase;
		currentValue *= 17;
		currentValue = currentValue % 256;
		return currentValue;
	}

	long long hashAString(std::string line)
	{
		long long summan = std::accumulate(line.begin(), line.end(), 0, [this](long long acc, long long val) {return this->hashingAlgorithm(acc, val); });
		return summan;
	}
};

std::vector<long long> computeSingleLine(std::string line)
{
	std::vector<long long> vec;
	std::transform(line.begin(), line.end(), std::back_inserter(vec), [](char cc) { return int(cc); });
	return vec;
}

long long computeIncreases(std::vector<std::string> vec)
{
	auto hasher = Hasher();
	long long cumsum = 0;
	for (auto str : vec)
	{
		auto kola = computeSingleLine(str);
		auto summan = std::accumulate(kola.begin(), kola.end(), 0, [&hasher](long long acc, long long val) {return hasher.hashingAlgorithm(acc, val); }); //
		cumsum += summan;
	}
	return cumsum;
}

class Lens
{

private:
	char typeOfLens;
	long boxNumber;
	long focalLength;
	std::string label;

public:
	Lens(char c, long bN, long fL, std::string str)
	{
		typeOfLens = c;
		boxNumber = bN;
		focalLength = fL;
		label = str;

	}
	char getTypeOfLens()
	{
		return typeOfLens;
	}
	long getBoxNumber()
	{
		return boxNumber;
	}
	long getFocalLength()
	{
		return focalLength;
	}
	std::string getLabel()
	{
		return label;
	}

};


Lens parseSingleEntry(std::string line)
{
	long boxNumber = 0;
	long focalLenght = 0;
	auto hasher = Hasher();

	auto itEquality = std::find(line.begin(), line.end(), '=');
	if (itEquality != line.end())
	{
		focalLenght = std::stol(std::string(itEquality + 1, line.end()));
		auto label = std::string(line.begin(), itEquality);
		boxNumber = hasher.hashAString(label);
		return Lens('=', boxNumber, focalLenght, label);
	}

	auto itDash = std::find(line.begin(), line.end(), '-');
	if (itDash != line.end())
	{
		focalLenght = -1;
		auto label = std::string(line.begin(), itEquality-1);
		boxNumber = hasher.hashAString(label);
		return Lens('-', boxNumber, focalLenght, label);
	}
}

long long computeSingleBox(std::vector<Lens> box)
{
	//std::sort(box.begin(), box.end());
	long long cumsum = 0;
	for (size_t i = 0; i < box.size(); i++)
	{
		cumsum += (i + 1) * box[i].getFocalLength();
	}
	return cumsum;
}

long long computePart2(std::vector<std::string> vec)
{
	auto hasher = Hasher();
	long long cumsum = 0;
	std::vector<Lens> commands;
	std::vector<std::vector<Lens>> boxes(256);

	for (auto str : vec)
	{
		auto tmpCommand = parseSingleEntry(str);

		if(tmpCommand.getTypeOfLens() == '=')
		{
			//check if same label already in the box
			auto lensesInBox = boxes[tmpCommand.getBoxNumber()];
			auto duplicateLens = std::find_if(lensesInBox.begin(), lensesInBox.end(), [&tmpCommand](Lens ll) {return ll.getLabel() == tmpCommand.getLabel(); });
			if (duplicateLens != lensesInBox.end())
			{
				//There was already a lens there
				auto pos = duplicateLens - lensesInBox.begin();
				lensesInBox.erase(duplicateLens);
				lensesInBox.insert(lensesInBox.begin() + pos, tmpCommand);
			}
			else
			{
				// no such lens prior
				lensesInBox.push_back(tmpCommand);
			}
			
			boxes[tmpCommand.getBoxNumber()] = lensesInBox;
		}
		if (tmpCommand.getTypeOfLens() == '-')
		{
			//check if same label already in the box
			auto lensesInBox = boxes[tmpCommand.getBoxNumber()];
			auto duplicaeLens = std::find_if(lensesInBox.begin(), lensesInBox.end(), [&tmpCommand](Lens ll) {return ll.getLabel() == tmpCommand.getLabel(); });
			if (duplicaeLens != lensesInBox.end())
			{
				//There was already a lens there
				lensesInBox.erase(duplicaeLens);
				boxes[tmpCommand.getBoxNumber()] = lensesInBox;
			}
		}
		commands.push_back(tmpCommand);
	}
	
	long long bb = 1;
	for (auto box : boxes)
	{
		cumsum += bb * computeSingleBox(box);
		bb++;
	}

	return cumsum;
}

int main()
{
	auto testData = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day15test.txt");
	auto testVec = splitStringIntoVector(testData);
	auto testPart1 = computeIncreases(testVec);
	auto testPart2 = computePart2(testVec);

	auto realData = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day15real.txt");
	auto realVec = splitStringIntoVector(realData);
	auto realPart1 = computeIncreases(realVec);
	auto realPart2 = computePart2(realVec);

	std::cout << "Part 1 test: " << testPart1 << "; real: " << realPart1 << std::endl;

	std::cout << "Part 2 test: " << testPart2 << "; real: " << realPart2 << std::endl;

	auto ending = 90;
}