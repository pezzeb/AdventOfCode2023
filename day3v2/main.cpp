// day1WithCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <fstream>
#include <sstream>
#include <tuple>
#include <set>


#include <vector>
#include <ranges>
#include <cstddef>

template <class I1, class I2>
bool have_common_element(I1 first1, I1 last1, I2 first2, I2 last2) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2)
			++first1;
		else if (*first2 < *first1)
			++first2;
		else
			return true;
	}
	return false;
}
template <class I1, class I2>
long number_common_element(I1 first1, I1 last1, I2 first2, I2 last2) {
	long matches = 0;
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2)
			++first1;
		else if (*first2 < *first1)
			++first2;
		else
			matches++;
	}
	return matches;
}


class Number
{

public:

	std::set<std::tuple<long, long>> pointsInNumber;
	long realNumber;

	Number(std::set<std::tuple<long, long>> points, long realNumberIn)
	{
		pointsInNumber = points;
		realNumber = realNumberIn;
	} 

	bool
	isPointInNumber(std::tuple<long, long> tpl)
	{
		auto it = pointsInNumber.find(tpl);

		if (it != pointsInNumber.end()) {
			return true;
		}
		else {
			return false;
		}
	}
	long getNumber() { return realNumber; }
};


class PlayingField
{
	long xSize, ySize;
	std::vector<std::string> field;
	std::set<std::tuple<long, long>> possiblePoints;
	std::set<std::tuple<long, long>> possibleGears;
	std::vector<Number> numbers;


public:
	PlayingField(std::vector<std::string> fieldInput)
	{
		field = fieldInput;
		ySize = field.size();
		xSize = field[0].size();
		numbers = findAllNumbers();
		possiblePoints = findAllPossiblePoints();
		possibleGears = findAllPossibleGears();
	}

	
	long part1Computer()
	{
		long cumsum = 0L;
		std::vector<long> outputVec;

		for (auto numb : numbers)
		{ 
			for (auto pnt : possiblePoints)
			{
				if (have_common_element(numb.pointsInNumber.begin(), numb.pointsInNumber.end(), possiblePoints.begin(), possiblePoints.end()))
				{
					cumsum += numb.getNumber();
					outputVec.push_back(numb.getNumber());
					break;
				}
			}
		}
		return cumsum;
	}
	long part2Computer()
	{
		long cumsum = 0L;
		std::vector<long> outputVec;

		for (auto gearPnt : possibleGears)
		{
			long matches = 0;
			std::vector<long> matchedNumber {};
			for (auto numb : numbers)
			{
				auto allPossibleGearNeigbours = createAllPossiblePositionFromSinglePoint(gearPnt);
				if (have_common_element(numb.pointsInNumber.begin(), numb.pointsInNumber.end(), allPossibleGearNeigbours.begin(), allPossibleGearNeigbours.end()) and matches < 3)
				{
					matches += 1;
					matchedNumber.push_back(numb.getNumber());
				}
			}
			if (matches == 2)
			{
				auto prod = matchedNumber[0] * matchedNumber[1];
				outputVec.push_back(prod);
				cumsum += prod;
			}
		}
		
		return cumsum;
	}



private:
	std::vector<Number> findAllNumbers()
	{
		std::vector<Number> numbers;

		for (size_t x = 0; x < xSize; x++)
		{
			auto tmpVector = parseLine(x);
			if (!tmpVector.empty())
				numbers.insert(numbers.end(), tmpVector.begin(), tmpVector.end());
		}
		if (numbers.empty())
			return std::vector<Number>();
		else
			return numbers;
	}

	std::set<std::tuple<long, long>>
	findAllPossiblePoints()
	{
		std::set<std::tuple<long, long>> cummulativeSet;
		auto is_digit = [](char c) { return std::isdigit(static_cast<unsigned char>(c)); };
		for (size_t x = 0; x < xSize; x++)
		{
			for (size_t y = 0; y < ySize; y++)
			{
				auto tmp = field[x][y];
				if (!is_digit(tmp) and tmp != '.')
				{
					auto singleSet = createAllPossiblePositionFromSinglePoint(std::tuple<long, long>(x, y));
					cummulativeSet.insert(singleSet.begin(), singleSet.end());
				}
			}
		}
		return cummulativeSet;
	}
	std::set<std::tuple<long, long>>
	findAllPossibleGears()
	{
		std::set<std::tuple<long, long>> cummulativeSet;
		auto is_digit = [](char c) { return std::isdigit(static_cast<unsigned char>(c)); };
		for (size_t x = 0; x < xSize; x++)
		{
			for (size_t y = 0; y < ySize; y++)
			{
				auto tmp = field[x][y];
				if (tmp == '*')
				{
					auto singleSet = std::tuple<long, long>(x, y);
					cummulativeSet.insert(singleSet);
				}
			}
		}
		return cummulativeSet;
	}

	std::set<std::tuple<long, long>>
	createAllPossiblePositionFromSinglePoint(std::tuple<long, long> pnt)
	{
		std::set<std::tuple<long, long>> mySet;
		for (int dx = -1; dx < 2; dx++)
		{
			for (int dy = -1; dy < 2; dy++)
			{
				long newx = std::get<0>(pnt) + dx;
				long newy = std::get<1>(pnt) + dy;
				if (newx > (-1) and newy > (-1) and newx < (xSize + 1) and newy < (ySize + 1))
				{
					mySet.insert(std::tuple<long, long>(newx, newy));
				}
			}
		}
		return mySet;
	}

	std::vector<Number>
		parseLine(long lineNumber)
	{
		auto is_digit = [](char c) { return std::isdigit(static_cast<unsigned char>(c)); };
		std::vector<Number> returnVec{};
		std::set < std::tuple < long, long>> tempSet;
		std::string tmpstring = "";
		for (size_t y = 0; y < ySize; y++)
		{
			auto tmpTuple = std::tuple<long, long>(lineNumber, y);
			auto it = possiblePoints.find(tmpTuple);
			if (is_digit(field[lineNumber][y]))
			{
				//Point found (which is a digit)	
				tmpstring += field[lineNumber][y];
				tempSet.insert(std::tuple<long, long>(lineNumber, y));
			}
			else if (tmpstring != "")
			{
				Number tmpp = Number(tempSet, std::stoi(tmpstring));
				returnVec.push_back(tmpp);
				tempSet.clear();
				tmpstring = "";
			}
			else
			{
				//Do nothing
			}
		}

		if (tmpstring != "") 
		{ 
			Number tmpp = Number(tempSet, std::stoi(tmpstring));
			returnVec.push_back(tmpp);
		}

		if (returnVec.empty())
			return {};
		else
			return returnVec;
	}



};

std::vector<std::string>
readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);


	std::vector<std::string> data; // Vector to store rows

	std::string line;
	while (std::getline(inputFile, line)) {
		std::istringstream iss(line);
		std::vector<long> row;

		std::string value;
		while (iss >> value) {
			data.push_back(value);
		}

		//data.push_back(row); // Add the row to the vector of rows
	}



	return data;
}


int main()
{
	auto testData = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day3test.txt");
	auto realData = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day3realArt.txt"); 

	auto testFieldPart1 = PlayingField(testData);
	auto answerTestPart1 = testFieldPart1.part1Computer();
	auto answerTestPart2 = testFieldPart1.part2Computer();
	std::cout << "Test Part1: " << answerTestPart1 << ", Test Part2: " << answerTestPart2 << std::endl;

	auto realFieldPart1 = PlayingField(realData);
	auto answerRealPart1 = realFieldPart1.part1Computer();
	auto answerRealPart2 = realFieldPart1.part2Computer();
	std::cout << "Real Part1: " << answerRealPart1 << ", Real Part2: " << answerRealPart2 << std::endl;
}