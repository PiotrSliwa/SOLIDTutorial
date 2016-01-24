#pragma once

#include <memory>
#include <string>
#include <algorithm>
#include <stdexcept>

#include "Input.hpp"
#include "Parser.hpp"

using namespace std;

class ParameterParser : public Parser
{
public:
    struct ParametersAreNotNumbersError : runtime_error
    {
        ParametersAreNotNumbersError():
            runtime_error("Given parameters must be numbers!")
        {}
    };

    static int parseA(const Input& inputElements)
    {
        return parse(0, inputElements);
    }

    static int parseB(const Input& inputElements)
    {
        return parse(2, inputElements);
    }

private:
    static int parse(int index, const Input& inputElements)
    {
        assertNumberOfElementsToBeAtLeast(index + 1, inputElements);
        auto& element = inputElements[index];
        if (not isNumber(element))
            throw ParametersAreNotNumbersError();
        return stoi(element);
    }

    static bool isNumber(const string& str)
    {
        auto beginning = begin(str);
        if (str[0] == '-')
            ++beginning;
        return all_of(beginning, end(str), [](auto c){ return isdigit(c); });
    }
};
