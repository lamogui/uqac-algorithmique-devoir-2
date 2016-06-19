
/*
Devoir 2 Question 6
Julien De Loor (julien.de-loor1@uqac.ca)

Probleme du voyageur du commerce j'ai utilisé un algorithme genetique...
Ouai je sais c'est pour les feinéants mais j'était occupé à faire sa: 
http://www.pouet.net/prod.php?which=67607

*/

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>
#include <iostream>
#include <cmath>
#include <map>
#include <ctime>
#include <algorithm>

#include "../common/mathutils.hpp"

#define WAITEND getchar()

typedef struct 
{
  float x;
  float y;
} pointf;

//Techniquement c'est pas des villes mais des tours 
//Bon c'est pas très grave
typedef pointf city; 


float manathan_distance(const pointf& p1, const pointf& p2)
{
  return fabs(p1.x - p2.x) + fabs(p1.y - p2.y);
}

float euclidian_distance(const pointf& p1, const pointf& p2)
{
  return sqrt(pow(p1.x-p2.x,2.f) + pow(p1.y - p2.y,2.f));
}

typedef float (*distance_func)(const pointf&, const pointf&);


typedef std::vector<int> solution_data;

void mutate_solution_data(solution_data& s, unsigned int n)
{
  for (unsigned int i = 0; i < n; ++i)
  {
    size_t i1, i2;
    i1 = randomRange(0, s.size());
    i2 = randomRange(0, s.size());

    std::iter_swap(s.begin() + i1, s.begin() + i2);
  }
}


float fitness(const solution_data& s, const std::map<int, city>& cities, distance_func d)
{
  if (cities.empty())
  {
    return 0;
  }
  float tot_distance = 0;

  solution_data::const_iterator it = s.cbegin();
  std::map<int, city>::const_iterator cit = cities.find(*it);
  assert(cit != cities.cend());
  city previous_city = cit->second; ++it;
  
  for (; it != s.end(); ++it)
  {
    cit = cities.find(*it);
    assert(cit != cities.cend());
    tot_distance += d(previous_city, cit->second);
    previous_city = cit->second;
  }
  return tot_distance;
}

typedef struct
{
  solution_data order;
  float fitness;
} solution;


void mutate_solution(solution& s, unsigned int n)
{
  mutate_solution_data(s.order, n);
}


void print_solution(const solution& s)
{
  std::cout << "fitness: " << s.fitness;
  std::cout << " order: " << s.order.front();

  for (size_t i = 1; i < s.order.size(); ++i)
  {
    std::cout << "->" << s.order[i];
  }
  
}


int main(int argc, char** argv)
{
  std::cout << "Algo TP2 Question 5 Julien De Loor: julien.de-loor1@uqac.ca" << std::endl;
  srand((unsigned int)time(NULL));
  if (argc != 2)
  {
    std::cout << "usage: " << argv[0] << " <file.txt>" << std::endl;
    WAITEND;
    return 0;
  }

  std::string filename = argv[1];
  std::ifstream file;
  file.open(filename.c_str());
  if (!file)
  {
    std::cerr << "Error: can't open file: " << filename << std::endl;
    return -1;
  }

  std::map<int, city> cities; 


  //Exctraction des données
  while (!file.eof())
  {
    int id;
    city c;
    file >> id >> c.x >> c.y;

    if (cities.find(id) != cities.end())
    {
      std::cerr << "Error: city " << id << " already exists" << std::endl;
      return -1;
    }

    cities[id] = c;
  }


  //Algorithme génétique !
  
  const size_t populationCount = 200;                                   //Nombre d'inidivus minimum 
  const size_t solutionSize = cities.size();                            //Nombre de villes (ou plutot tours)
  const unsigned int maxIterations = 100000;                            //Nombre maximal d'iterations
  const unsigned int maxUnchangedIterations = 900;                      //Nombre maximal d'iteration lorsque l'on ne trouve pas une meilleure solution
  const unsigned int minMutation = 0, maxMutations = cities.size() / 7; //Combien de mutations par enfant
  const unsigned int basePopulation = 10;                               //Combien d'individus selectionnés parmis les meilleurs 
  const distance_func distance = manathan_distance;

  solution_data copyable_solution_data;
  copyable_solution_data.reserve(solutionSize);
  for (std::map<int, city>::const_iterator it = cities.cbegin();
    it != cities.cend(); ++it)
  {
    copyable_solution_data.push_back(it->first);
  }

  std::vector<solution> population, previous_population;
  population.resize(populationCount); 

  for (std::vector<solution>::iterator it = population.begin();
    it != population.end(); ++it)
  {
    it->order = copyable_solution_data;
    mutate_solution(*it, solutionSize); //Randomize solution
    it->fitness = fitness(it->order, cities, distance);
  }

  auto sort_func = [](const solution& s1, const solution&s2) { return (bool)(s1.fitness < s2.fitness); };

  solution best;
  best.fitness = INFINITY;

  unsigned int unchanged_iteration = 0;
  unsigned int iteration;
  for (iteration = 0;
    iteration < maxIterations && unchanged_iteration < maxUnchangedIterations;
    ++iteration)
  {

    std::sort(population.begin(), population.end(), sort_func); 
    if (population.front().fitness < best.fitness) //On regarde si on a trouvé une meilleure solution
    {
      best = population.front();
      unchanged_iteration = 0;
    }
    else
    {
      ++unchanged_iteration;
    }

    std::vector<solution> best_cpy = population;
    best_cpy.resize(basePopulation);//On garde les meilleurs
    //On mixe avec leur parents
    best_cpy.insert(best_cpy.end(), previous_population.cbegin(), previous_population.cend());
    population = best_cpy;  

    while (population.size() < populationCount) //On clone les meilleurs
    {
      population.insert(population.end(), best_cpy.begin(), best_cpy.end());
    }

    

    previous_population = population;           //Les enfants devient parent/grandparents
    previous_population.resize(basePopulation); //On garde seulement une partie
   
    for (std::vector<solution>::iterator it = population.begin();
      it != population.end(); ++it) //On génére les enfants
    {
      mutate_solution(*it, randomRange(minMutation, maxMutations)); 
      it->fitness = fitness(it->order, cities, distance); //On calcul la valeur de chaque solution
    }

    //Juste pour avoir une idée de où on en est dans la resolution
    if (iteration % (maxUnchangedIterations / 2) == 0)
    {
      std::cout << "iteration: " << iteration << " best actual solution: ";
      print_solution(best);
      std::cout << std::endl;
    }

  }

  std::cout << std::endl;
  std::cout << "Ended after " << iteration << " iterations, Best solution found: " << std::endl;
  print_solution(best);
  std::cout << std::endl << " Manathan distance: " << fitness(best.order, cities, manathan_distance);
  std::cout << " Euclidian distance: " << fitness(best.order, cities, euclidian_distance) << std::endl;

  WAITEND;
  return 0;
}