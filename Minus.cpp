#include "BinaryExpression.cpp"

// The minus class preforms the minus operation on epxressions.
class Minus: public BinaryExpression {
public:
    // The super constructor.
    Minus(Expression* left, Expression* right) : BinaryExpression(left, right) {}
    // Calculate the subtraction.
    double calculate() {
        double x = getLeftExpression()->calculate();
        double y = getRightExpression()->calculate();
        return (x - y);
    }
};