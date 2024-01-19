#include <iostream>
#include <vector>
#include <map>
#include <set>
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

enum finalState { ACCEPT, REJECT, CONTINUE };

class ExpressionReturn
{
	finalState typeOfState;
	std::string nextState;
	bool valOfFunc;
public:
	ExpressionReturn(finalState typeOfState, std::string nextState) : typeOfState(typeOfState), nextState(nextState) {};
	ExpressionReturn(finalState typeOfState) : typeOfState(typeOfState), nextState("") {};
	ExpressionReturn() = default;

	void setBool(bool val) { valOfFunc = val; }
	bool getBool() { return valOfFunc; }
	finalState getTypeOfState() { return typeOfState; }
	std::string getNextState() { return nextState; }
};

class Expression
{
	std::function<bool(long long)> funcN;
	std::string typeOfOperator;
	ExpressionReturn potentialReturn;
	std::string variableOfExpression;
	long long comparisionValue;

public:

	Expression(std::string stringToBeParsed)
	{
		auto it = std::find(stringToBeParsed.begin(), stringToBeParsed.end(), ':');

		if (it == stringToBeParsed.end())//Not a real expression
		{
			if (stringToBeParsed == "A")
			{
				potentialReturn = ExpressionReturn(ACCEPT);
			}
			else if (stringToBeParsed == "R")
			{
				potentialReturn = ExpressionReturn(REJECT);
			}
			else
			{
				std::string nextState = stringToBeParsed;
				potentialReturn = ExpressionReturn(CONTINUE, nextState);
			}
		}
		else
		{
			std::string nextState = std::string(it + 1, stringToBeParsed.end());
			if (nextState == "A")
				potentialReturn = ExpressionReturn(ACCEPT);
			else if (nextState == "R")
				potentialReturn = ExpressionReturn(REJECT);
			else
				potentialReturn = ExpressionReturn(CONTINUE, nextState);

			auto itOperator = std::find_if(stringToBeParsed.begin(), stringToBeParsed.end(), [](char c) {return c == '<' or c == '>'; });

			typeOfOperator = *itOperator;

			variableOfExpression = std::string(stringToBeParsed.begin(), itOperator);
			auto rightSide = std::stoll(std::string(itOperator+1, it));
			comparisionValue = rightSide;
			if (*itOperator == '>')
			{
				funcN = [rightSide](long long y) {return y > rightSide; };
			}
			else
			{
				funcN = [rightSide](long long y) {return y < rightSide; };
			}
		}
	}

	ExpressionReturn computeExpression(long long val)
	{
		auto boolen = funcN(val);
		potentialReturn.setBool(boolen);
		return potentialReturn;
	};

	ExpressionReturn computeEmptyExpression()
	{
		potentialReturn.setBool(true);
		return potentialReturn;
	};

	std::string getVariableOfExpression()
	{
		return variableOfExpression;
	}
	ExpressionReturn getPotentialReturn()
	{
		return potentialReturn;
	}

	std::string getTypeOfOperator()
	{
		return typeOfOperator;
	}

	long long getComparisionValue()
	{
		return comparisionValue;
	}

};

class Rating
{
	std::map<std::string, long long> inputMap;
	
	
	const long long parseSingleRatingVariableValue(std::string str)
	{
		auto iteq = std::find(str.begin(), str.end(), '=');
		auto tmpStr = std::string(iteq+1, str.end());
		return stoll(tmpStr);
	}
	const std::string parseSingleRatingVariableName(std::string str)
	{
		auto iteq = std::find(str.begin(), str.end(), '=');
		auto tmpStr = std::string(str.begin(), iteq);
		return tmpStr;
	}

public:

	Rating() = default;
	Rating(long long x, long long m, long long a, long long s)
	{
		inputMap = {
			{ "x", x },
			{ "m", m },
			{ "a", a },
			{ "s", s }
		};
	};

	Rating(std::string stringToParse)
	{
		auto removeBrackets = std::string(stringToParse.begin() + 1, stringToParse.end() - 1);
		auto vec = splitStringIntoVector(removeBrackets);
		for (auto v : vec)
		{
			inputMap.insert(std::pair < std::string, long long>(parseSingleRatingVariableName(v), parseSingleRatingVariableValue(v)));
		}
	};

	const long long getValueForVariable(const std::string y)
	{
		return inputMap[y];
	}
	const long long sumUpRating()
	{
		return inputMap["x"] + inputMap["m"] + inputMap["a"] + inputMap["s"];
	}
};

class Workflow
{
	std::string nameOfFlow;
	std::vector<Expression> expressionsInFlow;
public:
	
	Workflow(std::string stringToParse)
	{
		auto it = std::find(stringToParse.begin(), stringToParse.end(), '{');
		nameOfFlow = std::string(stringToParse.begin(), it);
		auto vvv = splitStringIntoVector(std::string(it + 1, stringToParse.end() - 1));
		for (auto v : vvv)
		{
			expressionsInFlow.push_back(Expression(v));
		}
	}

	Workflow() = default;

	const ExpressionReturn computeRating(Rating rating)
	{
		for (auto ep : expressionsInFlow)
		{
			auto ii = ep.getVariableOfExpression();
			if (ii == "")
			{
				return ep.computeEmptyExpression();
			}
			else
			{
				auto kol = rating.getValueForVariable(ii);
				auto jjj = ep.computeExpression(kol);
				if (jjj.getBool())
				{
					//we can move on
					return jjj;
				}
			}
		}
	}

	const std::string getNameOfFlow()
	{
		return nameOfFlow;
	}

	const std::vector<Expression> getExpressionsInFlow()
	{
		return expressionsInFlow;
	}

};



std::vector<std::string>
readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);
	std::vector < std::string> data;
	std::string line;

	int i = 0;
	while (std::getline(inputFile, line))
	{
		data.emplace_back(line);
	}
	return data;
}

long long computePartOneSingle(std::vector<Workflow>& workflows, Rating& rating)
{
	long long cumsum = 0;
	auto itStart = std::find_if(workflows.begin(), workflows.end(), [](Workflow& wf) {return wf.getNameOfFlow() == "in"; });
	while (true)
	{
		auto asdf1 = itStart->computeRating(rating);
		if (asdf1.getTypeOfState()==ACCEPT)
		{
			return rating.sumUpRating();
		}
		else if (asdf1.getTypeOfState() == REJECT)
		{
			return 0;
		}
		else
		{
			itStart = std::find_if(workflows.begin(), workflows.end(), [&asdf1](Workflow& wf) {return wf.getNameOfFlow() == asdf1.getNextState(); });
		}
	}
}

long long computePartOne(std::vector<Workflow>& workflows, std::vector<Rating>& ratings)
{
	long long cumsum = 0;
	for (auto rr : ratings)
	{
		cumsum += computePartOneSingle(workflows, rr);
	}
	return cumsum;
}

long long computePart1Complete(std::string ratingPath, std::string workflowPath)
{
	auto ratingsTestVec = readData(ratingPath);
	auto workflowsTestVec = readData(workflowPath);
	auto ratings = std::vector<Rating>(ratingsTestVec.size());
	std::transform(ratingsTestVec.begin(), ratingsTestVec.end(), ratings.begin(), [](std::string str) { return Rating(str); });
	auto workflows = std::vector<Workflow>(workflowsTestVec.size());
	std::transform(workflowsTestVec.begin(), workflowsTestVec.end(), workflows.begin(), [](std::string str) { return Workflow(str); });
	return computePartOne(workflows, ratings);
}

class rangePossible
{

public:
	long long xlower, xupper;
	long long mlower, mupper;
	long long alower, aupper;
	long long slower, supper;


	rangePossible()
	{
		xlower = 1;
		xupper = 4000;
		mlower = 1;
		mupper = 4000;
		alower = 1;
		aupper = 4000;
		slower = 1;
		supper = 4000;
	}

	long long computePossibilities()
	{
		if (xupper < xlower)
			return 0;
		if (mupper < mlower)
			return 0;
		if (aupper < alower)
			return 0;
		if (supper < slower)
			return 0;

		auto tmpMult = (xupper - xlower + 1) * (mupper - mlower + 1) * (aupper - alower + 1) * (supper - slower + 1);
		return tmpMult;
	}

	void changeLowerLimit(std::string str, long long newVal)
	{
		if (str == "x")
			xlower = newVal+1;
		else if (str == "m")
			mlower = newVal + 1;
		else if (str == "a")
			alower = newVal + 1;
		else if (str == "s")
			slower = newVal + 1;
	}
	void changeUpperLimit(std::string str, long long newVal)
	{
		if (str == "x")
			xupper = newVal-1;
		else if (str == "m")
			mupper = newVal - 1;
		else if (str == "a")
			aupper = newVal - 1;
		else if (str == "s")
			supper = newVal - 1;
	}
};

//long long 
std::vector<rangePossible> computePartTwo(std::vector<Workflow>& workflows, std::string nextState, rangePossible rangePosIn)
{
	//long long cumsum = 0;
	std::vector<rangePossible> aggRangePossible;

	auto wf = std::find_if(workflows.begin(), workflows.end(), [&nextState](Workflow wfN) {return wfN.getNameOfFlow() == nextState; });

	for (auto jj : wf->getExpressionsInFlow())
	{
		auto rangePos = rangePosIn;

		//I have to split the path and not ta

		if (jj.getTypeOfOperator() == "<")
		{
			rangePos.changeUpperLimit(jj.getVariableOfExpression(), jj.getComparisionValue());
		}
		else if (jj.getTypeOfOperator() == ">")
		{
			rangePos.changeLowerLimit(jj.getVariableOfExpression(), jj.getComparisionValue());
		}

		if (jj.getPotentialReturn().getTypeOfState() == ACCEPT)
		{
			auto tmp = rangePos.computePossibilities();
			aggRangePossible.push_back(rangePos);
			//cumsum += tmp;
		}
		else if (jj.getPotentialReturn().getTypeOfState() == CONTINUE)
		{
			auto tmpRanges = computePartTwo(workflows, jj.getPotentialReturn().getNextState(), rangePos);
			aggRangePossible.insert(aggRangePossible.end(), tmpRanges.begin(), tmpRanges.end());
			//aggRangePossible.push_back(rangePos);
			//cumsum += tmp;
		}
		else
		{
			//REJECT
		}
	}

	return aggRangePossible;
}

long long computeVolumeOfOverlapping(rangePossible r1, rangePossible r2)
{
	// A are(x, y, z) and (x',y', z') (x' > x, y'>y,z' > z)
	// B are(a, b, c) and (a',b', c') (a' > a, b'>b,c' > c)
	//max(min(a',x') - max(a, x), 0)   * 
	//max(min(b',y') - max(b, y), 0)   * 
	//max(min(c',z') - max(c, z), 0)

	auto xlen = std::max(std::min(r1.xupper, r2.xupper) - std::max(r1.xlower, r2.xlower), 0LL);
	auto mlen = std::max(std::min(r1.mupper, r2.mupper) - std::max(r1.mlower, r2.mlower), 0LL);
	auto alen = std::max(std::min(r1.aupper, r2.aupper) - std::max(r1.alower, r2.alower), 0LL);
	auto slen = std::max(std::min(r1.supper, r2.supper) - std::max(r1.slower, r2.slower), 0LL);
	
	auto volumeIntersection = xlen * mlen * alen * slen;

	return volumeIntersection;
}

long long computePart2Complete(std::string ratingPath, std::string workflowPath)
{
	//auto ratingsTestVec = readData(ratingPath);
	auto workflowsTestVec = readData(workflowPath);
	//auto ratings = std::vector<Rating>(ratingsTestVec.size());
	//std::transform(ratingsTestVec.begin(), ratingsTestVec.end(), ratings.begin(), [](std::string str) { return Rating(str); });
	auto workflows = std::vector<Workflow>(workflowsTestVec.size());
	std::transform(workflowsTestVec.begin(), workflowsTestVec.end(), workflows.begin(), [](std::string str) { return Workflow(str); });
	auto allState = computePartTwo(workflows, "in", rangePossible());

	long long cumsum = 0;
	for (size_t i = 0; i < allState.size(); i++)
	{
		auto state1 = allState[i];
		auto ppp = state1.computePossibilities();
		cumsum += ppp;
	}

	long long cumDiffn = 0;
	for (size_t i = 0; i < allState.size(); i++)
	{
		auto state1 = allState[i];
		for (size_t j = i+1; j < allState.size(); j++) 
		{
			auto state2 = allState[j];
			auto diffN = computeVolumeOfOverlapping(state1, state2);
			cumDiffn += diffN;
		}
	}

	return cumsum;
}

int main()
{
	auto part1Test = computePart1Complete("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day19TestRatings.txt", "C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day19TestWorkFlows.txt");
	auto part2Test = computePart2Complete("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day19TestRatings.txt", "C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day19TestWorkFlows.txt");
	
	// BUG in part 2 I should not count things, but save all the intervals and then compute the final...
	
	auto part1Real = computePart1Complete("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day19RealRatings.txt", "C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day19RealWorkFlows.txt");
	//auto part2Real = computePart2Complete("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day19RealRatings.txt", "C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day19RealWorkFlows.txt");

	std::cout << "Part 1 test: " << part1Test << "; real: " << part1Real << std::endl;



	auto ending = 90;
}