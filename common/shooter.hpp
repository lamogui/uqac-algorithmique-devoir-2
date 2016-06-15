
#ifndef SHOOTER_HPP
#define SHOOTER_HPP

/*
Devoir 2
Julien De Loor (julien.de-loor1@uqac.ca)
*/

#include <string>

class Tower;

class Shooter
{
public:
  Shooter();
  Shooter(const std::string& name);
  virtual ~Shooter();


  std::string name;

  static const char* PeekName();


  bool moveToTower(Tower* t);
  inline Tower* tower() const { return _tower; }


private:
    Tower* _tower;


};

std::ostream& operator<<(std::ostream& os, const Shooter& s);


#endif
