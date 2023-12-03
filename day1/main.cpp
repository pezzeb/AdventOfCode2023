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

#include <vector>
#include <ranges>
#include <cstddef>

std::vector<std::string>
readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);
	/*if (!inputFile) {
		std::cerr << "Error opening the file." << std::endl;
		return 1;
	}*/

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


long firstPart(std::string path)
{
	auto loadedData = readData(path);

	auto is_digit = [](char c) { return std::isdigit(static_cast<unsigned char>(c)); };
	long cumsum = 0;
	for (const auto& str : loadedData)
	{
		auto itBegin = std::find_if(str.begin(), str.end(), is_digit);
		auto itEnd = std::find_if(str.rbegin(), str.rend(), is_digit);

		int tenInt = (*itBegin) - '0';
		int oneInt = (*itEnd) - '0';
		auto newNumber = tenInt * 10 + oneInt;
		cumsum += newNumber;
	}
	return cumsum;
}


long findnumber(std::string str, std::vector<std::string> stringNumbers)
{

	std::size_t maxSize = SIZE_MAX;
	auto is_digit = [](char c) { return std::isdigit(static_cast<unsigned char>(c)); };

	std::vector<std::size_t> positionOfLetters;

	int ijk = 1;
	for (const std::string& letter : stringNumbers) { positionOfLetters.push_back(str.find(letter)); }

	auto it = std::min_element(positionOfLetters.begin(), positionOfLetters.end());
	auto letterThatFirstAppeard = it - positionOfLetters.begin(); //letterPos is the first found letter - 1 

	auto it123 = std::find_if(str.begin(), str.end(), is_digit);


	if ((*it) < (it123 - str.begin())) //
	{
		return letterThatFirstAppeard + 1L;
	}
	else
	{
		return (*it123) - '0';;
	}

}

long findNumberForward(std::string str)
{
	std::vector<std::string> stringNumbers = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
	return findnumber(str, stringNumbers);
}
long findNumberBackwards(std::string str)
{
	std::vector<std::string> stringNumbers = { "eno", "owt", "eerht", "ruof", "evif", "xis", "neves", "thgie", "enin" };
	std::string reversedString(str.rbegin(), str.rend());
	return findnumber(reversedString, stringNumbers);
}


long secondPart(std::string path)
{
	auto loadedData = readData(path);
	long cumsum = 0;

	for (const auto& data : loadedData)
	{
		auto tenNumber = findNumberForward(data) * 10;
		auto oneNumber = findNumberBackwards(data);
		auto dummy = tenNumber + oneNumber;
		cumsum += dummy;
	}
	return cumsum;
}

int main()
{
	// Part 1 
	std::vector<std::string> stringVector = { "1abc2", "pqr3stu8vwx", "a1b2c3d4e5f", "treb7uchet"};
	auto part1test = firstPart("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day1test.txt");
	auto part1real = firstPart("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day1real.txt");
	std::cout << "Part 1, test: " << part1test << "; and real: " << part1real;

	std::cout << std::endl;

	//Part 2 
	auto part2test = secondPart("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day1test2.txt");
	auto part2real = secondPart("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day1real.txt");
	std::cout << "Part 2, test: " << part2test << "; and real: " << part2real;
		
	
}