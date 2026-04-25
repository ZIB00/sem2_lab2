#pragma once

#include <stdexcept>
#include <string>

class InvalidArgument : public std::invalid_argument
{
    public:
        explicit InvalidArgument(const std::string& message)
            : std::invalid_argument(message)
        {
        }
};

class OutOfRange : public std::out_of_range
{
    public:
        explicit OutOfRange(const std::string& message)
            : std::out_of_range(message)
        {
        }
};
