
#ifndef TOWER_HPP
#define TOWER_HPP

/*
Devoir 2
Julien De Loor (julien.de-loor1@uqac.ca)
Defini les attributs d'une tour
*/

#include "shooter.hpp"

#include <ostream>

class Tower
{
public:

  Tower(unsigned int _x, unsigned int _est);
  virtual ~Tower();

  unsigned int x;
  unsigned int est;


  friend bool Shooter::moveToTower(Tower* t);
  friend Shooter::~Shooter();
  
  inline Shooter* shooter() const { return _shooter; }

private:
  Shooter* _shooter;

};

std::ostream& operator<<(std::ostream& os, const Tower& t);

#endif