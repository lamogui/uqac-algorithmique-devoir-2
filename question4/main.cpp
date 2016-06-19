/*
Devoir 2 Question 4
Julien De Loor (julien.de-loor1@uqac.ca)
*/

#include "../common/street.hpp"
#include "../common/army.hpp"
#include <fstream>
#include <iostream>
#include <ctime>
#include <csignal>

#define WAITEND getchar()

static bool stop = false;

void sig_handler(int signo)
{
  if (signo == SIGINT)
  {
    std::cout << "Exiting..." << std::endl;
    stop = true;
  }
}

int main(int argc, char** argv)
{
  signal(SIGINT, sig_handler);

  std::cout << "Algo TP2 Question 4 Julien De Loor: julien.de-loor1@uqac.ca" << std::endl;
  srand((unsigned int)time(NULL));

  if (argc != 3)
  {
    std::cout << "usage: " << argv[0] << " <file.txt> <dist>" << std::endl;
    WAITEND;
    return 0;
  }
  unsigned int dist;
  if (sscanf_s(argv[2], "%u", &dist) != 1)
  {
    std::cerr << "Fatal Error: invalid <dist>" << std::endl;
    WAITEND;
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

  while (!stop)
  {

    street.resetWeatherEst();
    
    army.affectWeatherDist(street, dist);

    std::cout << "Affect our shooters to towers" << std::endl;

    std::cout << "--------------------------------------" << std::endl;
    std::cout << street << std::endl;

    std::cout << "Press any key to start next hour (CTRL+C to terminate)" << std::endl;

    WAITEND;
    
    std::cout << "Evaluate towers" << std::endl;

    street.resetTowerUsability();
    army.moveFromBadTowers();

  }
  return 0;
}