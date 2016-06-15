

#include "army.hpp"
#include "shooter.hpp"
#include "street.hpp"
#include "tower.hpp"

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

void Army::affectDist(Street& street, unsigned int dist)
{
  auto compare_func = [](Tower* t1, Tower* t2) { return t1->est < t2->est; };
  std::priority_queue<Tower*, std::vector<Tower*>, decltype(compare_func)> queue;
  Street::TowerMap::iterator it;
  for (it = street.towers.begin(); it != street.towers.end(); ++it)
  {
    queue.push(it->second);
  }

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
