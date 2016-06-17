

#include "army.hpp"
#include "shooter.hpp"
#include "street.hpp"
#include "tower.hpp"


#include <cassert>
#include <cstdlib>
#include <queue>
#include <iostream>

Army::Army()
{

}

Army::~Army()
{
  reset();
}

void Army::reset()
{
  std::set<Shooter*>::iterator it;
  for (it = shooters.begin(); it != shooters.end(); ++it)
  {
    delete *it;
  }
  shooters.clear();
}

void Army::create(unsigned int count)
{
  reset();
  for (unsigned int i = 0; i < count; ++i)
  {
    shooters.insert(new Shooter());
  }
}

void Army::affectNaive(Street& street)
{
  auto compare_func = [](Tower* t1, Tower* t2) { return t1->est < t2->est; };
  std::priority_queue<Tower*, std::vector<Tower*>, decltype(compare_func)> queue;
  Street::TowerMap::iterator it;
  for (it = street.towers.begin(); it != street.towers.end(); ++it)
  {
    queue.push(it->second);
  }

  std::set<Shooter*>::iterator is;
  for (is = shooters.begin(); is != shooters.end(); ++is)
  {
    (*is)->moveToTower(queue.top());
    queue.pop();
  }
}

void Army::affectDist(Street& street, unsigned int dist, unsigned int iterations)
{
  auto compare_func = [](Tower* t1, Tower* t2) { return t1->est < t2->est; };
  std::priority_queue<Tower*, std::vector<Tower*>, decltype(compare_func)> queue;
  Street::TowerMap::iterator it;
  for (it = street.towers.begin(); it != street.towers.end(); ++it)
  {
    queue.push(it->second);
  }

  //Calcul instinctif d'une bonne solution
  Solution B;
  std::set<Shooter*>::iterator is;
  for (is = shooters.begin(); is != shooters.end(); ++is)
  {
    Tower* t; 
    do 
    {
      if (queue.empty())
      {
        std::cerr << "Error failed to compute the initial solution" << std::endl;
        return;
      }
      t = queue.top();
      queue.pop();
    } while (!Army::CanAddToSolutionDist(B, *t, dist));
    
    B.insert(t);
  }

  unsigned int E = ComputeSolutionValue(B);
  std::cout << "Instinctive initial solution: " << E << std::endl;

  unsigned int k = 0;

  unsigned int max_est_delta = 10;

  unsigned int count_best_solution = 0;

  //Pseudo recuit simulé
  Solution last_good_solution = B;
  for (k = 0; k < iterations; ++k)
  {
    Solution current_solution = last_good_solution; //On part de la derniere bonne solution

    //On va regarder le voisinage des solutions 
    Tower* r = RemoveRandomTower(current_solution); //Pour cela on retire une tour au hasard
    Tower* t = street.peekRandomTower(); //et on la remplace par une autre au hasard également
    
    unsigned int infinite_loop = 0;
    while (!CanAddToSolutionDist(current_solution, *t, dist) || t==r ) //On verifie que la nouvelle solution est valide
    {
      t = street.peekRandomTower();
      ++infinite_loop;
      if (infinite_loop > street.towers.size())
      {
        break;
      }
    }
    if (infinite_loop > street.towers.size())
    {
      std::cerr << "No valid solution in neighbourhood after " << infinite_loop << " iterations return instinctive solution" << std::endl;
      break;
    }

    current_solution.insert(t);
    
    unsigned int e = ComputeSolutionValue(current_solution);
    if (e >= E) //>= to prevent division by 0
    {
      //Une meilleure solution a été trouvé 
      B = current_solution;
      last_good_solution.swap(current_solution);
      ++count_best_solution;
      E = e;
      std::cout << "Better solution found in neighbourhood of initial solution: " << E << std::endl;
    }
    else if ( (rand() % (E-e)) < max_est_delta ) //Pseudo Recuit simulé ici en gros plus je m'eloigne d'une très bonne solution moins j'ai de chances de la selectionner
    {
      last_good_solution.swap(current_solution);
    }
  }

  std::cout << "Found " << count_best_solution << " better solution in neighbourhood of initial solution " << std::endl;
  std::cout << "Best solution found: " << E << std::endl;

  affectSolution(B);
}


void Army::affectSolution(const Solution& solution)
{
  Solution::iterator is; 
  std::set<Shooter*>::iterator it = shooters.begin();

  for (is = solution.begin(); is != solution.end() && it != shooters.end(); ++is)
  {
    (*it)->moveToTower(*is);
    ++it;
  }
}

bool Army::CanAddToSolutionDist(Solution& solution, const Tower& t, unsigned int dist)
{
  Solution::iterator it; 
  for (it = solution.begin(); it != solution.end(); ++it)
  {
    if (Tower::Distance(t,*(*it)) < dist)
    {
      return false;
    }
  }
  return true;
}

unsigned int Army::ComputeSolutionValue(Solution& solution)
{
  unsigned int value = 0;
  Army::Solution::iterator it;
  for (it = solution.begin(); it != solution.end(); ++it)
  {
    value += (*it)->est;
  }
  return value;
}


Tower* Army::RemoveRandomTower(Solution& s)
{
  assert(s.size());
  
  Solution::iterator it = s.begin();
  std::advance(it, rand() % s.size());
  Tower* t = *it;
  s.erase(it);
  return t;
}
