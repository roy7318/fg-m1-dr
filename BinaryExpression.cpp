#pragma once
#include "Expression.h"

// The binary expression class handles mathematical operations on two expressions.
class BinaryExpression: public Expression {
// The members are the first expression, and the second expression.
private:
    Expression* leftExpression;
    Expression* rightExpression;
public:
    // The constructor gets both the expressions.
    BinaryExpression(Expression* left, Expression* right) {
        this->leftExpression = left;
        this->rightExpression = right;
    }
    // Get the left expression.
    Expression* getLeftExpression() {
        return this->leftExpression;
    }
    // Get the right expression.
    Expression* getRightExpression() {
        return this->rightExpression;
    }
    // The calculate method will be done in the sub classes according to their expressions.
     virtual double calculate()=0;
};