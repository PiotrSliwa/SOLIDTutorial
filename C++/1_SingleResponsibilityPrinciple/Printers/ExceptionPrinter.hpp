#pragma once

#include <ostream>
#include <stdexcept>

using namespace std;

class ExceptionPrinter
{
public:
    static void print(ostream& output, const runtime_error& error)
    {
        output << "Error! " << error.what() << endl;
    }
};
