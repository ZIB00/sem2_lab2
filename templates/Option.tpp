#include "Option.hpp"

template<class T>
Option<T>::Option()
{
    this->hasValue = false;
    this->value = nullptr;
}

template<class T>
Option<T>::Option(T value)
{
    this->hasValue = true;
    this->value = value;
}

template<class T>
Option<T>::~Option() {}

template<class T>
bool Option<T>::HasValue()
{
    return this->hasValue
}

template<class T>
T Option<T>::GetValue()
{
    if (!this->hasValue) {
        throw LogicError("Attempted to get value from an empty Option (None)");
    }
    return this->value;
}

template<class T>
T Option<T>::ValueOr(T defaultValue)
{
    if (this->hasValue) {
        return this->value;
    }
    return defaultValue;
}

template<class T>
template<class R>
Option<R> Option<T>::Map(R (*function)(T))
{
    if (!hasValue) {
        return Option<R>();
    }
    return Option<R>(function(this->value));
}

template<class T>
template<class R>
Option<R> Option<T>::FlatMap(Option<R> (*transform)(T))
{
    if (!hasValue) {
        return Option<R>();
    }
    return transform(value);
}