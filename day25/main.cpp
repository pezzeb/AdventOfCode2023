
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


using edge = std::pair<std::string, std::string>;
using node = std::string;

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

std::map<std::string, std::vector<std::string>> readData(std::string dataFile)
{
	std::ifstream inputFile(dataFile);
	std::string line;
	std::map<std::string, std::vector<std::string>> data;

	while (std::getline(inputFile, line))
	{
		auto tmp = splitStringIntoVector(line, ':');
		auto tmp2 = splitStringIntoVector(tmp[1], ' ');
		tmp2.erase(tmp2.begin());

		data[tmp[0]] = tmp2;
	}

	for (auto kv : data)
	{
		for (auto node : kv.second)
		{
			data[node].push_back(kv.first);
		}

	}
	for (auto & kv : data)
	{
		sort(kv.second.begin(), kv.second.end());
		auto it = std::unique(kv.second.begin(), kv.second.end());
		
		kv.second.erase(it, kv.second.end());
	}

	return data;
}

class Graph25
{
	long long randomNumber(long long num)
	{
		auto randInt1 = rand() % num;
		return randInt1;
	}

public:

	// Kragers Algorithm

	std::map<std::string, std::vector<std::string>> graphRepresentation;
	std::vector<std::string> nodeNames;

	Graph25(std::string path)
	{
		auto dataRead = readData(path);


		std::vector<std::string> allNodes;

		for (auto kv : dataRead)
		{
			allNodes.push_back(kv.first);
		}
		graphRepresentation = dataRead;
		nodeNames = allNodes;
	}

	bool contractNodes()
	{
		if (graphRepresentation.size() > 2)
		{
			std::string node1Name = nodeNames[randomNumber(graphRepresentation.size())];
			auto possibleNodesToContract = graphRepresentation[node1Name];
			std::string node2Name = possibleNodesToContract[randomNumber(possibleNodesToContract.size())];

			auto allAdjacentNodes = graphRepresentation[node1Name];

			allAdjacentNodes.insert(allAdjacentNodes.end(), graphRepresentation[node2Name].begin(), graphRepresentation[node2Name].end());

			allAdjacentNodes.erase(std::remove(allAdjacentNodes.begin(), allAdjacentNodes.end(), node1Name), allAdjacentNodes.end());
			allAdjacentNodes.erase(std::remove(allAdjacentNodes.begin(), allAdjacentNodes.end(), node2Name), allAdjacentNodes.end());

			nodeNames.erase(std::remove(nodeNames.begin(), nodeNames.end(), node1Name), nodeNames.end());
			nodeNames.erase(std::remove(nodeNames.begin(), nodeNames.end(), node2Name), nodeNames.end());

			std::string nodeMergedName = node1Name + ";" + node2Name;
			nodeNames.push_back(nodeMergedName);

			for (auto leaf : allAdjacentNodes)
			{
				auto it1 = std::find(graphRepresentation[leaf].begin(), graphRepresentation[leaf].end(), node1Name);
				if (it1 != graphRepresentation[leaf].end())
					*it1 = nodeMergedName;

				it1 = std::find(graphRepresentation[leaf].begin(), graphRepresentation[leaf].end(), node2Name);
				if (it1 != graphRepresentation[leaf].end())
					*it1 = nodeMergedName;
			}

			graphRepresentation[nodeMergedName] = allAdjacentNodes;
			graphRepresentation.erase(graphRepresentation.find(node1Name));
			graphRepresentation.erase(graphRepresentation.find(node2Name));

			return false;
		}
		else
		{
			return true;
		}
	}

	void contractNodesWhile()
	{
		while (!contractNodes()) {}
	}
};

long long computeDay25(Graph25 graphN)
{
	long long res;
	while (true)
	{
		auto copyOfGraph = graphN;
		copyOfGraph.contractNodesWhile();

		auto kv1 = copyOfGraph.graphRepresentation.begin();
		if (kv1->second.size() == 3)
		{
			long long fac1 = splitStringIntoVector(kv1->first, ';').size();
			kv1++;
			long long fac2 = splitStringIntoVector(kv1->first, ';').size();
			res = fac1 * fac2;
			break;
		}
	}
	return res;
}

void main()
{
	Graph25 graphTest = Graph25("C:/Users/soder/source/repos/pezzeb/AdventOfCode2023/data/day25test.txt");
	auto testRes = computeDay25(graphTest);

	Graph25 graphReal = Graph25("C:/Users/soder/source/repos/pezzeb/AdventOfCode2023/data/day25real.txt");
	auto realRes = computeDay25(graphReal);

	std::cout << "Test: " << testRes << " and Real: " << realRes << std::endl;

}