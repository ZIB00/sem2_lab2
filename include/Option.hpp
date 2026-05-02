#pragma once

template<class T>
class Option
{
    private:
        bool hasValue;
        T value;

    public:
        Option();
        Option(T value);
        ~Option();

        bool HasValue();
        T GetValue();
        T ValueOr(T defaultValue);

        template<class R>
        Option<R> Map(R (*)(T));

        template<class R>
        Option<R> FlatMap(Option<R> (*)(T));
};

#include "Option.tpp"