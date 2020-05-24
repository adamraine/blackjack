#include <getopt.h>
#include "Utils.h"

std::string Options::players = "";
int Options::num_decks = 6;
int Options::num_rounds = 1;
size_t Options::deck_seed = 0;
bool Options::verbose_flag = false;

void Options::getOptions(int argc, char* argv[]) {
  int option_index = 0;
  option long_opts[] = {
    { "verbose", no_argument, nullptr, 'v' },
    { "help", no_argument, nullptr, 'h' },
    { "decks", required_argument, nullptr, 'd' },
    { "rounds", required_argument, nullptr, 'r' },
    { "seed", required_argument, nullptr, 's' },
    { "players", required_argument, nullptr, 'p' },
    { nullptr, 0, nullptr, '\0' },
  };
  int opt;
  while ((opt = getopt_long(argc, argv, "hvd:r:s:p:", long_opts, &option_index)) != -1) {
    switch (opt) {
      case 'v':
        verbose_flag = true;
        break;
      case 'h':
        ERROR("Not ready yet :(");
        break;
      case 'd':
        num_decks = atoi(optarg);
        break;
      case 'r':
        num_rounds = atoi(optarg);
        break;
      case 's':
        deck_seed = atoi(optarg);
        if(deck_seed == 0) {
          ERROR("Invalid deck seed");
        }
        break;
      case 'p':
        players = optarg;
        break;
      default:
        ERROR("Invalid Argument");
        break;
    }
  }
}
