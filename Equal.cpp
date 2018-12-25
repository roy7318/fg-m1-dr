#include "Expression.h"

// The class that checks the == condition.
class Equal {
private:
    // The expressions.
    Expression* left;
    Expression* right;
public:
    // Constructor.
    Equal (Expression* one, Expression* two) {
        left = one;
        right = two;
    }
    // Returns true if they are equal.
    bool check() {
        return (left->calculate() == right->calculate());
    }
};
