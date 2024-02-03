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
#include <queue>
#include <regex>

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
	std::vector<std::string> output;
	for (auto ss : substrings)
	{
		output.push_back(ss);
	}
	return output;
}

enum VOLT { HIGH, LOW, DNQ };

class Pulse
{
private:
	std::string nextModule;
	std::string currentModule;
	VOLT singleVoltage;
	
public:

	Pulse(std::string nextModuleIn, std::string currentModuleIn, VOLT singleVoltageIn)
	{
		nextModule = nextModuleIn;
		currentModule = currentModuleIn;
		singleVoltage = singleVoltageIn;
	}

	Pulse(std::string nextModuleIn , VOLT singleVoltageIn)
	{
		nextModule = nextModuleIn;
		currentModule = "";
		singleVoltage = singleVoltageIn;
	}

	Pulse()
	{
		nextModule = "";
		currentModule = "";

		singleVoltage = DNQ;
	}

	Pulse(VOLT singleVoltageIn)
	{
		nextModule = "";
		currentModule = "";
		singleVoltage = singleVoltageIn;
	}

	std::string getNextModule()
	{
		return nextModule;
	}

	std::string getCurrentModule()
	{
		return currentModule;
	}

	VOLT getVoltage()
	{
		return singleVoltage;
	}

};

class Module
{
public:
	virtual std::vector<Pulse> computePulse(Pulse pulse) = 0;
};
class BroadCaster : public Module
{
public:
	std::vector<std::string> listOfreceivers;
	BroadCaster(std::string line)
	{
		listOfreceivers = splitStringIntoVector(line);
	}
	BroadCaster(std::vector<std::string> listOfreceiversIn)
	{
		listOfreceivers = listOfreceiversIn;
	}

	std::vector<Pulse> computePulse(Pulse pulse)
	{
		std::vector<Pulse> listOfPulses ;
		for (auto str : listOfreceivers)
		{
			listOfPulses.push_back(Pulse(str, "broadcaster", LOW));
		}
		return listOfPulses;
	}
};
class FlipFlop : public Module
{
private:
	VOLT currentVolt;
	std::vector<std::string> outputModules;

public:

	FlipFlop(std::vector<std::string> output)
	{
		currentVolt = LOW;
		outputModules = output;
	}
	FlipFlop(std::string output)
	{
		currentVolt = LOW;
		outputModules = std::vector<std::string>{ output };
	}

	std::vector<Pulse> computePulse(Pulse pulse)
	{
		if (pulse.getVoltage() == HIGH)
		{
			return std::vector<Pulse>{Pulse()};
		}
		else
		{
			if (currentVolt == HIGH)
			{
				currentVolt = LOW;
			}
			else
			{
				currentVolt = HIGH;
			}

			std::vector<Pulse> returnVector;
			for (std::string outputModule : outputModules)
			{
				if (currentVolt == LOW)
					returnVector.push_back(Pulse(outputModule, pulse.getNextModule(), LOW));
				else
					returnVector.push_back(Pulse(outputModule, pulse.getNextModule(), HIGH));
			}
			return returnVector;
		}
	}
};
class Conjunction : public Module
{
	std::map<std::string, VOLT> voltageOfInputs;
	std::vector<std::string> outputModules;
public:
	Conjunction(std::vector<std::string> output, std::vector<std::string> listOfInputs)
	{
		outputModules = output;
		for (auto str : listOfInputs)
			voltageOfInputs[str] = LOW;
	}
	Conjunction(std::string output, std::vector<std::string> listOfInputs)
	{
		outputModules = std::vector<std::string>{ output };
		for (auto str : listOfInputs)
			voltageOfInputs[str] = LOW;
	}

	std::vector<Pulse> computePulse(Pulse pulse)
	{
		if (voltageOfInputs.find(pulse.getCurrentModule()) != voltageOfInputs.end())
			voltageOfInputs[pulse.getCurrentModule()] = pulse.getVoltage();
		else
			auto breakpointy = 90;
		
		bool inputs = true;
		for (auto it = voltageOfInputs.begin(); it != voltageOfInputs.end(); it++)
		{
			inputs = inputs && (it->second == HIGH);
		}

		std::vector<Pulse> returnVector;
		for(auto outputModule : outputModules)
		{ 
			if (inputs)
				returnVector.push_back(Pulse(outputModule, pulse.getNextModule(), LOW));
			else
				returnVector.push_back(Pulse(outputModule, pulse.getNextModule(), HIGH));
		}
		return returnVector;
		
	}
};

class Output : public Module
{
private:

public:

	Output(){}

	std::vector<Pulse> computePulse(Pulse pulse)
	{
		return std::vector<Pulse> {Pulse()};
	}
};

class Circuit
{
private:
	long long numberOfLow;
	long long numberOfHigh;
public:
	std::queue<Pulse> pulseQueue;
	std::map <std::string, Module*> modules;

	Circuit() = default;

	void addPulsesToBeHandled(std::vector<Pulse> vec)
	{
		for (auto it : vec)
		{
			pulseQueue.push(it);
		}
	}

	void handlePulse()
	{
		Pulse it = pulseQueue.front();
		pulseQueue.pop();
		if (it.getVoltage() == DNQ)
		{ 
			// Do nothing
		}
		else
		{
			if (it.getVoltage() == LOW)
				numberOfLow++;
			else
				numberOfHigh++;

			auto nextModule = modules[it.getNextModule()];
			auto morePulses = nextModule->computePulse(it);
			addPulsesToBeHandled(morePulses);
		}
	}

	void runCircuit()
	{
		pulseQueue.push(Pulse("broadcaster", LOW));
		while (true)
		{
			if (pulseQueue.size() == 0)
				break;
			else
				handlePulse();
		}
	}

	void runCircuit(long long n)
	{
		for (size_t i = 0; i < n; i++)
		{
			runCircuit();
		}
	}

	long long computePart1()
	{
		return numberOfLow * numberOfHigh;
	}
};

std::string removeSpacesLine(std::string line)
{
	line = std::regex_replace(line, std::regex(" "), "");
	line = std::regex_replace(line, std::regex("->"), ",");
	return line;
}


std::vector<std::string> readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);
	std::string line;
	std::vector<std::string> data;

	while (std::getline(inputFile, line))
	{
		//std::istringstream iss(line);
		data.push_back(line);
	}
	return data;
}

void parseSingleLine(std::string line, Circuit& circuit, std::map<std::string, std::vector<std::string>>& conjMap)
{
	auto splitLine = splitStringIntoVector(removeSpacesLine(line));
	auto rhs = std::vector<std::string>(splitLine.begin() + 1, splitLine.end());

	if (splitLine[0].find("broadcaster") != std::string::npos)
	{
		circuit.modules.insert(std::make_pair("broadcaster", new BroadCaster(rhs)));
	}
	else if (splitLine[0].find("%") != std::string::npos)
	{
		auto lhsName = std::string(splitLine[0].begin() + 1, splitLine[0].end());
		circuit.modules.insert(std::make_pair(lhsName, new FlipFlop(rhs)));
	}
	else if (splitLine[0].find("&") != std::string::npos)
	{
		auto lhsName = std::string(splitLine[0].begin() + 1, splitLine[0].end());
		auto ttttt = conjMap[lhsName];
		circuit.modules.insert(std::make_pair(lhsName, new Conjunction(rhs, ttttt)));
	}
}

std::map<std::string, std::vector<std::string>> 
extractAllConjunctions(std::vector<std::string> vec)
{
	std::vector<std::string> outputVec;
	std::map<std::string, std::vector<std::string>> tmp;

	for (auto line : vec)
	{
		auto splitLine = splitStringIntoVector(removeSpacesLine(line));
		if (splitLine[0].find("&") != std::string::npos)
		{
			auto lhsName = std::string(splitLine[0].begin() + 1, splitLine[0].end());
			outputVec.push_back(lhsName);
			tmp[lhsName] = std::vector<std::string>{};
		}
	}
	return tmp;
}

void buildUpConjuctionMap(std::vector<std::string> inputLines, std::map<std::string, std::vector<std::string>>& conjName)
{
	std::vector<std::string> outputVec;
	for (auto line : inputLines)
	{
		auto splitLine = splitStringIntoVector(removeSpacesLine(line));

		auto lhsName = std::string(splitLine[0].begin() + 1, splitLine[0].end());
		auto rhsNames = std::vector<std::string>(splitLine.begin()+1,splitLine.end());
		for (auto rhs : rhsNames)
		{
			auto ggg = conjName.find(rhs);
			if (ggg != conjName.end())
			{
				ggg->second.push_back(lhsName);
			}
		}
	}
}

Circuit 
parseInputToCircuit(std::string pathOfInput)
{
	auto inputLines = readData(pathOfInput);//
	auto allConj = extractAllConjunctions(inputLines);
	buildUpConjuctionMap(inputLines, allConj);

	auto circuit = Circuit();
	for (auto line : inputLines)
	{
		parseSingleLine(line, circuit, allConj);
	}
	return circuit;
}

int main()
{
	// Test First Example Manual
	//auto systemCircuit = Circuit();

	//systemCircuit.modules.insert(std::make_pair("broadcaster", new BroadCaster("a,b,c")));
	//systemCircuit.modules.insert(std::make_pair("a", new FlipFlop("b")));
	//systemCircuit.modules.insert(std::make_pair("b", new FlipFlop("c")));
	//systemCircuit.modules.insert(std::make_pair("c", new FlipFlop("inv")));
	//systemCircuit.modules.insert(std::make_pair("inv", new Conjunction("a", std::vector<std::string>{"c"})));

	//systemCircuit.runCircuit();
	//systemCircuit.runCircuit(999);

	//// Test Second Example Manual
	//auto systemCircuit2 = Circuit();

	//systemCircuit2.modules.insert(std::make_pair("broadcaster", new BroadCaster("a")));
	//systemCircuit2.modules.insert(std::make_pair("a", new FlipFlop(std::vector<std::string>{"inv", "con"})));
	//systemCircuit2.modules.insert(std::make_pair("inv", new Conjunction("b", std::vector<std::string>{"a"})));
	//systemCircuit2.modules.insert(std::make_pair("b", new FlipFlop("con")));
	//systemCircuit2.modules.insert(std::make_pair("con", new Conjunction("output", std::vector<std::string>{"a", "b"})));
	//systemCircuit2.modules.insert(std::make_pair("output", new Output()));

	//systemCircuit2.runCircuit();
	//systemCircuit2.runCircuit();
	//systemCircuit2.runCircuit();
	//systemCircuit2.runCircuit();
	//systemCircuit2.runCircuit(996);

	// Read Input of Testing circuits
	//Circuit testCircuit1 = parseInputToCircuit("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day20test1.txt");
	//testCircuit1.runCircuit();
	//testCircuit1.runCircuit(999);
	//std::cout << "Test 1 part 1: " << testCircuit1.computePart1() << std::endl;

	//Circuit testCircuit2 = parseInputToCircuit("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day20test2.txt");
	//testCircuit2.modules.insert(std::make_pair("output", new Output()));
	//testCircuit2.runCircuit();
	//testCircuit2.runCircuit(999);
	//std::cout << "Test 2 part 1: " << testCircuit2.computePart1() << std::endl;

	Circuit realCircuit = parseInputToCircuit("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day20real.txt");
	realCircuit.modules.insert(std::make_pair("rx", new Output()));
	realCircuit.runCircuit();
	realCircuit.runCircuit(999);
	std::cout << "Real part 1: " << realCircuit.computePart1() << std::endl; //137247232 is too low

	auto ending = 90;
}