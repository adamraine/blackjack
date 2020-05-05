#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <iostream>

struct Options {
    static std::string players;
    static int num_decks;
    static int num_rounds;
    static size_t deck_seed;
    static bool verbose_flag;
    static void getOptions(int argc, char* argv[]);
};

#define VPRINT if(Options::verbose_flag) std::cout

#define ERROR(msg) std::cerr << msg << std:: endl; exit(1)

#endif // OPTIONS_H_