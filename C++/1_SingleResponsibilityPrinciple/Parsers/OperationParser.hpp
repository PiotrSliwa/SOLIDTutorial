#pragma once

#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

#include "Input.hpp"
#include "Parser.hpp"

using namespace std;

class OperationParser : public Parser
{
public:
    struct OperationNotSupportedError : runtime_error
    {
        OperationNotSupportedError(char operation):
            runtime_error(string("Operation ") + string(1, operation) + string(" is not supported!"))
        {}

        OperationNotSupportedError(string operation):
            runtime_error(string("Operation ") + operation + string(" is not supported!"))
        {}
    };

    static char parse(const Input& inputElements)
    {
        assertNumberOfElementsToBeAtLeast(2, inputElements);
        auto& element = inputElements[1];
        if (element.size() > 1)
            throw OperationNotSupportedError(element);
        char operation = element[0];
        const vector<char> supportedOperations = {'+', '-'};
        if (none_of(begin(supportedOperations), end(supportedOperations), [operation](auto& supported){ return supported == operation; }))
            throw OperationNotSupportedError(operation);
        return operation;
    }
};
