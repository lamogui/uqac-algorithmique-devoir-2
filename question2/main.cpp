/*
Devoir 2 Question 2
Julien De Loor (julien.de-loor1@uqac.ca)
*/

#include "../common/street.hpp"
#include "../common/army.hpp"
#include <fstream>
#include <iostream>

#define WAITEND getchar()

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    std::cout << "usage: " << argv[0] << " <file.txt> <dist>" << std::endl;
    return 0;
  }
  unsigned int dist;
  if (sscanf(argv[2], "%u", &dist) != 1)
  {
    std::cerr << "Fatal Error: invalid <dist>" << std::endl;
    return -1;
  }


  std::string filename = argv[1];
  std::ifstream file;
  file.open(filename);
  if (!file)
  {
    std::cerr << "Fatal Error: Could not open " << filename.c_str() << std::endl;
    WAITEND;
    return -1;
  }

  unsigned int shooter_count; 
  if (!(file >> shooter_count))
  {
    std::cerr << "Fatal Error: Could not read the number of shooters" << std::endl;
    WAITEND;
    return -1;
  }


  Army army;
  army.create(shooter_count);

  std::cout << "Created an army of " << shooter_count << " shooters" << std::endl;

  Street street;
  if (!street.loadFromStream(file))
  {
    WAITEND;
    return -1;
  }

  std::cout << "Created a street of " << street.towers.size() << " towers" << std::endl;
  

  army.affectDist(street, dist);

  std::cout << "Affect our shooters to towers" << std::endl;

  std::cout << "--------------------------------------" << std::endl;
  std::cout << street << std::endl;


  WAITEND;
  return 0;
}