#include "Expression.h"

// The class checks the != operation.
class NotEqual {
private:
    // The expressions.
    Expression* left;
    Expression* right;
public:
    // The constructor.
    NotEqual (Expression* one, Expression* two) {
        left = one;
        right = two;
    }
    // Returns true if expression are not equal.
    bool check() {
        return (left->calculate() != right->calculate());
    }
};
