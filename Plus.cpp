#include "BinaryExpression.cpp"

// The class adds two expressions.
class Plus: public BinaryExpression {
public:
    // The super constructor.
    Plus(Expression* left, Expression* right) : BinaryExpression(left, right) {}
    // Calculate the addition.
    double calculate() {
        double x = getLeftExpression()->calculate();
        double y = getRightExpression()->calculate();
        return (x + y);
    }
};