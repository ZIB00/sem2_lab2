#pragma once

#include <stdexcept>
#include <string>

class InvalidArgument : public std::invalid_argument
{
    public:
        using std::invalid_argument::invalid_argument;
};

class OutOfRange : public std::out_of_range
{
    public:
        using std::out_of_range::out_of_range;
};
