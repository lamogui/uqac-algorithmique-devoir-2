
#include "tower.hpp"
#include "shooter.hpp"

Tower::Tower(unsigned int _x, unsigned int _est):
  x(_x),
  est(_est),
  _shooter(nullptr)
{
}


Tower::~Tower()
{
  if (_shooter != nullptr)
  {
    _shooter->moveToTower(nullptr);
  }
}


unsigned int Tower::Distance(const Tower& t1, const Tower& t2)
{
  if (t1.x > t2.x)
  {
    return t1.x - t2.x;
  }
  return t2.x - t1.x;
}

std::ostream& operator<<(std::ostream& os, const Tower& t)
{
  os << "x=" << t.x << ", est=" << t.est;
  if (t.shooter())
  {
    os << ", shooter=" << t.shooter()->name;
  }
  return os;
}


