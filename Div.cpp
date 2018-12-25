#include "BinaryExpression.cpp"

// The class that calculated the / operation.
class Div: public BinaryExpression {
public:
    // The super constructor.
    Div(Expression* left, Expression* right) : BinaryExpression(left, right) {}
    // Calculate the addition.
    double calculate() {
        double x = getLeftExpression()->calculate();
        double y = getRightExpression()->calculate();
        return (x / y);
    }
};