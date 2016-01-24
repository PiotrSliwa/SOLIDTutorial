#pragma once

#include <istream>
#include <string>

#include "Input.hpp"

using namespace std;

class InputReader
{
public:
    static Input read(istream& input)
    {
        Input inputElements;
        while (not input.eof())
        {
            string str;
            input >> str;
            inputElements.push_back(str);
        }
        return inputElements;
    }
};
