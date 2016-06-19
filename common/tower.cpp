
#include "tower.hpp"
#include "shooter.hpp"
#include "mathutils.hpp"


Tower::Tower(unsigned int _x, unsigned int _est):
  x(_x),
  est(_est),
  positivEst(false),
  doNotUse(false),
  _shooter(nullptr),
  _realKills(_est)
{
}


Tower::~Tower()
{
  if (_shooter != nullptr)
  {
    _shooter->moveToTower(nullptr);
  }
}

unsigned int Tower::varEst() const
{
  if (positivEst)
  {
    return est + (int)(0.075f*(float)est);
  }
  return est;
}

void Tower::computeRealKills()
{
  if (positivEst)
  {
    _realKills = est + (int)(0.15f*(float)est*randomReal<float>());
  }
  else
  {
    _realKills = est + (int)(0.30f*(float)est*(-0.5f+randomReal<float>()));
  }
}

unsigned int Tower::realKills() const
{
  return _realKills;
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
  os << "x=" << t.x << ", est=" << t.est << ", weather_est=" << t.varEst();
  if (t.shooter())
  {
    os << ", shooter=" << t.shooter()->name << ", kills=" << t.realKills();
  }
  return os;
}




