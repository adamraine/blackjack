#ifndef COUNT_H_
#define COUNT_H_

#include <array>
#include "Card.h"

class Count {
  std::array<double, 10> counts;
public:
  Count(std::array<double, 10> _counts) : counts(_counts) {}
  double getCountValue(Card card);

  static Count hilo;
};

#endif // COUNT_H_
