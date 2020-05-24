#include "Table.h"

#include <fstream>
#include <sstream>
#include "Utils.h"

#define NORMAL_MIN 5 // Total value
#define SOFT_MIN 13 // Total value
#define PAIR_MIN 1 // Individual value
#define UPCARD_MIN 1 // Individual value

using std::vector;
using std::string;
using std::istringstream;
using std::fstream;

Table Table::basic_strategy = Table("strategy/basic.csv");

static void addToTable(vector<vector<char> > & vec, istringstream & ss) {
  vector<char> row;
  string val;
  while(getline(ss, val, ',')) {
    row.push_back(std::stoi(val));
  }
  vec.push_back(row);
}

Table::Table(string filename) {
  fstream fin(filename);
  string line, row_name;
  getline(fin, line); // Ignore headers
  while(getline(fin, line)) {
    std::istringstream ss(line);
    getline(ss, row_name, ',');
    if(row_name.find('-') != string::npos) {
      addToTable(soft, ss);
    }
    else if(row_name.find('|') != string::npos) {
      addToTable(pair, ss);
    }
    else {
      addToTable(normal, ss);
    }
  }
}

Decision Table::getDecision(const Hand & hand, Card upcard) {
  size_t upcard_index = (upcard.isSoft() ? 0 : upcard.getValue() - UPCARD_MIN);
  char decision;
  if(hand.isPair()) {
    if(hand.isSoft()) {
      decision = pair[0][upcard_index];
    }
    else {
      decision = pair[hand.getCards()[0].getValue() - PAIR_MIN][upcard_index];
    }
  }
  else if(hand.isSoft()) {
    decision = soft[hand.totalValue() - SOFT_MIN][upcard_index];
  }
  else {
    decision = normal[hand.totalValue() - NORMAL_MIN][upcard_index];
  }

  // Convert table value to decision
  switch(decision) {
    case 1:
      return Decision::hit;
    case 2:
      return Decision::stand;
    case 3:
      return Decision::split;
    case 4:
      if(hand.getCards().size() == 2) {
        return Decision::double_down;
      }
      return Decision::hit;
    case 5:
      if(hand.getCards().size() == 2) {
        return Decision::double_down;
      }
      return Decision::stand;
    default:
      ERROR("Invalid table value.");
  }
}
