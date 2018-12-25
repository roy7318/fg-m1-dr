#include "BinaryExpression.cpp"

// The multiplication class performs that operation on expressions.
class Mul: public BinaryExpression {
public:
    // The super constructor.
    Mul(Expression* left, Expression* right) : BinaryExpression(left, right) {}
    // Calculate the multiplication.
    double calculate() {
        double x = getLeftExpression()->calculate();
        double y = getRightExpression()->calculate();
        return (x * y);
    }
};