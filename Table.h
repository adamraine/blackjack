#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include <string>
#include "Player.h"

class Table {
  std::vector<std::vector<char> > normal;
  std::vector<std::vector<char> > pair;
  std::vector<std::vector<char> > soft;
public:
  Table(std::string filename);
  Decision getDecision(const Hand & hand, Card upcard);

  static Table basic_strategy;
};

#endif // TABLE_H_
