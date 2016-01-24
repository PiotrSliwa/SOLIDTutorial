#include <iostream>
#include <sstream>

#include "Calculator.hpp"

using namespace std;

void testCalculator(string input, string expectedOutput)
{
    stringstream inputStream;
    stringstream outputStream;
    Calculator calculator(inputStream, outputStream);

    inputStream << input;
    calculator.runCalculator();

    string output;
    getline(outputStream, output);
    if (output != expectedOutput)
    {
        cout << "Test failed: " << endl
                << " Expected: \"" << expectedOutput << "\"" << endl
                << "  but was: \"" << output << "\"" << endl;
    }
}

int main()
{
    testCalculator("2 G 3", "Error! Operation G is not supported!");
    testCalculator("2 +- 3", "Error! Operation +- is not supported!");

    /* Tests for addition */
    testCalculator("2 + 3", "The result of addition is 5!");
    testCalculator("2 + -3", "The result of addition is -1!");
    testCalculator("2 +", "Error! Too few parameters!");
    testCalculator("X + 3", "Error! Given parameters must be numbers!");

    /* Tests for substraction */
    testCalculator("2 - 3", "The result of substraction is -1!");
    testCalculator("2 - -3", "The result of substraction is 5!");
    testCalculator("2 -", "Error! Too few parameters!");
    testCalculator("X - 3", "Error! Given parameters must be numbers!");

    return 0;
}
