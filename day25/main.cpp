
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

	std::map<std::string, std::vector<std::string>> graphRepresentation;
	std::vector<std::string> nodeNames;

	/*Graph25(std::string path)
	{

	}*/

	

	bool contractNodes()
	{
		if (graphRepresentation.size() > 2)
		{
			long long randomNodeid = randomNumber(graphRepresentation.size());
			std::string node1Name = nodeNames[randomNodeid];

			auto possibleNodesToContract = graphRepresentation[node1Name];
			long long randomNode2id = randomNumber(possibleNodesToContract.size());
			std::string node2Name = possibleNodesToContract[randomNode2id];

			auto adjacentNodesToNode1 = graphRepresentation[node1Name];
			auto adjacentNodesToNode2 = graphRepresentation[node2Name];
			std::vector<std::string> allnewLeaveNodes;

			allnewLeaveNodes = adjacentNodesToNode1;
			allnewLeaveNodes.insert(allnewLeaveNodes.end(), adjacentNodesToNode2.begin(), adjacentNodesToNode2.end());

			allnewLeaveNodes.erase(
				std::remove_if(
					allnewLeaveNodes.begin(),
					allnewLeaveNodes.end(),
					[node1Name, node2Name](std::string str) { return str == node1Name || str == node2Name; }),
				allnewLeaveNodes.end());

			std::string nodeMergedName = node1Name + ";" + node2Name;
			nodeNames.push_back(nodeMergedName);
			nodeNames.erase(
				std::remove_if(
					nodeNames.begin(),
					nodeNames.end(),
					[node1Name, node2Name](std::string str) { return str == node1Name || str == node2Name; }),
				nodeNames.end());

			for (auto leaf : allnewLeaveNodes)
			{
				//auto temp = graphRepresentation[leaf];
				
				auto it1 = std::find(graphRepresentation[leaf].begin(), graphRepresentation[leaf].end(), node1Name);
				if (it1 != graphRepresentation[leaf].end())
					*it1 = nodeMergedName;

				it1 = std::find(graphRepresentation[leaf].begin(), graphRepresentation[leaf].end(), node2Name);
				if (it1 != graphRepresentation[leaf].end())
					*it1 = nodeMergedName;

				graphRepresentation[leaf] = graphRepresentation[leaf];
			}

			graphRepresentation[nodeMergedName] = allnewLeaveNodes;
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
		while (!contractNodes())
		{

		}
	}

};

void main()
{

	//auto kol123a = readData("C:/Users/soder/source/repos/pezzeb/AdventOfCode2023/data/day25test.txt");
	auto kol123a = readData("C:/Users/soder/source/repos/pezzeb/AdventOfCode2023/data/day25real.txt");
	std::vector<std::string> allNodes;

	for (auto kv : kol123a)
	{
		allNodes.push_back(kv.first);
	}
	auto graphTest = Graph25();
	graphTest.graphRepresentation = kol123a;
	graphTest.nodeNames = allNodes;

	long long res;
	long long ijk = 0;
	while (true)
	{
		ijk++;
		std::cout << ijk << std::endl;
		auto copyOfGraph = graphTest;
		copyOfGraph.contractNodesWhile();

		auto kv1 = copyOfGraph.graphRepresentation.begin();
		//auto kv2 = copyOfGraph.graphRepresentation.begin() + 1;
		if (kv1->second.size() == 3)
		{
			auto weAreDone = 123;
			long long fac1 = splitStringIntoVector(kv1->first, ';').size();
			kv1++;
			long long fac2 = splitStringIntoVector(kv1->first, ';').size();
			res = fac1 * fac2;
			std::cout << "Ansewer: " << res << std::endl;
			break;
		}

	}
	//auto graphTest2 = graphTest;
	//auto graphTest3 = graphTest;
	//auto graphTest4 = graphTest;
	//graphTest2.contractNodesWhile();
	//graphTest3.contractNodesWhile();
	//graphTest4.contractNodesWhile();
	

}