#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

class MyClass
{
public:
    MyClass(istream& input, ostream& output):
        input(input),
        output(output)
    {}

    void runCalculator()
    {
        string equation;
        getline(input, equation);
        output << calculate(equation) << endl;
    }

    string calculate(const string& equation)
    {
        string rawParamA, rawParamB;
        char operation;
        parseInputAndSetValues(equation, rawParamA, rawParamB, operation);

        if (rawParamA.empty() or rawParamB.empty())
        {
            return "Error! Too few parameters!";
        }

        if (not isNumber(rawParamA) or not isNumber(rawParamB))
        {
            return "Error! Given parameters must be numbers!";
        }

        if (not isOperationSupported(operation))
        {
            return "Error! Operation " + string(1, operation) + " is not supported!";
        }

        auto parameterA = stoi(rawParamA);
        auto parameterB = stoi(rawParamB);

        return calculateAndGetResult(parameterA, parameterB, operation);
    }

private:
    bool isOperationSupported(char operation)
    {
        return operation == '+'
            or operation == '-';
    }

    void parseInputAndSetValues(const string& equation, string& parameterA, string& parameterB, char& operation)
    {
        stringstream ss;
        ss << equation;
        ss >> parameterA >> operation >> parameterB;
    }

    bool isNumber(const string& str) const
    {
        auto beginning = begin(str);
        if (*beginning == '-')
            ++beginning;
        return all_of(beginning, end(str), [](auto c){ return isdigit(c); });
    }

    string calculateAndGetResult(int a, int b, char operation)
    {
        string output = "The result of ";

        switch (operation)
        {
            case '+':
            {
                output += "addition is " + to_string(a + b) + "!";
                break;
            }
            case '-':
            {
                output += "substraction is " + to_string(a - b) + "!";
                break;
            }
            default:
            {
                return "***Internal error***";
            }
        }

        return output;
    }

    istream& input;
    ostream& output;
};