#include "Expression.h"

// The class checks the < condition.
class Smaller {
private:
    // The expressions.
    Expression* left;
    Expression* right;
public:
    // The constructor.
    Smaller (Expression* one, Expression* two) {
        left = one;
        right = two;
    }
    // Returns true if the left expression is smaller.
    bool check() {
        return (left->calculate() < right->calculate());
    }
};
