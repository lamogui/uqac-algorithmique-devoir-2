

#include "army.hpp"
#include "shooter.hpp"
#include "street.hpp"
#include "tower.hpp"

#include <queue>

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

