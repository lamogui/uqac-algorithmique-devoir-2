
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
class Tower;


class Army
{
public:
  Army();
  virtual ~Army();
  typedef std::set<Tower*> Solution;

  void reset();
  void create(unsigned int count);

  void affectNaive(Street& street);
  void affectNaiveDist(Street& street, unsigned int dist);

  void affectWeatherDist(Street& street, unsigned int dist);


  void affectSolution(const Solution& solution);

  void moveFromBadTowers();   


  static Tower* RemoveRandomTower(Solution& s);


  std::set<Shooter*> shooters;
  
  unsigned int ComputeSolutionValue(Solution& solution);
  unsigned int ComputeSolutionRealValue(Solution& solution);
  unsigned int ComputeSolutionWeatherValue(Solution& solution);

  static bool CanAddToSolutionDist(Solution& solution, const Tower& t, unsigned int dist);

};


#endif