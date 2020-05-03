#include <iostream>
#include <fstream>
#include "avlbst.h"
#include <map>
#include <set>

using namespace std;


typedef AVLTree<char, int> ANSWER_MAP_T;
void printSolution(ANSWER_MAP_T& answer, std::ostream& os);
bool determineColoring(std::map<char, std::set<char>>& links, AVLTree<char, int>& coloring, int idx, std::ostream& os);
bool isValid(std::map<char, std::set<char>>& links, AVLTree<char, int>& coloring, int idx);

int main(int argc, char* argv[])
{
    if(argc < 3) {
        cout << "Please supply a map input file and output filename" << endl;
        return 1;
    }
    // COMPLETE main()
    ifstream in_file(argv[2]);
    ofstream out_file(argv[3]);

    std::cout << "files opened" << std::endl;

    int numCountries;
    int rows;
    int cols;
    char holdNextLineChar;
    std::set<char> countryList;
    std::map<char, std::set<char>> connections; //used for tracking countries to which a contry is connected

    std::cout << "variables created" << std::endl;

    in_file >> numCountries >> rows >> cols;
    std::cout << "variables initialized" << std::endl;

    //this size will allow easy comparisons between upper/lower/left/right grids
    char** cMap = new char*[rows+2];
    for (int i = 0; i < rows+2; i++)
    {
      cMap[i] = new char[cols+2];
    }
    // filling the map
    for (int i = 1; i < rows+1; i++)
    {
      for (int j = 1; j < cols+1; j++)
      {
        in_file >> cMap[i][j];
        countryList.insert(cMap[i][j]); //keeps track of all unique chars
      }
      in_file >> holdNextLineChar;
    }
    std::cout << "cMap created" << std::endl;

    for (std::set<char>::iterator it = countryList.begin(); it != countryList.end(); ++it)
    {
      std::cout << "printing countryList" << std::endl;
      std::cout << *it << std::endl;
    }

    //assigning outer edges of chars in the map to null equivalent
    std::cout << "filling cMap" << std::endl;
    for (int i = 0; i < cols+2; i++)
      {
        std::cout << "here" << std::endl;
        cMap[0][i] = '\0';
        std::cout << "here" << std::endl;
        cMap[rows+1][i] = '\0';
      }
    for (int  i = 0; i < rows+2; i++)
    {
      cMap[i][0] = '\0';
      cMap[i][cols+1] = '\0';
    }
    std::cout << "printing cMap" << std::endl;
    for (int i = 0; i < rows+2; i++)
    {
      for (int j = 0; j < cols+2; j++)
      {
        std::cout << cMap[i][j];
      }
      std::cout << std::endl;
    }

    AVLTree<char, int> coloring;
    std::set<char>::iterator countryIt;
    for (countryIt = countryList.begin(); countryIt != countryList.end(); ++countryIt)
    {
      connections.insert(std::make_pair(*countryIt, std::set<char>()));
      coloring.insert(std::make_pair(*countryIt, 0));
    }
    std::cout << "filled coloring, countryIt" << std::endl;
    //make connections
    for (int i = 1; i < rows+2; i++)
    {
      for (int j = 1; j < cols+2; j++)
      {
        char current = cMap[i][j];
        if (current != cMap[i][j-1] && cMap[i][j-1] != '\0')
          connections[current].insert(cMap[i][j-1]);
        if (current != cMap[i][j+1] && cMap[i][j+1] != '\0')
          connections[current].insert(cMap[i][j+1]);
        if (current != cMap[i-1][j] && cMap[i-1][j] != '\0')
          connections[current].insert(cMap[i-1][j]);
        if (current != cMap[i+1][j] && cMap[i+1][j] != '\0')
          connections[current].insert(cMap[i+1][j]);
      }
    }
    std::cout << "printing all nodes in connections" << std::endl;
    for (std::map<char, std::set<char>>::iterator it = connections.begin(); it != connections.end(); ++it)
    {
      std::cout << it->first << std::endl;
    }
    determineColoring(connections, coloring, 0, out_file);

    // Call printSolution to output the answers to the output file

    for (int i = 0; i < rows+2; i++)
    {
      delete [] cMap[i];
    }
    delete [] cMap;
    return 0;
}

bool determineColoring(std::map<char, std::set<char>>& links, AVLTree<char, int>& coloring, int idx, std::ostream& os)
{
  if ((unsigned int) idx == links.size())  //if every country has been assigned a color, print the solution
  {
    printSolution(coloring, os);
    return true;
  }
  AVLTree<char, int>::iterator colorIt = coloring.begin();  //used to specify the appropriate country whose color is being modified
  for (int i = 0; i < idx; i++)
    { ++colorIt; }
  idx++;  //used to indicate that one more node is now under scrutiny during the isValid call
  for (int i = 1; i <= 4; i++) //checks all possible colorings
  {
    colorIt->second = i;  //assign a possible coloring for the country
    if (isValid(links, coloring, idx))  //if the coloring works for now...
    {
      if (determineColoring(links, coloring, idx, os))  //see if other colorings will complete the overall coloring
        {
          break;  //will reach here if we have already gone through printSolution
        }
    }
  }
  return false;
}

bool isValid(std::map<char, std::set<char>>& links, AVLTree<char, int>& coloring, int idx)
{
  std::map<char, std::set<char>>::iterator linksIt = links.begin(); //will keep track of which country is being tested against its links
  for (int i = 0; i < idx; i++) //will check every country who has been assigned a color thus far
  { //will check every connection a given country has
    for (std::set<char>::iterator linkConnection = linksIt->second.begin(); linkConnection != linksIt->second.end(); ++linkConnection)
    { //if the country has a connection who is assigned the same color as it, the coloring is not valid
      if (coloring.find(*linkConnection)->second == coloring.find(linksIt->first)->second)
        return false;
    }
    linksIt++;
  }
  //if no country's coloring contradict's another's, returns true
  return true;
}

void printSolution(ANSWER_MAP_T& answer, std::ostream& os)
{
    for(ANSWER_MAP_T::iterator it = answer.begin();
            it!= answer.end();
            ++it) {
        os << it->first << " " << it->second << endl;
    }
}
