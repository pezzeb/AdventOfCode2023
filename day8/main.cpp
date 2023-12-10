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
    std::string currentPosition;
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

public:
    std::string takeStep()
    {
        auto dir = travelDirection.getNextDirection();
        travelDirection.rotateDirection();
        auto toPositions = travelMap[currentPosition];
        std::string position;
        if (dir == 'L')
            currentPosition = toPositions.first;
        else 
            currentPosition = toPositions.second;

        return currentPosition;
    }


    traveler(std::string path, std::string startPoint)
    {
        readData(path);
        currentPosition = startPoint;
    }
    traveler(directionHolder travelDirectionIn, std::map<std::string, std::pair<std::string, std::string>> travelMapIn, std::string startPoint)
    {
        travelDirection = travelDirectionIn;
        travelMap = travelMapIn;
        currentPosition = startPoint;
    }

    long long part1Computer()
    {
        //std::string currentPosition = startingPoint;
        long long nsteps = 0;

        while (currentPosition != "ZZZ")
        {
            nsteps++;
            takeStep();
        }
        return nsteps;
    }

    long long part2Computer()
    {
        //std::string currentPosition = startingPoint;
        long long nsteps = 0;

        do 
        {
            nsteps++;
            takeStep();
        } while (currentPosition.back() != 'Z');

        return nsteps;
    }

    std::string getCurrentPosition()
    {
        return currentPosition;
    }


};

class part2Class
{
    std::vector<traveler> ghosts;

    std::string readDirection(std::string dataFile)
    {
        std::ifstream inputFile(dataFile);
        std::vector<std::string> data;
        std::string line;


        std::getline(inputFile, line);

        return line;
    }
    std::vector<std::string> readAllLeftHandSides(std::string dataFile)
    {
        std::ifstream inputFile(dataFile);
        std::vector<std::string> data;
        std::string line;
        std::vector<std::string> lhs;

        std::getline(inputFile, line);
        std::getline(inputFile, line);

        while (std::getline(inputFile, line))
        {
            std::istringstream iss(line);
            std::string lhsSingle;
            iss >> lhsSingle;
            if (lhsSingle.back() == 'A')
                lhs.push_back(lhsSingle);
        }

        return lhs;
    }

    std::map<std::string, std::pair<std::string, std::string>> readMappN(std::string dataFile)
    {
        std::ifstream inputFile(dataFile);
        std::vector<std::string> data;
        std::string line;
        std::map<std::string, std::pair<std::string, std::string>> kartaN;

        std::getline(inputFile, line);
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

            kartaN.insert(std::pair<std::string, std::pair<std::string, std::string>>(keyN, pairToInsert));
        }

        return kartaN;
    }

    bool isTravelersAtEnd()
    {
        for (auto ghost : ghosts)
        {
            if (ghost.getCurrentPosition().back() != 'Z')
                return false;
        }
        return true;
    }

public:
    part2Class(std::string path)
    {
        auto directionString = readDirection(path);
        auto allLeftHandSides = readAllLeftHandSides(path);
        auto kartN = readMappN(path);

        for (auto lhs : allLeftHandSides)
        {
            ghosts.push_back(traveler(directionString, kartN, lhs));
        }
    }

    long long computePart2naive()
    {
        long long nStep = 0;
        while (!isTravelersAtEnd())
        {
            nStep++;
            for(auto& ghost : ghosts)
                ghost.takeStep();
        }
        return nStep;
    }

    long long computePart2()
    {
        std::vector<long long> nStep;
        std::vector<long long> nStepSecond;
        
        for (auto& ghost : ghosts)
        {
            nStep.push_back(ghost.part2Computer());
            nStepSecond.push_back(ghost.part2Computer());
        }
        
        auto cunningLCM = std::lcm(nStep[0], nStep[1]);
        if (nStep.size() > 2)
        {
            auto it = nStep.begin() + 2;
            while (it!=nStep.end())
            {
                cunningLCM = std::lcm(cunningLCM, *it);
                it++;
            }
        }
        
        
        return cunningLCM;
    }

};



int main()
{

    auto testHand1 = traveler("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day8test1.txt", "AAA");
    auto testPart1test1 = testHand1.part1Computer();

    auto testHand2 = traveler("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day8test2.txt", "AAA");
    auto testPart1test2 = testHand2.part1Computer();

    auto realHand2 = traveler("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day8real.txt", "AAA");
    auto realPart1 = realHand2.part1Computer();

    std::cout << "Part 1:" << "test 1: " << testPart1test1 << ", test 2: " << testPart1test2 << ", real: " << realPart1 << std::endl;

   // PART 2 
    auto testHand3 = part2Class("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day8test3.txt");
    auto testHand3part2 = testHand3.computePart2();

    auto realHandPart2 = part2Class("C:/Users/soder/Source/Repos/pezzeb/AdventOfCode2023/data/day8real.txt");
    auto realPart2 = realHandPart2.computePart2();

    std::cout << "Part 2:" << "test: " << testHand3part2 << ", real: " << realPart2 << std::endl;


    int kola = 90;
}