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
	ExpressionReturn potentialReturn;
	std::string variableOfExpression;

public:

	Expression(std::string stringToBeParsed) //px{ a < 2006:qkq,m>2090:A,rfg }
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
			variableOfExpression = std::string(stringToBeParsed.begin(), itOperator);
			auto rightSide = std::stoll(std::string(itOperator+1, it));
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

int main()
{
	auto part1Test = computePart1Complete("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day19TestRatings.txt", "C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day19TestWorkFlows.txt");
	auto part1Real = computePart1Complete("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day19RealRatings.txt", "C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day19RealWorkFlows.txt");
	
	std::cout << "Part 1 test: " << part1Test << "; real: " << part1Real << std::endl;

	//auto ratingsRealVec = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day19RealRatings.txt");
	//auto workflowsRealVec = readData("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day19RealWorkFlows.txt");
	//auto ratings = std::vector<Rating>(ratingsTestVec.size());
	//std::transform(ratingsTestVec.begin(), ratingsTestVec.end(), ratings.begin(), [](std::string str) { return Rating(str); });
	//auto workflows = std::vector<Workflow>(workflowsTestVec.size());
	//std::transform(workflowsTestVec.begin(), workflowsTestVec.end(), workflows.begin(), [](std::string str) { return Workflow(str); });
	//auto part1real = computePartOne(workflows, ratings);

	//Manual
	//auto firstRating = Rating("{x=787,m=2655,a=1222,s=2876}");
	//
	//auto w1 = Workflow("in{s<1351:px,qqz}");
	//auto w2 = Workflow("qqz{s>2770:qs,m<1801:hdj,R}");
	//auto w3 = Workflow("qs{s>3448:A,lnx}");
	//auto w4 = Workflow("lnx{m>1548:A,A}");

	//auto asdf1 = w1.computeRating(firstRating);
	//auto asdf2 = w2.computeRating(firstRating);
	//auto asdf3 = w3.computeRating(firstRating);
	//auto asdf4 = w4.computeRating(firstRating);

	//auto summan = firstRating.sumUpRating();

	auto ending = 90;
}