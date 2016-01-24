#pragma once

#include <iostream>
#include <stdexcept>

#include "InputReader.hpp"
#include "ParameterParser.hpp"
#include "OperationParser.hpp"
#include "ExceptionPrinter.hpp"
#include "ResultCalculator.hpp"
#include "OutputPrinter.hpp"

using namespace std;

class Calculator
{
public:
    Calculator(istream& input, ostream& output):
        input(input),
        output(output)
    {}

    void runCalculator()
    {
        try
        {
            auto equation = InputReader::read(input);
            calculateAndPrintResult(equation);
        }
        catch (const runtime_error& error)
        {
            ExceptionPrinter::print(output, error);
        }
    }

private:
    void calculateAndPrintResult(const Input& equation)
    {
        int a = ParameterParser::parseA(equation);
        int b = ParameterParser::parseB(equation);
        char operation = OperationParser::parse(equation);
        int result = ResultCalculator::calculate(a, b, operation);
        OutputPrinter::print(output, result, operation);
    }

    istream& input;
    ostream& output;
};