#pragma once
#include "Expression.h"

// The number class, inherits from the expression interface, used for generic numerical values.
class Number: public Expression {
// The only member is the value of the number.
private:
    // Stored in a double.
    double value;
public:
    // The constructor.
    Number(double value) {
        this->value = value;
    }
    // Calculating the value of the number simply returns it's value via member.
    double calculate() {
        return this->value;
    }
};