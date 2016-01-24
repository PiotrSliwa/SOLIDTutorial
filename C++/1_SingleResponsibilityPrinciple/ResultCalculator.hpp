#pragma once

#include <string>

using namespace std;

class ResultCalculator
{
public:
    static int calculate(int a, int b, char operation)
    {
        switch (operation)
        {
            case '+':
            {
                return a + b;
            }
            case '-':
            {
                return a - b;
            }
            default:
            {
                throw string("Error! Operation " + string(1, operation) + " is not supported for calculations!");
            }
        }
    }
};
