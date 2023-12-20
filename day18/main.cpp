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

class Digger
{
private:
	std::string direction;
	long steps;
public:
	Digger(std::string c, long l)
	{
		direction = c;
		steps = l;
	}
	std::string getDirection()
	{
		return direction;
	}
	long getSteps()
	{
		return steps;
	}
};

std::vector<Digger>
readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);
	std::vector<Digger> data;
	std::string line;

	std::vector<std::vector<std::string>> output;

	std::vector<std::string> tmpVec;
	while (std::getline(inputFile, line)) 
	{
		std::istringstream iss(line);

		std::string s1, s2, s3;
		iss >> s1 >> s2 >> s3;

		auto tmpD = Digger(s1, std::stol(s2));
		data.push_back(tmpD);
	}
	return data;
}

class Stepper
{
public:
	long x;
	long y;
	Stepper(std::string c)
	{
		x = 0;
		y = 0;

		if (c == "R")
			y = 1;
		else if (c == "L")
			y = -1;
		else if (c == "U")
			x = -1;
		else if (c == "D")
			x = 1;
	}
};

long long computePart1(std::vector<Digger> vecDigger)
{
	std::vector<std::tuple<double, double>> coordinates;

	double x0 = 0;
	double y0 = 0;

	coordinates.push_back(std::tuple<double, double>(x0, y0));

	for (auto digg : vecDigger)
	{
		auto step = Stepper(digg.getDirection());

		for (size_t i = 0; i < digg.getSteps(); i++)
		{
			x0 += step.x;
			y0 += step.y;
			coordinates.push_back(std::tuple<double, double>(x0, y0));
		}
		
	}
	
	double area = 0;
	for (size_t i = 0; i < coordinates.size()-1; i++) // last point is duplicated
	{
		auto crd0 = coordinates[i];
		auto crd1 = coordinates[i+1];
		auto xDiff = std::get<0>(crd0) - std::get<0>(crd1);
		auto yDiff = std::get<1>(crd0) + std::get<1>(crd1);
		area += xDiff * yDiff;
	}
	area = area / 2;

	area = area < 0 ? (- area) : area;

	double interiorPoints = area - (coordinates.size() - 1) / 2 + 1;

	return interiorPoints + coordinates.size() - 1;
}


int main()
{


	auto ownTestData = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day18test0.txt");
	auto ownTestAnswer = computePart1(ownTestData);

	auto testData = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day18test.txt");
	auto testAnswer = computePart1(testData);

	auto realData = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day18real.txt");
	auto realAnswer = computePart1(realData);

	std::cout << "Part 1, test own: " << ownTestAnswer << "; test 1: " << testAnswer << "; real: " << realAnswer << std::endl;

	auto ending = 90;
}