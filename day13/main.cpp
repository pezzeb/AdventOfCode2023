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
#include <functional>


#include <vector>
#include <ranges>
#include <cstddef>

class Pattern
{
	std::vector<std::string> rawFormat; // This is not important 
	std::vector<long long> rows;
	std::vector<long long> cols;

	std::vector<std::string> turnAroundThePattern()
	{
		int nRows = rawFormat.size();
		int nCols = rawFormat[0].size();

		std::string tmp = "";
		std::vector<std::string> output;
		for (size_t i = 0; i < nCols; i++)
		{
			tmp.clear();
			for (size_t j = 0; j < nRows; j++)
			{
				tmp += rawFormat[j][i];
			}
			output.push_back(tmp);
		}
		return output;
	}
	long long encodeSingleLine(std::string line)
	{
		long long cumsum = 0;
		long long  ijk = 0;
		for (auto ll : line)
		{
			if (ll == '#')
				cumsum += pow(2, ijk);
			ijk++;
		}
		return cumsum;
	}



public:
	Pattern(std::vector<std::string> rawIn)
	{
		rawFormat = rawIn;

		for (auto line : rawIn)
		{
			rows.push_back(encodeSingleLine(line));
		}
		auto rotate = turnAroundThePattern();
		for (auto line : rotate)
		{
			cols.push_back(encodeSingleLine(line));
		}
	}

	bool isReflection(std::vector<long long> vec)
	{
		int nHalf = vec.size() / 2;
		for (size_t i = 0; i < nHalf; i++)
		{
			if (*(vec.begin() + i) != *(vec.rbegin() + i))
				return false;
		}
		return true;
	}
	int findReflectionInGeneric(std::vector<long long> vec)
	{
		auto placeWhereReflection = 1;
		for (auto it = vec.begin() + 1; it!=vec.end() ; it=it+2)
		{
			std::vector<long long> tmp = std::vector<long long>(vec.begin(), it+1);
			auto isss = isReflection(tmp);
			if (isss)
			{
				auto diffN = it - vec.begin();
				return placeWhereReflection;
			}
			placeWhereReflection++;
		}
		placeWhereReflection = vec.size()-1;
		for (auto it = vec.rbegin() + 1; it != vec.rend(); it = it + 2)
		{
			std::vector<long long> tmp = std::vector<long long>(vec.rbegin(), it+1);
			auto isss = isReflection(tmp);
			if (isss)
			{
				auto diffN = it - vec.rbegin();
				return placeWhereReflection;
			}
			placeWhereReflection--;
		}
		return -1;
	}

	long long computePart1()
	{
		auto tt1 = findReflectionInGeneric(rows);
		if (tt1 > 0)
		{
			return tt1*100;
		}
		auto tt2 = findReflectionInGeneric(cols);
		if (tt2 > 0)
		{
			return tt2;
		}
		auto kolasdf = 90;
	}

};

std::vector<Pattern> readData(std::string dataFile)
	{
		std::ifstream inputFile(dataFile);
		std::string line;

		std::vector<std::string> tmp;

		std::vector<Pattern> output;

		while (std::getline(inputFile, line))
		{
			if (line == "")
			{
				output.push_back(Pattern(tmp));
				tmp.clear();
			}
			else
			{
				tmp.push_back(line);
			}
		}
		if (!tmp.empty())
		{
			output.push_back(Pattern(tmp));
			tmp.clear();
		}

			return output;
	}

long long computePart1InMain(std::vector<Pattern> data)
{
	long long cumsum = 0;
	for (auto pp : data)
	{
		cumsum += pp.computePart1();
	}
	return cumsum;
}

int main()
{

	auto test1 = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day13test.txt");
	auto real1 = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day13real.txt");
	auto test1answer = computePart1InMain(test1);
	auto real1answer = computePart1InMain(real1);

	std::cout << "Part 1 test: " << test1answer << ", real: " << real1answer << std::endl; 


	//Part 1 18570 is too low and 33344 is too high

	auto kola = 90;
}