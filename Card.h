#ifndef CARD_H_
#define CARD_H_

class Card {
    int value;
public:
    Card(int _value) : value(_value) {}
    int getValue() const { return value; }
    bool isSoft() const { return value == 11; }
};

#endif // CARD_H_