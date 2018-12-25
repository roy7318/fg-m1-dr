#include "Expression.h"

// User to check the bigger > condition.
class Bigger {
private:
    // The expressions.
    Expression* left;
    Expression* right;
public:
    // The constructor.
    Bigger (Expression* one, Expression* two) {
        left = one;
        right = two;
    }
    // Returns true if the left expression is larger.
    bool check() {
        return (left->calculate() > right->calculate());
    }
};
