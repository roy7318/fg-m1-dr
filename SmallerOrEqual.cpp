#include "Expression.h"

// The class checks the smaller or equal condition.
class SmallerOrEqual {
private:
    // The expressions.
    Expression* left;
    Expression* right;
public:
    // The constructor.
    SmallerOrEqual (Expression* one, Expression* two) {
        left = one;
        right = two;
    }
    // If the left is smaller or equal to the right return true.
    bool check() {
        return (left->calculate() <= right->calculate());
    }
};
