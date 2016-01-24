#pragma once

#include <string>
#include <stdexcept>

#include "Input.hpp"

using namespace std;

class Parser
{
public:
    struct TooFewParametersError : runtime_error
    {
        TooFewParametersError():
            runtime_error("Too few parameters!")
        {}
    };

protected:
    static void assertNumberOfElementsToBeAtLeast(int minNumberOfElements, const Input& inputElements)
    {
        if (inputElements.size() < minNumberOfElements)
            throw TooFewParametersError();
    }
};
