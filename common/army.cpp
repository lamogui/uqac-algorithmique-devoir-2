

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

  Solution B;
  std::set<Shooter*>::iterator is;
  for (is = shooters.begin(); is != shooters.end(); ++is)
  {
    //(*is)->moveToTower(queue.top());
    B.insert(queue.top());
    queue.top()->computeRealKills();
    queue.pop();
  }
  std::cout << "Value of solution found: " << Army::ComputeSolutionValue(B) << std::endl;
  std::cout << "Real value of solution found: " << Army::ComputeSolutionRealValue(B) << std::endl;
  affectSolution(B);

}

void Army::affectNaiveDist(Street& street, unsigned int dist)
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
        std::cerr << "Error failed to compute a valid solution" << std::endl;
        return;
      }
      t = queue.top();
      queue.pop();
    } while (!Army::CanAddToSolutionDist(B, *t, dist));
    
    t->computeRealKills();
    B.insert(t);
  }
  
  std::cout << "Value of solution found: " << Army::ComputeSolutionValue(B) << std::endl;
  std::cout << "Real value of solution found: " << Army::ComputeSolutionRealValue(B) << std::endl;
  affectSolution(B);
}


void Army::affectWeatherDist(Street& street, unsigned int dist)
{ 

  auto compare_func = [](Tower* t1, Tower* t2) { return t1->varEst() < t2->varEst(); };

  std::vector<Tower*> queue;
  queue.reserve(street.towers.size());

  Street::TowerMap::iterator it;
  for (it = street.towers.begin(); it != street.towers.end(); ++it)
  {
    queue.push_back(it->second);
  }

  std::sort(queue.begin(), queue.end(), compare_func);

  //Calcul instinctif d'une bonne solution
  Solution B;
  std::set<Shooter*>::iterator is;
  for (is = shooters.begin(); is != shooters.end(); ++is)
  {
    Tower* t; 
    if ((*is)->tower() != nullptr)
    {
      t = (*is)->tower();
    }

    else
    {
      do
      {
        if (queue.empty())
        {
          std::cerr << "Error failed to compute a valid solution" << std::endl;
          return;
        }
        t = queue.back();
        queue.pop_back();
      } while (!Army::CanAddToSolutionDist(B, *t, dist));
    }

    t->computeRealKills();
    B.insert(t);

    if (t->realKills() < t->est)
    {
      bool need_sort = false;
      Tower *np, *nn;
      street.getNeighbors(*t, np, nn);
      if (np != nullptr)
      {
        np->positivEst = true;
        need_sort = true;
      }
      if (nn != nullptr)
      {
        nn->positivEst = true;
        need_sort = true;
      }

      if (need_sort)
      {
        //On doit retrier ici et donc impossible d'utiliser priority_queue
        std::sort(queue.begin(), queue.end(), compare_func);
      }
    }

  }
  
  std::cout << "Value of solution found: " << Army::ComputeSolutionValue(B) << std::endl;
  std::cout << "Estimated Weather of solution found: " << Army::ComputeSolutionWeatherValue(B) << std::endl;
  std::cout << "Real value of solution found: " << Army::ComputeSolutionRealValue(B) << std::endl;
  affectSolution(B);
}


void Army::affectSolution(const Solution& solution)
{
  Solution::iterator is; 
  std::set<Shooter*>::iterator it = shooters.begin();

  for (is = solution.begin(); is != solution.end(); ++is)
  {
    if ((*is)->shooter() != nullptr)
    {
      continue;
    }
    
    while ((*it)->tower() != nullptr)
    {
      ++it;
      if (it == shooters.end())
      {
        return;
      }
    }

    (*it)->moveToTower(*is);
  }
}

bool Army::CanAddToSolutionDist(Solution& solution, const Tower& t, unsigned int dist)
{
  if (t.doNotUse)
  {
    return false;
  }
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

unsigned int Army::ComputeSolutionRealValue(Solution& solution)
{
  unsigned int value = 0;
  Army::Solution::iterator it;
  for (it = solution.begin(); it != solution.end(); ++it)
  {
    value += (*it)->realKills();
  }
  return value;
}


unsigned int Army::ComputeSolutionWeatherValue(Solution& solution)
{
  unsigned int value = 0;
  Army::Solution::iterator it;
  for (it = solution.begin(); it != solution.end(); ++it)
  {
    value += (*it)->varEst();
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

void Army::moveFromBadTowers()
{
  std::set<Shooter*>::iterator it;
  for (it = shooters.begin(); it != shooters.end(); ++it)
  {
    if ((*it)->tower() != nullptr)
    {
      if ((*it)->tower()->realKills() < (*it)->tower()->est)
      {
        (*it)->tower()->doNotUse = true;
        (*it)->moveToTower(nullptr);


        std::cout << (*it)->name << " will need to move because kills are under estimation" << std::endl;
      }
    }
  }
}
