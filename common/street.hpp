#ifndef STREET_HPP
#define STREET_HPP

/*
Devoir 2
Julien De Loor (julien.de-loor1@uqac.ca)
Defini une liste de tours ordonée
*/

#include <map>
class Tower;

class Street
{
public:
  std::map<unsigned int, Tower*> towers;

};

#endif