

/*
  Devoir 1 
  Julien De Loor (julien.de-loor1@uqac.ca)
*/

#include "mathutils.hpp"
#include <cassert>

bool isPowerOfTwo(size_t x)
{
  unsigned int numberOfOneBits = 0;

  while (x && numberOfOneBits <= 1)
  {
    if ((x & 1) == 1)
    {
      numberOfOneBits++;
    }
    x >>= 1;
  }

  return (numberOfOneBits == 1);
}

uint8_t rand8()
{
  static unsigned const limit = RAND_MAX - RAND_MAX % 256;
  unsigned result = rand();
  while (result >= limit) {
    result = rand();
  }
  return static_cast<uint8_t>(result % 256);
}

uint64_t rand64()
{
  uint64_t results = 0;
  for (int count = 8; count > 0; --count) {
    results = 256U * results + rand8();
  }
  return results;
}

size_t randomRange(size_t begin, size_t end)
{
  assert(begin < end);
  size_t interval = end - begin;
  return (rand64() % interval) + begin;
}

