#pragma once
#include <stack>
#include <string>
#include <cstring>
#include "Expression.h"
#include "Number.cpp"
using namespace std;

// The class implements the Shunting Yard algorithm for calculating expressions.
class ShuntingYardParser {
public:
    // The evaluate function which evaluates the expression.
    Expression* evaluate(string &expression) {
        char ch;
        char tokens[expression.length() + 1];
        strcpy (tokens, expression.c_str());
        // Stack for numbers.
        stack<double > values;
        // Stack for operators.
        stack<char> ops;
        for (int i = 0; i < expression.length() + 1; i++) {
            // If current token is a whitespace, skip it.
            if (tokens[i] == ' ')
                continue;
            // If current token is a number, push it to stack for numbers.
            if (tokens[i] >= '0' && tokens[i] <= '9') {
                string sbuf = "";
                // There may be more than one digits in number.
                while (i < expression.length() + 1 && tokens[i] >= '0' && tokens[i] <= '9')
                    sbuf= sbuf + (tokens[i++]);
                i=i-1;
                values.push((double)stoi(sbuf));
            }
            // If current token is an opening brace, push it to 'ops'.
            else if (tokens[i] == '(')
                ops.push(tokens[i]);
            // If closing brace encountered, solve entire brace.
            else if (tokens[i] == ')') {
                double t =0;
                double s=0;
                while (ops.top() != '(') {
                    if(!values.empty()){
                        t=values.top();
                        values.pop();
                    }
                    if(!values.empty()){
                        s=values.top();
                        values.pop();
                    }
                    ch = ops.top();
                    ops.pop();
                    values.push(applyOp(ch, t, s));
                }
                ops.pop();
            }
                // If current token is an operator.
            else if (tokens[i] == '+' || tokens[i] == '-' ||
                     tokens[i] == '*' || tokens[i] == '/') {
                // While top of 'ops' has same or greater precedence to current
                // token, which is an operator. Apply operator on top of 'ops'
                // to top two elements in values stack.
                char var;
                double x=0;
                double y=0;
                while (!ops.empty() && hasPrecedence(tokens[i], ops.top())) {
                    var = ops.top();
                    ops.pop();
                    if (!values.empty()) {
                        x = values.top();
                        values.pop();
                    }
                    if (!values.empty()) {
                        y = values.top();
                        values.pop();
                    }

                    values.push(applyOp(var, x, y));
                }
                // Push current token to 'ops'.
                ops.push(tokens[i]);
            }
        }
        // Entire expression has been parsed at this point, apply remaining
        // ops to remaining values.
        char c;
        double a;
        double b;
        while (!ops.empty()) {
            c = ops.top();
            ops.pop();
            if (!values.empty()) {
                a = values.top();
                values.pop();
            }
            if (!values.empty()) {
                b = values.top();
                values.pop();
            }
            values.push(applyOp(c, a, b));
        }
        // Top of 'values' contains result, return it
        double val = values.top();
        values.pop();
        Expression* res = new Number(val);
        return res;
    }

    // Returns true if 'op2' has higher or same precedence as 'op1',
    // otherwise returns false.
    bool hasPrecedence(char op1, char op2) {
        if (op2 == '(' || op2 == ')')
            return false;
        if ((op1 == '*' || op1 == '/') && (op2 == '+' || op2 == '-'))
            return false;
        else
            return true;
    }

    // A utility method to apply an operator 'op' on operands 'a'
    // and 'b'. Return the result.
    double applyOp(char op, double b, double a) {
        switch (op) {
            case '+':
                return a + b;
            case '-':
                return a - b;
            case '*':
                return a * b;
            case '/':
                if (b == 0)
                    perror("Cannot divide by zero");
                return a / b;
        }
        return 0;
    }
};