#pragma once

// The expression class, used when calculating expressions given in the command file.
class Expression {
public:
    // Virtual method that calls the calculate function, that calculates the expression.
    virtual double calculate()=0;
};