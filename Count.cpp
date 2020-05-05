#include "Count.h"

Count Count::hilo = Count({-1, 1, 1, 1, 1, 1, 0, 0, 0, -1});

double Count::getCountValue(Card card) {
    if(card.getValue() % 10 == 1) {
        return counts[0];
    }
    return counts[card.getValue() - 1];
}