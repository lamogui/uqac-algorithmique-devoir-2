
#ifndef ARMY_HPP
#define ARMY_HPP

/*
Devoir 2
Julien De Loor (julien.de-loor1@uqac.ca)
Defini une armée de tireurs
*/

#include <set>

class Shooter;
class Street;


class Army
{
public:
  Army();
  virtual ~Army();

  void reset();
  void create(unsigned int count);

  void affectNaive(Street& street);

  std::set<Shooter*> shooters;

};


#endif