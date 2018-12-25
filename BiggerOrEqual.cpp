#include "Expression.h"

// Used to check the >= condition.
class BiggerOrEqual {
private:
    // The left and right expressions.
    Expression* left;
    Expression* right;
public:
    // Constructor.
    BiggerOrEqual (Expression* one, Expression* two) {
        left = one;
        right = two;
    }
    // Returns true if the left expression is larger or equal to the right expression.
    bool check() {
        return (left->calculate() >= right->calculate());
    }
};
