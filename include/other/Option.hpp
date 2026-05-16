#pragma once
#include <functional>

template<class T>
class Option
{
    private:
        T* value;

    public:
        Option();
        Option(T val);
        Option(const Option<T>& other); 
        ~Option();
        
        Option<T>& operator=(const Option<T>& other);

        bool HasValue();
        T GetValue();
        T ValueOr(T defaultValue);

        template<class R>
        Option<R> Map(std::function<R(T)> function);

        template<class R>
        Option<R> FlatMap(std::function<Option<R>(T)> function); 
};

#include "Option.tpp"