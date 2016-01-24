#pragma once

#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

class OutputPrinter
{
public:
    struct OperationNotSupportedError : runtime_error
    {
        OperationNotSupportedError(char operation):
            runtime_error(string("Operation ") + string(1, operation) + string(" is not supported for printing!"))
        {}
    };

    static void print(ostream& output, int result, char operation)
    {
        auto operationName = getOperationName(operation);
        output << "The result of " << operationName << " is " << result << "!";
    }

private:
    static string getOperationName(char operation)
    {
        switch (operation)
        {
            case '+':
                return "addition";
            case '-':
                return "substraction";
            default:
                throw OperationNotSupportedError(operation);
        }
    }
};
