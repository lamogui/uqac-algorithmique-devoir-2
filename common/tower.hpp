
#ifndef TOWER_HPP
#define TOWER_HPP

/*
Devoir 2
Julien De Loor (julien.de-loor1@uqac.ca)
Defini les attributs d'une tour
*/

#include "shooter.hpp"

#include <ostream>

/*
  Algo TP2 Julien De Loor
  Defini les propriétés d'une tour
*/

class Tower
{
public:

  Tower(unsigned int _x, unsigned int _est);
  virtual ~Tower();

  unsigned int x;
  unsigned int est;
  bool positivEst; //When a tower close to this tower have est > real 

  bool doNotUse; //When shooter shoot less than est set this flag to true

  unsigned int varEst() const;    //Return est + 7.5% if positiv est is true else return est

  void computeRealKills(); // real number of kills
  unsigned int realKills() const;

 


  friend bool Shooter::moveToTower(Tower* t);
  friend Shooter::~Shooter();
  
  inline Shooter* shooter() const { return _shooter; }
  
  static unsigned int Distance(const Tower& t1, const Tower& t2);

private:
  Shooter* _shooter;
  unsigned int _realKills;

};

std::ostream& operator<<(std::ostream& os, const Tower& t);

#endif