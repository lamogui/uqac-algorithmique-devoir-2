

#include "street.hpp"
#include "tower.hpp"

#include <iostream>


Street::Street():
  name("Renascence Street")
{
}

Street::~Street()
{
  reset();
}

void Street::reset()
{
  TowerMap::iterator it;
  for (it = towers.begin(); it != towers.end(); ++it)
  {
    delete it->second;
  }
  towers.clear();
}


bool Street::loadFromStream(std::istream& stream)
{
  reset();
  if (!stream)
  {
    std::cerr << "Failed to load Street: invalid stream" << std::endl;
    return false;
  }
  unsigned int n = 0;
  if (!(stream >> n))
  {
    std::cerr << "Failed to load Street: invalid stream while getting number of tower" << std::endl;
    return false;
  }

  for (unsigned int i = 0; i < n; i++)
  {
    unsigned int x, est;
    if (!(stream >> x >> est))
    {
      std::cerr << "Failed to load Street: invalid stream while getting tower " << i << " infos" << std::endl;
      return false;
    }

    TowerMap::iterator it = towers.find(x);
    if (it != towers.end())
    {
      std::cerr << "Failed to load Street: error loading tower " << i << " infos, tower at " << x << " already exists" << std::endl;
      return false;
    }
    towers[x] = new Tower(x, est);
  }
  return true;
}


std::ostream& operator<<(std::ostream& os, const Street& s)
{
  os << s.name;
  if (!s.towers.empty())
  {
    os << " (" << s.towers.size() << " towers)" << std::endl;
    os << "--------------------------------------" << std::endl;
    Street::TowerMap::const_iterator it;
    for (it = s.towers.cbegin(); it != s.towers.cend();)
    {
      os << "  O " << *(it->second) << std::endl;
      int x = it->first;
      if ((++it) != s.towers.cend())
      {
        int d = it->first - x;
        d /= 3;
        for (int i = 0; i < d; i++)
        {
          os << "  |" << std::endl;
        }
      }
    }
  }
  return os;
}
