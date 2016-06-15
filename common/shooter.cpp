
#include "shooter.hpp"
#include "tower.hpp"

Shooter::Shooter(const std::string& _name):
  name(_name),
  _tower(nullptr)
{
}

Shooter::Shooter():
  _tower(nullptr)
{
  name = Shooter::PeekName();
}

Shooter::~Shooter()
{
  if (_tower != nullptr)
  {
    _tower->_shooter = nullptr;
  }
}


bool Shooter::moveToTower(Tower* t)
{
  if (t && t->_shooter != nullptr)
  {
    return false;
  }

  if (_tower != nullptr)
  {
    _tower->_shooter = nullptr;
  }

  if (t)
  {
    t->_shooter = this;
  }

  _tower = t;
  return true;
}

#define NB_NAMES 40

static const char* names[NB_NAMES] = {
  "Sophie",
  "Joseph",
  "Marguerite",
  "Arthur",
  "Philomene",
  "Alfred",
  "Elisabeth",
  "Albert",
  "Henriette",
  "Georges",
  "Adelaide",
  "Adelard",
  "Emile",
  "Josephine",
  "Alphonse",
  "Caroline",
  "Henri",
  "Catherine",
  "Louis",
  "Mathilde",
  "Wilfrid",
  "Ernest",
  "Eugene",
  "Julie",
  "Philippe",
  "Charles",
  "Napoleon",
  "Edouard",
  "Pierre",
  "Omer",
  "Edmond",
  "Marie",
  "Celina",
  "Louise",
  "Adeline",
  "Adele",
  "Emelie",
  "Marceline",
  "Zoe",
  "Emilie"
};

const char* Shooter::PeekName()
{
  static int i = 0;
  const char* n = names[i];
  ++i;
  if (i >= NB_NAMES)
  {
    i = 0;
  }
  return n;
}

std::ostream& operator<<(std::ostream& os, const Shooter& s)
{
  os << "name=" << s.name;
  if (s.tower())
  {
    os << ", tower=(x=" << s.tower()->x << ", est=" << s.tower() << ")";
  }
  return os;
}


