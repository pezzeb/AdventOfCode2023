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


long long fact(long long n)
{
	return n < 2 ? n : n * fact(n - 1);
}

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
			//std::cout << "here" << std::endl;
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
		std::string shortWild(pattern.begin()+ matching + 1, pattern.end()); //Should might shorten the input in this fashion
		//long firstInteresting = shortWild.find("*");
		//auto firstInteresting = std::max(ii, 1L);
		//std::string tmp(pattern.begin() + firstInteresting +1,pattern.end()); // this can be optmized by always removing an interesting element, I think this is needed
		auto tmp = shortenToInteresting(shortWild);
		shorterPattern = computer(tmp, vec);
	}

	//shortenBoth with a match
	if (pattern[matching + patternToFind.size()] == '#') // this is that the next character after the match CANNot be utsed
		return shorterPattern; // cannot remove this one, but return what I had before
	std::string newPattern(pattern.begin()+matching+patternToFind.size()+1, pattern.end()); // should this be with +1?
	//std::string shortNew(newPattern.begin() + 1, newPattern.end());

	newPattern = shortenToInteresting(newPattern);
	vec.erase(vec.begin());
	auto bothshortend = computer(newPattern, vec);

	return shorterPattern + bothshortend; //Might be a bug that i do not remove the next element when placing an element
}



int main()
{
	long long answer = fact(6);

	//added an extra period at the end AND REMOVED periods in start
	auto kola1 = computer("???.###.", std::vector<int>{1, 1, 3});
	//auto kola21 = computer("?..?...?##..", std::vector<int>{1, 1, 3});
	auto kola2 = computer("??..??...?##..", std::vector<int>{1, 1, 3});
	auto kola3 = computer("?#?#?#?#?#?#?#?..", std::vector<int>{1, 3, 1, 6});
	auto kola4 = computer("????.#...#...", std::vector<int>{ 4, 1, 1});
	//auto kola51 = computer("???.######..#####...", std::vector<int>{1, 6, 5});
	auto kola5 = computer("????.######..#####...", std::vector<int>{1, 6, 5});
	auto kola6 = computer("?###????????..", std::vector<int>{3, 2, 1});

	auto reall1 = computer("#?..???????????????.", std::vector<int>{1, 1, 2, 1, 1, 4});

	auto ending = 90;
}

