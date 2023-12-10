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

class directionHolder
{
    std::string direction;

public:
    directionHolder(std::string dirIn)
    {
        direction = dirIn;
    }

    directionHolder() = default;

    const char getNextDirection()
    {
        return direction.front();
    }

    void rotateDirection()
    {
        std::rotate(direction.begin(), direction.begin() + 1, direction.end());
    }

};


class traveler
{
    directionHolder travelDirection;
    //std::map<std::pair<std::string, std::pair<std::string, std::string>> travelMap;
    std::map<std::string, std::pair<std::string, std::string>> travelMap;

    void readData(std::string dataFile)
    {
        std::ifstream inputFile(dataFile);
        std::vector<std::string> data;
        std::string line;


        std::getline(inputFile, line);
        travelDirection = directionHolder(line);

        std::getline(inputFile, line); //EMPTY LINE
        while (std::getline(inputFile, line))
        {
            std::istringstream iss(line);

            //AAA = (BBB, BBB)
            std::string keyN, dummy, leftDirection, rightDirection;
            iss >> keyN >> dummy >> leftDirection >> rightDirection;

            leftDirection = std::string(leftDirection.begin() + 1, leftDirection.end() - 1);
            rightDirection = std::string(rightDirection.begin(), rightDirection.end() - 1);

            auto pairToInsert = std::pair<std::string, std::string>(leftDirection, rightDirection);

            travelMap.insert(std::pair<std::string, std::pair<std::string, std::string>>(keyN, pairToInsert));
        }

        //return data;
    }

    std::string takeStep(std::string fromPosition)
    {
        auto dir = travelDirection.getNextDirection();
        travelDirection.rotateDirection();
        auto toPositions = travelMap[fromPosition];
        std::string position;
        if (dir == 'L')
            return toPositions.first;
        else 
            return toPositions.second;
    }

public:
    traveler(std::string path)
    {
        readData(path);
    }
    traveler() = default;

    long long part1Computer()
    {
        std::string currentPosition = "AAA";
        long long nsteps = 0;

        while (currentPosition != "ZZZ")
        {
            nsteps++;
            currentPosition = takeStep(currentPosition);
        }
        return nsteps;
    }


};



int main()
{
    std::string str = "abcdef";

    //std::rotate(str.begin(), str.end(), 1);
    std::rotate(str.begin(), str.begin() + 1, str.end());

    auto testHand1 = traveler("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day8test1.txt");
    auto testPart1test1 = testHand1.part1Computer();

    auto testHand2 = traveler("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day8test2.txt");
    auto testPart1test2 = testHand2.part1Computer();

    auto realHand2 = traveler("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day8real.txt");
    auto realPart1 = realHand2.part1Computer();

    std::cout << "Part 1:" << "test 1: " << testPart1test1 << ", test 2: " << testPart1test2 << ", real: " << realPart1 << std::endl;

    int kola = 90;
}