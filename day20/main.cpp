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
		for(auto outputModule : outputModules)
		{ 
			voltageOfInputs[pulse.getCurrentModule()] = pulse.getVoltage();

			bool inputs = true;

			for (auto it = voltageOfInputs.begin(); it != voltageOfInputs.end(); it++)
			{
				inputs = inputs && (it->second == HIGH);
			}
			if (inputs)
				return std::vector<Pulse>{Pulse(outputModule, LOW)};
			else
				return std::vector<Pulse>{Pulse(outputModule, HIGH)};
		}
		
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
};

int main()
{
	// Test First Example Manual
	auto systemCircuit = Circuit();
	auto broadCaster = BroadCaster("a,b,c");
	auto flipflop1 = FlipFlop("b");
	auto flipflop2 = FlipFlop("c");
	auto flipflop3 = FlipFlop("inv");
	auto conjection1 = Conjunction("a", std::vector<std::string>{"c"}); //Conjunciton fungerar inte helt perfelt

	systemCircuit.modules["broadcaster"] = &broadCaster;
	systemCircuit.modules["a"] = &flipflop1;
	systemCircuit.modules["b"] = &flipflop2;
	systemCircuit.modules["c"] = &flipflop3;
	systemCircuit.modules["inv"] = &conjection1;

	systemCircuit.runCircuit();

	// Test Second Example Manual
	auto systemCircuit2 = Circuit();

	auto broadCaster2 = BroadCaster("a");
	systemCircuit2.modules["broadcaster"] = &broadCaster2;

	auto flipflop12 = FlipFlop(std::vector<std::string>{"inv", "con"});
	systemCircuit2.modules["a"] = &flipflop12;

	auto conjection12 = Conjunction("b", std::vector<std::string>{"a"});
	systemCircuit2.modules["inv"] = &conjection12;

	auto flipflop22 = FlipFlop("con");
	systemCircuit2.modules["b"] = &flipflop22;

	auto conjection22 = Conjunction("output", std::vector<std::string>{"a", "b"});
	systemCircuit2.modules["con"] = &conjection22;

	auto outputer = Output();
	systemCircuit2.modules["output"]      = &outputer;

	/*systemCircuit2.runCircuit();
	systemCircuit2.runCircuit();
	systemCircuit2.runCircuit();
	systemCircuit2.runCircuit();*/
	systemCircuit2.runCircuit(1000);

	auto ending = 90;
}