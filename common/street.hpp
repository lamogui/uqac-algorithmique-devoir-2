#ifndef STREET_HPP
#define STREET_HPP

/*
Devoir 2
Julien De Loor (julien.de-loor1@uqac.ca)
Defini une liste de tours ordonée
*/

#include <map>
#include <istream>
#include <ostream>

class Tower;

class Street
{
public:
  Street();
  virtual ~Street();

  void reset();
  bool loadFromStream(std::istream& stream);

  Tower* peekRandomTower() const ;

  typedef std::map<unsigned int, Tower*> TowerMap;

  TowerMap towers;
  std::string name;




};

std::ostream& operator<<(std::ostream& os, const Street& s);

#endif