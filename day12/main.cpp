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

class ParsedData
{
public:
	std::string pattern;
	std::vector<int> partition;

	ParsedData(std::string a, std::vector<int> b)
	{
		pattern = a;
		partition = b;
	}
};

std::string shortenToInteresting(std::string lineOrg)
{
	std::string line = lineOrg;
	std::replace(line.begin(), line.end(), '#', '*');
	std::replace(line.begin(), line.end(), '?', '*');

	auto firstInteresting = line.find("*");
	if (firstInteresting != std::string::npos)
	{
		std::string dummy = line;
		std::string tmp(lineOrg.begin() + firstInteresting, lineOrg.end()); // this can be optmized by always removing an interesting element, I think this is needed
		//std::string shortNew(dummy.begin() + 2, dummy.end());

		return tmp;
	}
	else
		return "";
}

std::vector<int>
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
	std::vector<int> output;
	for (auto ss : substrings)
	{
		output.push_back(std::stoi(ss));
	}
	return output;
}

std::vector<ParsedData>
readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);
	std::vector<ParsedData> data;
	std::string line;

	std::vector<std::vector<std::string>> output;

	std::vector<std::string> tmpVec;
	while (std::getline(inputFile, line)) {
		std::istringstream iss(line);
		std::string s1, s2;
		iss >> s1 >> s2;

		auto asdf = splitStringIntoVector(s2);
		auto sss1 = shortenToInteresting(s1) + '.';
		auto tmp = ParsedData(sss1, asdf);
		data.push_back(tmp);
	}
	return data;
}

long long fact(long long n)
{
	return n < 2 ? n : n * fact(n - 1);
}



long long computer(std::string pattern, std::vector<int> vec)
{
	if (vec.size() == 0)
	{
		if (pattern.find("#") != std::string::npos)
		{
			return 0;
		}
		else
		{
			std::cout << "here" << std::endl;
			return 1; //There are still some NON-optional spots unfilled.
		}
	}
	else if (pattern.empty())
		return 0;


	//can be a combination of ? and # so it does not suffice to search them independently
	std::string wildone = pattern;
	std::replace(wildone.begin(), wildone.end(), '#', '*');
	std::replace(wildone.begin(), wildone.end(), '?', '*');

	auto upcomingNumber = vec[0];
	auto patternToFind = std::string(upcomingNumber, '*');

	std::size_t matching = wildone.find(patternToFind);
	if (matching == std::string::npos)
		return 0;

	// shorten pattern and resend with vector + shorten both 
	// Pattern shorer
	long long shorterPattern;
	if (pattern[0] == '#')
		shorterPattern = 0; //This has to be matched
	else
	{
		//std::string shortWild(pattern.begin() + matching + 1, pattern.end()); //Should might shorten the input in this fashion
		//auto minshorting = 
		std::string shortWild(pattern.begin() + 1, pattern.end()); //Should might shorten the input in this fashion
		//long firstInteresting = shortWild.find("*");
		//auto firstInteresting = std::max(ii, 1L);
		//std::string tmp(pattern.begin() + firstInteresting +1,pattern.end()); // this can be optmized by always removing an interesting element, I think this is needed
		auto tmp = shortenToInteresting(shortWild);
		shorterPattern = computer(tmp, vec);
	}

	//shortenBoth with a match
	//if (pattern[matching + patternToFind.size()] == '#') // this is that the next character after the match CANNot be utsed
	if (matching > 0) // this is that the next character after the match CANNot be utsed
		return shorterPattern; // cannot remove this one, but return what I had before
	std::string newPatternTmp(pattern.begin()+patternToFind.size(), pattern.end()); // should this be with +1 yes since it cannot be together?
	if (newPatternTmp[0]=='#')
		return shorterPattern;
	
	std::string newPattern(newPatternTmp.begin() + 1, newPatternTmp.end());

	newPattern = shortenToInteresting(newPattern);
	vec.erase(vec.begin());

	auto bothshortend = computer(newPattern, vec);
	//auto bothshortend = 0;

	return shorterPattern + bothshortend; //Might be a bug that i do not remove the next element when placing an element
}

long long part1Computer(std::vector<ParsedData> data)
{
	long long cumsum = 0;
	for (auto pp : data)
	{
		auto delta = computer(pp.pattern, pp.partition);
		cumsum += delta;
	}
	return cumsum;
}

int main()
{
	//long long answer = fact(6);

	////added an extra period at the end AND REMOVED periods in start
	//auto kola1 = computer("???.###.", std::vector<int>{1, 1, 3});
	////auto kola21 = computer("?..?...?##..", std::vector<int>{1, 1, 3});
	//auto kola2 = computer("??..??...?##..", std::vector<int>{1, 1, 3});
	auto kola31 = computer("#?#?#?#?..", std::vector<int>{1, 6});
	auto kola3 = computer("?#?#?#?#?#?#?#?..", std::vector<int>{1, 3, 1, 6});
	//auto kola4 = computer("????.#...#...", std::vector<int>{ 4, 1, 1});
	////auto kola51 = computer("???.######..#####...", std::vector<int>{1, 6, 5});
	/*auto kola5 = computer("????.######..#####...", std::vector<int>{1, 6, 5});
	auto kola51 = computer("######..#####...", std::vector<int>{ 6, 5});*/
	//auto kola521 = computer("???.######..", std::vector<int>{6});
	//auto kola52 = computer("???.######..", std::vector<int>{ 1, 6});
	/*auto kola6 = computer("?###????????..", std::vector<int>{3, 2, 1});
	auto kola61 = computer("???????..", std::vector<int>{2, 1});

	auto reall1 = computer("???.#??.", std::vector<int>{1, 2});*/


	auto testData = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day12test.txt");
	auto testPart1 = part1Computer(testData);

	auto realData = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day12real.txt");
	auto realPart1 = part1Computer(realData);

	std::cout << "Part 1 test: " << testPart1 << "; real: " << realPart1 << std::endl;

	//7213 is too high

	auto ending = 90;
}

